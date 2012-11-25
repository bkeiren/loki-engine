#include <Windows.h>	// Required for wglGetProcAddress
#include <GLEW\\glew.h>
#include <GL\\glut.h>
#include "core/renderer/renderer.h"
#include "core/renderer/scene/scene.h"
#include "core/renderer/GLSLShader/glslshader.h"
#include "core/graphics/Texture2D.h"
#include "core/graphics/TextureCube.h"
#include "core/resourcemanager/texturemanager.h"
#include "core/resourcemanager/modelmanager.h"
#include "core/graphics/effect/EffectManager.h"
#include "core/renderer/debugrenderer.h"
#include "core/graphics/effect/EffectManager.h"
#include "core/window/Window.h"

#include "core/graphics/FrameBuffer.h"
#include "core/graphics/DisplayList.h"

#include "core/entitysystem/component/default/MeshRenderer.h"
#include "core/entitysystem/component/default/Light.h"
#include "core/entitysystem/component/default/Transform.h"
#include "core/entitysystem/component/default/CameraComponent.h"
#include "core/entitysystem/Entity.h"
#include "core/game/Sky.h"

#include "core/graphics/Mesh.h"
#include "core/graphics/SubMesh.h"
#include "core/graphics/Material.h"

using namespace loki;
using namespace loki::renderer;

#define ASSIMP_LOGFILE	"logs//log_assimp.txt"

namespace loki
{

namespace renderer
{

#define GBUFFER_DIFFUSE_SPEC	graphics::FRAMEBUFFER_COLOR_ATTACHMENT0
#define GBUFFER_POSITIONS		graphics::FRAMEBUFFER_COLOR_ATTACHMENT1
#define GBUFFER_NORMALS			graphics::FRAMEBUFFER_COLOR_ATTACHMENT2
#define GBUFFER_DEPTH_STENCIL	graphics::FRAMEBUFFER_DEPTH_STENCIL_ATTACHMENT
#define GBUFFER_LIGHTACCUM		graphics::FRAMEBUFFER_COLOR_ATTACHMENT3

LkRenderer* g_Renderer = NULL;

}

}

LkRenderer::LkRenderer( Window* _Window )	:
	m_LightEffect(0),
	m_GBuffer(0),
#ifdef DBG_VISUALIZATIONS
	m_DBG_VisualizeGBufferTargets(true),
	m_DBG_VisualizeLightVolumes(false),
#endif
	m_Window(_Window)
{
	assert(_Window != 0);

	_GetAPIInformation();
	_LogAPIInformation();

	// In order to avoid crashes (because certain OpenGL methods aren't fully defined), we need to explicitly bind certain
	// methods to their implementations.
	_BindGLFunctions();

	// Initialize the effect manager.
	// NOTE: Must be done before instantiating an object of class LkMRTObject because this class accesses g_EffectManager.
	graphics::g_EffectManager = new graphics::EffectManager();

	if (!_ConstructGBuffer())
	{
		LOG(VL_ERROR, "Renderer::Init: G-Buffer creation failed");
		m_GBuffer = NULL;
	}

	m_LightEffect = graphics::g_EffectManager->CreateEffectFromFile(DEFAULT_RESOURCE("shaders//light.cgfx"), "LightEffect");
	if (!m_LightEffect)
	{
		LOG(VL_ERROR, "Renderer::Init: Failed to load light effect");
	}
	m_LightEffectTechnique_CameraInside = m_LightEffect->GetTechnique("CameraInside");
	m_LightEffectTechnique_CameraOutside = m_LightEffect->GetTechnique("CameraOutside");

	m_LightAccumulationToBackBufferEffect = graphics::g_EffectManager->CreateEffectFromMemory(
#include "core/renderer/lightaccumtobackbuffer_cgeffect.inl"
		, "LightAccumulationToBackBuffer");

#ifdef DBG_VISUALIZATIONS
	m_GBufferTargets_General = graphics::g_EffectManager->CreateEffectFromMemory(
#include "core/renderer/gbuffertargets_general_cgeffect.inl"
		, "GBufferTargets_General");

	m_GBufferTargets_Normals = graphics::g_EffectManager->CreateEffectFromMemory(
#include "core/renderer/gbuffertargets_normals_cgeffect.inl"
		, "GBufferTargets_Normals");

	m_GBufferTargets_Depth = graphics::g_EffectManager->CreateEffectFromMemory(
#include "core/renderer/gbuffertargets_depth_cgeffect.inl"
		, "GBufferTargets_Depth");
#endif

	// Ensure the window is sized properly after Init().
	//ResizeViewport(m_WindowWidth, m_WindowHeight);


	glEnable(GL_CULL_FACE);	// Enable face culling.
	glCullFace(GL_BACK);	// Set face culling type.

	glEnable(GL_DEPTH_TEST);	// Enable depth testing.
	glDepthMask(GL_TRUE);		// Enable the use of the depth mask (Actually enables z-testing).
	glDepthFunc(GL_LEQUAL);		// Set depth test to less-equal.


	// Initialize AssImp library.	
	m_AssImpLogStream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, ASSIMP_LOGFILE);
	aiAttachLogStream(&m_AssImpLogStream);


	LOG(VL_ALWAYS, "Renderer:: Renderer initialized");
}

LkRenderer::LkRenderer()
{
	ILLEGAL_CTOR_ERROR("Renderer");
}

LkRenderer::~LkRenderer()
{
	// Shutdown AssImp.
	aiDetachAllLogStreams();

	// 	delete g_ModelManager;
	// 	g_ModelManager = NULL;

	// 	delete g_TextureManager;
	// 	g_TextureManager = NULL;

	delete graphics::g_EffectManager;
	graphics::g_EffectManager = NULL;

	delete m_GBuffer;
	m_GBuffer = NULL;

	LOG(VL_ALWAYS, "Renderer:: Renderer terminated");
}

void LkRenderer::_GetAPIInformation()
{
	m_OpenGL_Vendor = std::string((const char*)glGetString(GL_VENDOR));
	m_OpenGL_Renderer = std::string((const char*)glGetString(GL_RENDERER));
	m_OpenGL_Version = std::string((const char*)glGetString(GL_VERSION));
	std::string extensions = std::string((const char*)glGetString(GL_EXTENSIONS));
	
	while (extensions.length() > 0)
	{
		int32 next_space = extensions.find((char)32);
		if (next_space != extensions.npos)
		{
			std::string extension = extensions.substr(0, next_space);
			extensions.erase(0, next_space + 1);
			m_OpenGL_Extensions.push_back(extension);
		}
	}
}

void LkRenderer::_LogAPIInformation()
{
	LOG(VL_NORMAL, "Graphics Information:\n\tVendor: %s\n\tCard: %s\n\tAPI Version: %s",
				   m_OpenGL_Vendor.c_str(),
				   m_OpenGL_Renderer.c_str(),
				   m_OpenGL_Version.c_str());
}

// NOTE: Renderer needing to be passed an instance to a level == circular dependency between engine systems.
// The rendering system should not need to know anything about the set-up of levels.
void LkRenderer::Render()
{
	static graphics::EFrameBufferAttachment DrawBuffersP0[] = { GBUFFER_LIGHTACCUM };
	static graphics::EFrameBufferAttachment DrawBuffersP1[] = { GBUFFER_DIFFUSE_SPEC, GBUFFER_POSITIONS, GBUFFER_NORMALS, GBUFFER_LIGHTACCUM }; 

	m_GBuffer->Bind();
	m_GBuffer->SetDrawBuffers(DrawBuffersP1, 4);
	//m_GBuffer->ClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Render the sky to the light accumulation buffer.
	m_GBuffer->SetDrawBuffers(DrawBuffersP0, 1);
	_RenderSky();

	m_GBuffer->SetDrawBuffers(DrawBuffersP1, 4);
	const vec4& vp = components::CameraComponent::GetActiveCamera()->GetViewPort();
	glViewport(vp.x * m_Window->GetWidth(), 
			   vp.y * m_Window->GetHeight(), 
			   vp.z * m_Window->GetWidth(), 
			   vp.w * m_Window->GetHeight());
	
		_RenderOpaqueGeometry();

		_RenderParticles();

		// Draw debug stuff.
		debug::DrawItems(components::CameraComponent::GetActiveCamera()->GetProjectionMatrix(), 
						 components::CameraComponent::GetActiveCamera()->GetViewMatrix());

		m_GBuffer->SetDrawBuffer(GBUFFER_LIGHTACCUM);
		
		_RenderLighting();

	m_GBuffer->Unbind();

	_RenderLightAccumulationToBackBuffer();
	
#ifdef DBG_VISUALIZATIONS
	if (m_DBG_VisualizeGBufferTargets)
	{
		_RenderGBufferTargets();
	}
#endif

	// Draw UI.
	_RenderUI();

	// Check for any OpenGL errors.
	CheckGLError();
}

void LkRenderer::_BindGLFunctions()
{
#define REGISTER_GL_FUNC(func, type)	(func = (type)wglGetProcAddress(#func))
	// Example:
	// glGenFramebuffersEXT			= (PFNGLGENFRAMEBUFFERSEXTPROC)			wglGetProcAddress("glGenFramebuffersEXT");
	// becomes
	// REGISTER_GL_FUNC(glGenFramebuffersEXT, PFNGLGENFRAMEBUFFERSEXTPROC);

	REGISTER_GL_FUNC( glGenFramebuffersEXT, PFNGLGENFRAMEBUFFERSEXTPROC );
	REGISTER_GL_FUNC( glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC );
	REGISTER_GL_FUNC( glGenRenderbuffersEXT, PFNGLGENRENDERBUFFERSEXTPROC );
	REGISTER_GL_FUNC( glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC );
	REGISTER_GL_FUNC( glBindFramebufferEXT, PFNGLBINDFRAMEBUFFEREXTPROC );
	REGISTER_GL_FUNC( glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC );
	REGISTER_GL_FUNC( glBindRenderbufferEXT, PFNGLBINDRENDERBUFFEREXTPROC );
	REGISTER_GL_FUNC( glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC );
	REGISTER_GL_FUNC( glRenderbufferStorageEXT, PFNGLRENDERBUFFERSTORAGEEXTPROC );
	REGISTER_GL_FUNC( glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC );
	REGISTER_GL_FUNC( glFramebufferRenderbufferEXT, PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC );
	REGISTER_GL_FUNC( glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC );
	REGISTER_GL_FUNC( glFramebufferTexture2DEXT, PFNGLFRAMEBUFFERTEXTURE2DEXTPROC );
	REGISTER_GL_FUNC( glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DPROC );
	REGISTER_GL_FUNC( glCheckFramebufferStatusEXT, PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC );
	REGISTER_GL_FUNC( glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC );
	REGISTER_GL_FUNC( glDeleteFramebuffersEXT, PFNGLDELETEFRAMEBUFFERSEXTPROC );
	REGISTER_GL_FUNC( glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC );
	REGISTER_GL_FUNC( glDeleteRenderbuffersEXT, PFNGLDELETERENDERBUFFERSEXTPROC );
	REGISTER_GL_FUNC( glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC );
	REGISTER_GL_FUNC( glActiveTextureARB, PFNGLACTIVETEXTUREARBPROC );
	REGISTER_GL_FUNC( glDrawBuffers, PFNGLDRAWBUFFERSPROC );
	REGISTER_GL_FUNC( glUseProgramObjectARB, PFNGLUSEPROGRAMOBJECTARBPROC );
	REGISTER_GL_FUNC( glActiveTextureARB, PFNGLACTIVETEXTUREARBPROC );
	REGISTER_GL_FUNC( glActiveTexture, PFNGLACTIVETEXTUREPROC );
	REGISTER_GL_FUNC( glUniform1iARB, PFNGLUNIFORM1IARBPROC );
	REGISTER_GL_FUNC( glUniform1fARB, PFNGLUNIFORM1FARBPROC );
	REGISTER_GL_FUNC( glUniform2iARB, PFNGLUNIFORM2IARBPROC );
	REGISTER_GL_FUNC( glUniform2fARB, PFNGLUNIFORM2FARBPROC );
	REGISTER_GL_FUNC( glUniform3iARB, PFNGLUNIFORM3IARBPROC );
	REGISTER_GL_FUNC( glUniform3fARB, PFNGLUNIFORM3FARBPROC );
	REGISTER_GL_FUNC( glUniform4iARB, PFNGLUNIFORM4IARBPROC );
	REGISTER_GL_FUNC( glUniform4fARB, PFNGLUNIFORM4FARBPROC );
	REGISTER_GL_FUNC( glCreateShaderObjectARB, PFNGLCREATESHADEROBJECTARBPROC );
	REGISTER_GL_FUNC( glShaderSourceARB, PFNGLSHADERSOURCEARBPROC );
	REGISTER_GL_FUNC( glCompileShaderARB, PFNGLCOMPILESHADERARBPROC );
	REGISTER_GL_FUNC( glGetObjectParameterivARB, PFNGLGETOBJECTPARAMETERIVARBPROC );
	REGISTER_GL_FUNC( glCreateProgramObjectARB, PFNGLCREATEPROGRAMOBJECTARBPROC );
	REGISTER_GL_FUNC( glAttachObjectARB, PFNGLATTACHOBJECTARBPROC );
	REGISTER_GL_FUNC( glLinkProgramARB, PFNGLLINKPROGRAMARBPROC );
	REGISTER_GL_FUNC( glGetUniformLocationARB, PFNGLGETUNIFORMLOCATIONARBPROC );
	REGISTER_GL_FUNC( glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC );
	REGISTER_GL_FUNC( glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC );
	REGISTER_GL_FUNC( glGenerateMipmap, PFNGLGENERATEMIPMAPPROC );
	REGISTER_GL_FUNC( glGenBuffers, PFNGLGENBUFFERSPROC );
	REGISTER_GL_FUNC( glBindBuffer, PFNGLBINDBUFFERPROC );
	REGISTER_GL_FUNC( glBufferData, PFNGLBUFFERDATAARBPROC );
	REGISTER_GL_FUNC( glBufferSubData, PFNGLBUFFERSUBDATAPROC );
	REGISTER_GL_FUNC( glDeleteBuffers, PFNGLDELETEBUFFERSPROC );
	REGISTER_GL_FUNC( glClientActiveTexture, PFNGLCLIENTACTIVETEXTUREPROC );
	REGISTER_GL_FUNC( glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC );
	REGISTER_GL_FUNC( glBindAttribLocation, PFNGLBINDATTRIBLOCATIONPROC );
	REGISTER_GL_FUNC( glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC );
	REGISTER_GL_FUNC( glGetAttribLocation, PFNGLGETATTRIBLOCATIONPROC );
	REGISTER_GL_FUNC( glVertexAttrib1f, PFNGLVERTEXATTRIB1FPROC );
	REGISTER_GL_FUNC( glVertexAttrib2f, PFNGLVERTEXATTRIB2FPROC );
	REGISTER_GL_FUNC( glVertexAttrib3f, PFNGLVERTEXATTRIB3FPROC );
	REGISTER_GL_FUNC( glVertexAttrib4f, PFNGLVERTEXATTRIB4FPROC );
	REGISTER_GL_FUNC( glVertexAttrib1d, PFNGLVERTEXATTRIB1DPROC );
	REGISTER_GL_FUNC( glVertexAttrib2d, PFNGLVERTEXATTRIB2DPROC );
	REGISTER_GL_FUNC( glVertexAttrib3d, PFNGLVERTEXATTRIB3DPROC );
	REGISTER_GL_FUNC( glVertexAttrib4d, PFNGLVERTEXATTRIB4DPROC );
	REGISTER_GL_FUNC( glVertexAttrib1s, PFNGLVERTEXATTRIB1SPROC );
	REGISTER_GL_FUNC( glVertexAttrib2s, PFNGLVERTEXATTRIB2SPROC );
	REGISTER_GL_FUNC( glVertexAttrib3s, PFNGLVERTEXATTRIB3SPROC );
	REGISTER_GL_FUNC( glVertexAttrib4s, PFNGLVERTEXATTRIB4SPROC );
	REGISTER_GL_FUNC( glVertexAttrib4Nub, PFNGLVERTEXATTRIB4NUBPROC );
	REGISTER_GL_FUNC( glVertexAttrib1fv, PFNGLVERTEXATTRIB1FVPROC );
	REGISTER_GL_FUNC( glVertexAttrib2fv, PFNGLVERTEXATTRIB2FVPROC );
	REGISTER_GL_FUNC( glVertexAttrib3fv, PFNGLVERTEXATTRIB3FVPROC );
	REGISTER_GL_FUNC( glVertexAttrib4fv, PFNGLVERTEXATTRIB4FVPROC );
	REGISTER_GL_FUNC( glVertexAttrib1dv, PFNGLVERTEXATTRIB1DVPROC );
	REGISTER_GL_FUNC( glVertexAttrib2dv, PFNGLVERTEXATTRIB2DVPROC );
	REGISTER_GL_FUNC( glVertexAttrib3dv, PFNGLVERTEXATTRIB3DVPROC );
	REGISTER_GL_FUNC( glVertexAttrib4dv, PFNGLVERTEXATTRIB4DVPROC );
	REGISTER_GL_FUNC( glVertexAttrib1sv, PFNGLVERTEXATTRIB1SVPROC );
	REGISTER_GL_FUNC( glVertexAttrib2sv, PFNGLVERTEXATTRIB2SVPROC );
	REGISTER_GL_FUNC( glVertexAttrib3sv, PFNGLVERTEXATTRIB3SVPROC );
	REGISTER_GL_FUNC( glVertexAttrib4sv, PFNGLVERTEXATTRIB4SVPROC );
	REGISTER_GL_FUNC( glUseProgram, PFNGLUSEPROGRAMPROC );
	REGISTER_GL_FUNC( glUniform1i, PFNGLUNIFORM1IPROC );
	REGISTER_GL_FUNC( glUniform1iv, PFNGLUNIFORM1IVPROC );
	REGISTER_GL_FUNC( glUniform2i, PFNGLUNIFORM2IPROC );
	REGISTER_GL_FUNC( glUniform2iv, PFNGLUNIFORM2IVPROC );
	REGISTER_GL_FUNC( glUniform3i, PFNGLUNIFORM3IPROC );
	REGISTER_GL_FUNC( glUniform3iv, PFNGLUNIFORM3IVPROC );
	REGISTER_GL_FUNC( glUniform4i, PFNGLUNIFORM4IPROC );
	REGISTER_GL_FUNC( glUniform4iv, PFNGLUNIFORM4IVPROC );
	REGISTER_GL_FUNC( glUniform1f, PFNGLUNIFORM1FPROC );
	REGISTER_GL_FUNC( glUniform1fv, PFNGLUNIFORM1FVPROC );
	REGISTER_GL_FUNC( glUniform2f, PFNGLUNIFORM2FPROC );
	REGISTER_GL_FUNC( glUniform2fv, PFNGLUNIFORM2FVPROC );
	REGISTER_GL_FUNC( glUniform3f, PFNGLUNIFORM3FPROC );
	REGISTER_GL_FUNC( glUniform3fv, PFNGLUNIFORM3FVPROC );
	REGISTER_GL_FUNC( glUniform4f, PFNGLUNIFORM4FPROC );
	REGISTER_GL_FUNC( glUniform4fv, PFNGLUNIFORM4FVPROC );
	REGISTER_GL_FUNC( glUniform1d, PFNGLUNIFORM1DPROC );
	REGISTER_GL_FUNC( glUniform1dv, PFNGLUNIFORM1DVPROC );
	REGISTER_GL_FUNC( glUniform2d, PFNGLUNIFORM2DPROC );
	REGISTER_GL_FUNC( glUniform2dv, PFNGLUNIFORM2DVPROC );
	REGISTER_GL_FUNC( glUniform3d, PFNGLUNIFORM3DPROC );
	REGISTER_GL_FUNC( glUniform3dv, PFNGLUNIFORM3DVPROC );
	REGISTER_GL_FUNC( glUniform4d, PFNGLUNIFORM4DPROC );
	REGISTER_GL_FUNC( glUniform4dv, PFNGLUNIFORM4DVPROC );
	REGISTER_GL_FUNC( glUniform1ui, PFNGLUNIFORM1UIPROC );
	REGISTER_GL_FUNC( glUniform1uiv, PFNGLUNIFORM1UIVPROC );
	REGISTER_GL_FUNC( glUniform2ui, PFNGLUNIFORM2UIPROC );
	REGISTER_GL_FUNC( glUniform2uiv, PFNGLUNIFORM2UIVPROC );
	REGISTER_GL_FUNC( glUniform3ui, PFNGLUNIFORM3UIPROC );
	REGISTER_GL_FUNC( glUniform3uiv, PFNGLUNIFORM3UIVPROC );
	REGISTER_GL_FUNC( glUniform4ui, PFNGLUNIFORM4UIPROC );
	REGISTER_GL_FUNC( glUniform4uiv, PFNGLUNIFORM4UIVPROC );
	REGISTER_GL_FUNC( glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC );
	REGISTER_GL_FUNC( glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC );
	REGISTER_GL_FUNC( glGetFramebufferAttachmentParameteriv, PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC );
	REGISTER_GL_FUNC( glMapBuffer, PFNGLMAPBUFFERPROC );
	REGISTER_GL_FUNC( glUnmapBuffer, PFNGLUNMAPBUFFERPROC );
	REGISTER_GL_FUNC( glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC );
	REGISTER_GL_FUNC( glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC );
	REGISTER_GL_FUNC( glBindVertexArray, PFNGLBINDVERTEXARRAYPROC );

#undef REGISTER_GL_FUNC
}

void LkRenderer::ToggleWireframe()
{
	GLint mode[2];	// glGetIntegerv apparently writes outside the 32 bits that a single GLint occupies, so this is a simple fix.
	glGetIntegerv(GL_POLYGON_MODE, mode);
	switch (mode[0])
	{
	case GL_LINE:
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
	case GL_FILL:
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}
	}
}

void LkRenderer::CheckGLError()
{
	GLenum error = 0;
	do
	{
		error = glGetError();
		switch (error)
		{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			LOG(VL_ERROR, "CheckGLError: GL_INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:
			LOG(VL_ERROR, "CheckGLError: GL_INVALID_VALUE");
			break;
		case GL_INVALID_OPERATION:
			LOG(VL_ERROR, "CheckGLError: GL_INVALID_OPERATION (Invalid function called within glBegin(), glEnd() pair?)");
			break;
		case GL_STACK_OVERFLOW:
			LOG(VL_ERROR, "CheckGLError: GL_STACK_OVERFLOW");
			break;
		case GL_STACK_UNDERFLOW:
			LOG(VL_ERROR, "CheckGLError: GL_STACK_UNDERFLOW");
			break;
		case GL_OUT_OF_MEMORY:
			LOG(VL_ERROR, "CheckGLError: GL_OUT_OF_MEMORY");
			break;
		case GL_TABLE_TOO_LARGE:
			LOG(VL_ERROR, "CheckGLError: GL_TABLE_TOO_LARGE");
			break;
		default:
			LOG(VL_ERROR, "CheckGLError: Unhandled error (%i)", (int32)error);
			break;
		}
	} while (error != GL_NO_ERROR);
}

#ifdef DBG_VISUALIZATIONS
void LkRenderer::ToggleVisualizeRenderTargets()
{
	m_DBG_VisualizeGBufferTargets = !m_DBG_VisualizeGBufferTargets;
}

void LkRenderer::ToggleVisualizeLightVolumes()
{
	m_DBG_VisualizeLightVolumes = !m_DBG_VisualizeLightVolumes;
}
#endif

void LkRenderer::DrawPixels( int32 _Width, int32 _Height, graphics::EInternalFormat _Format, const void* _Buffer )
{
	glDrawPixels(_Width, _Height, graphics::GLInternalFormats[_Format], GL_UNSIGNED_BYTE, _Buffer);
}

int32 LkRenderer::GetWindowWidth()
{
	return m_Window->GetWidth();
}

int32 LkRenderer::GetWindowHeight()
{
	return m_Window->GetHeight();
}

int32 LkRenderer::GetRenderWidth()
{
	return m_GBuffer->GetWidth();
}

int32 LkRenderer::GetRenderHeight()
{
	return m_GBuffer->GetHeight();
}

vec2 LkRenderer::GetPixelScale()
{
	return vec2((f32)renderer::g_Renderer->GetRenderWidth() / renderer::g_Renderer->GetWindowWidth(),
					 (f32)renderer::g_Renderer->GetRenderHeight() / renderer::g_Renderer->GetWindowHeight());
}

void LkRenderer::_RenderSky()
{
	game::Sky::Render();
}

void LkRenderer::_RenderOpaqueGeometry()
{
	//glCullFace(GL_BACK);	// Handled by Cg.
	glDepthFunc( GL_LEQUAL );
	glColorMask(true, true, true, true);
	glDepthMask(true);

	components::CameraComponent* camera = components::CameraComponent::GetActiveCamera();
	mat4 viewmatrix = camera->GetViewMatrix();
	mat4 projectionmatrix = camera->GetProjectionMatrix();
	f32 zfar = camera->GetFarPlane();
	f32 znear = camera->GetNearPlane();

	for (components::MeshRenderer::MeshRenderersConstIter it = components::MeshRenderer::m_MeshRenderers.begin();
		 it != components::MeshRenderer::m_MeshRenderers.end();
		 ++it)
	{
		components::MeshRenderer* rc = (*it);	// Must have MeshFilter too.

		if (rc->m_Mesh)	// TODO: Replace this check with a default model to indicate missing models.
		{
			graphics::Mesh* mesh = rc->m_Mesh;

			const mat4& _ModelMatrix = rc->GetTransform().GetMatrix();
			const mat4& _ViewMatrix = viewmatrix;
			const mat4& _ProjectionMatrix = projectionmatrix;
			float _ZFar = zfar;
			float _ZNear = znear;

			graphics::TextureCube* SkyCubeMap = game::Sky::GetCubeMap();

			for (uint32 i = 0; i < mesh->GetSubMeshCount(); ++i)
			{
				const graphics::SubMesh* submesh = mesh->GetSubMesh(i);
				const graphics::Material* material = rc->m_Materials[math::clamp(i,(uint32) 0, (uint32)rc->m_Materials.size() - 1)];
				graphics::Effect* effect = material->GetEffect();

				if (!effect)
				{
					LOG(VL_ERROR, "Model::_Render: No effect associated with material");
					continue;
				}

				graphics::EffectParameter* param = 0;

				// Set the global ambient color.
				// TODO.

#define SETCGPARAM(paramname, value)	{param = effect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

				SETCGPARAM("LKMODELVIEWPROJ", _ProjectionMatrix * _ViewMatrix * _ModelMatrix);		// Set the model view projection matrix.
				SETCGPARAM("LKMODELMATRIX", _ModelMatrix);			// Set the model matrix.
				SETCGPARAM("LKMODELMATRIXIT", mat3(math::inverseTranspose(_ModelMatrix)));		// Set the inverse transpose of the model matrix.	
				SETCGPARAM("LKEYEPOSITION", math::inverse(_ViewMatrix)[3]);			// Set the eye position.
				SETCGPARAM("LKVIEWMATRIX", _ViewMatrix);
				SETCGPARAM("LKUVSCALE", material->GetUVScale());		// Set the UV scale.
				SETCGPARAM("LKZFAR", _ZFar);	// Set the Z-far value.
				SETCGPARAM("LKZNEAR", _ZNear);	// Set the Z-near value.
				SETCGPARAM("LKMATERIALSHININESS", material->GetShininess());
				SETCGPARAM("LKMATERIALREFLECTIVITY", material->GetReflectivity());
				SETCGPARAM("LKSKYSAMPLER", SkyCubeMap ? SkyCubeMap->GetTextureHandle() : 0);





				// 		static graphics::TextureCube* CubeMapTest = graphics::TextureCube::Load("resources//textures//cubemap.bmp");
				// 		SETCGPARAM("LKENVCUBEMAP", CubeMapTest->GetTextureHandle());





				const graphics::Texture2D* tex = 0;

				// Set the diffuse texture.
				tex = material->GetTexture(graphics::Material::TT_DIFFUSE);
				SETCGPARAM("LKDIFFUSETEX", (tex)?(tex->GetTextureHandle()):((GLuint)0));

				// Set the normal texture.
				tex = material->GetTexture(graphics::Material::TT_NORMAL);
				SETCGPARAM("LKNORMALTEX", (tex)?(tex->GetTextureHandle()):((GLuint)0));

				// Set the specular texture.
				tex = material->GetTexture(graphics::Material::TT_SPECULAR);
				SETCGPARAM("LKSPECULARTEX", (tex)?(tex->GetTextureHandle()):((GLuint)0));

				// Set the emissive texture.
				tex = material->GetTexture(graphics::Material::TT_EMISSIVE);
				SETCGPARAM("LKEMISSIVETEX", (tex)?(tex->GetTextureHandle()):((GLuint)0));

				while (effect->HasNextPass())
				{
					submesh->Draw();
				}

#undef SETCGPARAM
			}
		}
	}
}

void LkRenderer::_RenderLighting()
{
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	// Don't clear the color buffer. This is done when the Gbuffer is initialized.
									// We want to keep the content in the lighting buffer at this point because it has been
									// initialized with emissive texture data (Any texture that always needs to be at full intensity, regardless of light levels).

	// Enable blending.
	glEnable( GL_BLEND );
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable( GL_STENCIL_TEST );	// Enable stencil testing.
	glDepthMask(false);	// Disable depth-writing.

	// Clear the stencil buffer just once when we start lighting.
	// After each light, the stencil buffer should have been cleared to 0 because we tell OpenGL to zero out the stencil
	// buffer at fragments that pass the stencil test in the final pass.
	glClear(GL_STENCIL_BUFFER_BIT);


#define SETCGPARAM(paramname, value)	{graphics::EffectParameter* param = m_LightEffect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

	components::CameraComponent* camera = components::CameraComponent::GetActiveCamera();
	mat4 _ProjectionMatrix = camera->GetProjectionMatrix();
	mat4 _ViewMatrix = camera->GetViewMatrix();
	mat4 _ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	vec3 _EyePosition = camera->GetEntity()->GetTransform().GetPosition();

	graphics::Texture2D* PointLightAttenuationTexture = components::Light::GetPointAttenuationTexture();
	graphics::Texture2D* SpotLightAttenuationTexture = components::Light::GetSpotAttenuationTexture();

	SETCGPARAM("LKEYEPOSITION", _EyePosition);
	SETCGPARAM("LKRT0", m_GBuffer->GetAttachmentTexture(GBUFFER_DIFFUSE_SPEC));
	SETCGPARAM("LKRT1", m_GBuffer->GetAttachmentTexture(GBUFFER_POSITIONS));
	SETCGPARAM("LKRT2", m_GBuffer->GetAttachmentTexture(GBUFFER_NORMALS));
	SETCGPARAM("LKRT3", m_GBuffer->GetAttachmentTexture(GBUFFER_DEPTH_STENCIL));
	SETCGPARAM("LKPOINTATTTEXTURE", (PointLightAttenuationTexture)?(PointLightAttenuationTexture->GetTextureHandle()):(0));
	SETCGPARAM("LKSPOTATTTEXTURE", (SpotLightAttenuationTexture)?(SpotLightAttenuationTexture->GetTextureHandle()):(0));
	SETCGPARAM("LKZNEAR", camera->GetNearPlane());
	SETCGPARAM("LKZFAR", camera->GetFarPlane());
	SETCGPARAM("LKVIEWMATRIX", _ViewMatrix);

	components::Light::Lights& lights = components::Light::GetAllLights();
	for (components::Light::LightsConstIter it = lights.begin(); it != lights.end(); ++it)
	{
		components::Light* light = (*it);

		Transform& transform = light->GetEntity()->GetTransform();
		mat4 _ModelMatrix = transform.GetMatrix();
		//_ModelMatrix = math::gtc::matrix_transform::rotate(_ModelMatrix, 90.0f, vec3(1.0f, 0.0f, 0.0f));
		SETCGPARAM("LKMODELVIEWPROJ", _ViewProjectionMatrix * _ModelMatrix);		// Set the model view projection matrix.
		SETCGPARAM("LKMODELMATRIX", _ModelMatrix);			// Set the model matrix.
		SETCGPARAM("LKMODELMATRIXIT", mat3(math::inverseTranspose(_ModelMatrix)));		// Set the inverse transpose of the model matrix.	
		SETCGPARAM("LKLIGHTPOSITION", transform.GetPosition());
		SETCGPARAM("LKLIGHTRANGE", light->GetRange());
		SETCGPARAM("LKLIGHTCOLOR", light->GetColor());
		SETCGPARAM("LKLIGHTINTENSITY", light->GetIntensity());
		SETCGPARAM("LKLIGHTTYPE", light->GetLightType());

		bool CameraInsideVolume = false;
		switch (light->GetLightType())
		{
		case components::Light::LIGHT_POINT:
			{
				CameraInsideVolume = math::length(_EyePosition - transform.GetPosition()) < light->GetRange();
				break;
			}
		case components::Light::LIGHT_SPOT:
			{
				f32 SpotAngle = light->GetSpotAngle();
				vec3 LightVec = transform.GetOrientationVector();
				SETCGPARAM("LKIGHTSPOTANGLE", SpotAngle);
				SETCGPARAM("LKLIGHTVECTOR", LightVec);

				// Calculate if the camera is inside the cone shape.
				// NOTE: A special case is when the camera is actually at the very tip of the cone shape.
				// To handle this, an epsilon value (0.0001f) is used which is the margin of error that
				// has been observed when calculating the distance from the camera to the actual light.
				// If the distance from the camera to the light is less than this epsilon value, we 
				// handle that case as if the camera is actually inside the cone volume.
				// Otherwise, we calculate the angle between the camera and the light direction. If this
				// angle is less than the cone angle, we're inside. Else we're outside. Works like a charm.
				// NOTE: This could also be 'fixed' by always having the cone volume translated slightly so that
				// the tip is not at the actual position of the transform. This would still cause an issue when the two
				// positions align, but that should be very, very, very rare (Most likely never). That would save us
				// from handling a special case here.
				vec3 LightToEye = _EyePosition - transform.GetPosition();
				float LightToEyeLength = math::length(LightToEye);
				f32 d = (LightToEyeLength > 0.0001f) ?	// If the distance from the light to the camera is greater than our error margin...
					(math::degrees(math::acos(math::dot(LightToEye / LightToEyeLength, LightVec)))) :	// Calculate the actual angle.
				(0.0f);	// Assume we're inside.
				CameraInsideVolume =	(LightToEyeLength < light->GetRange()) && 
					(d <= SpotAngle * 0.5f);
				break;
			}
		default:
			{
				break;
			}
		}

		m_LightEffect->SetActiveTechnique( CameraInsideVolume ? m_LightEffectTechnique_CameraInside : m_LightEffectTechnique_CameraOutside );
		while (m_LightEffect->HasNextPass())
		{
			light->GetGeometry()->Draw();
		}
#ifdef DBG_VISUALIZATIONS
		if (m_DBG_VisualizeLightVolumes)
		{
			m_LightEffect->SetActiveTechnique("DebugDraw");
			while (m_LightEffect->HasNextPass())
			{
				light->GetGeometry()->Draw();
			}
		}
#endif
	}


	glDepthMask(true);	// Enable depth-writing again.
	glDisable( GL_STENCIL_TEST );	// Disable stencil testing.
	glDisable(GL_BLEND);
	glColorMask(true, true, true, true);	// Enable color-writing.
}

void LkRenderer::_RenderTransparentGeometry()
{

}

// void Renderer::RenderPreUIPostProcess()
// {
// 	// Render a full-screen quad with the post-process shader.
// 
// 	glColorMask(true, true, true, true);
// 	glDepthMask(false);
// 	glDisable(GL_STENCIL_TEST);
// 	glDisable(GL_CULL_FACE);
// 	glDisable(GL_DEPTH_TEST);
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_ONE, GL_ONE);
// 	glMatrixMode(GL_MODELVIEW);
// 	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
// 	glMatrixMode(GL_MODELVIEW);
// 	glPushMatrix();
// 	glLoadIdentity();
// 	glMatrixMode(GL_PROJECTION);
// 	glPushMatrix();
// 	glLoadIdentity();
// 	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
// 
// 	glUseProgramObjectARB(m_PPShader.GetProgramHandle());
// 
// 	glUniform2fARB(m_PPShaderScreenDimensionsID, m_WindowWidth, m_WindowHeight);
// 	glActiveTextureARB(GL_TEXTURE0_ARB);
// 	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT0());
// 	glActiveTextureARB(GL_TEXTURE1_ARB);
// 	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT1());
// 	glActiveTextureARB(GL_TEXTURE2_ARB);
// 	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT2());
// 	glActiveTextureARB(GL_TEXTURE3_ARB);
// 	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRTDepthStencil());
// 	glActiveTextureARB(GL_TEXTURE4_ARB);
// 	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRTLightAccumulation());
// 	glUniform1iARB(m_PPShaderRT0ID, 0);
// 	glUniform1iARB(m_PPShaderRT1ID, 1);
// 	glUniform1iARB(m_PPShaderRT2ID, 2);
// 	glUniform1iARB(m_PPShaderRTDepthID, 3);
// 
// 	// Render a full-screen quad.
// 	glBegin(GL_QUADS);
// 	glTexCoord2f(0.0f, 1.0f);
// 	glVertex3f(0.0f, 1.0f, 0.0f);
// 
// 	glTexCoord2f(1.0f, 1.0f);
// 	glVertex3f(1.0f, 1.0f, 0.0f);
// 
// 	glTexCoord2f(1.0f, 0.0f);
// 	glVertex3f(1.0f, 0.0f, 0.0f);
// 
// 	glTexCoord2f(0.0f, 0.0f);
// 	glVertex3f(0.0f, 0.0f, 0.0f);
// 	glEnd();
// 
// 	glActiveTextureARB(GL_TEXTURE0_ARB);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glActiveTextureARB(GL_TEXTURE1_ARB);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glActiveTextureARB(GL_TEXTURE2_ARB);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 	glActiveTextureARB(GL_TEXTURE3_ARB);
// 	glBindTexture(GL_TEXTURE_2D, 0);
// 
// 	glUseProgramObjectARB(0);
// 
// 	glDisable(GL_BLEND);
// 	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
// 	glEnable(GL_DEPTH_TEST);
// 	glEnable(GL_CULL_FACE);
// 	glMatrixMode(GL_PROJECTION);
// 	glPopMatrix();
// 	glMatrixMode(GL_MODELVIEW);
// 	glPopMatrix();
// }

void LkRenderer::_RenderUIObjects()
{

}

void LkRenderer::_RenderUI()
{
	
}

void LkRenderer::_RenderParticles()
{
// 	const game::LkLevel::ParticleSystems* systems = m_CurrentLevelToRender->GetParticleSystems();
// 	for (game::LkLevel::ParticleSystems::const_iterator it = systems->begin(); it != systems->end(); ++it)
// 	{
// 		(*it).second->Render(m_CurrentLevelToRender->GetCurrentCamera()->GetViewMatrix(), m_CurrentLevelToRender->GetCurrentCamera()->GetProjectionMatrix());
// 	}
}

void LkRenderer::_RenderLightAccumulationToBackBuffer()
{
#ifdef SETCGPARAM
#undef SETCGPARAM
#endif

#define SETCGPARAM(paramname, value)	{graphics::EffectParameter* param = m_LightAccumulationToBackBufferEffect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

	//SETCGPARAM("LKLIGHTACCUMULATIONTEX", m_GBuffer->GetRenderbufferTexture(4));
	SETCGPARAM("LKLIGHTACCUMULATIONTEX", m_GBuffer->GetAttachmentTexture(GBUFFER_LIGHTACCUM));

	while (m_LightAccumulationToBackBufferEffect->HasNextPass())
	{
		// TODO: Remove immediate mode. Make a displaylist or something?
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(-1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(1.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(1.0f, -1.0f);
		glEnd();
	}

#undef SETCGPARAM
}

void LkRenderer::_RenderGBufferTargets()
{
#ifdef SETCGPARAM
#undef SETCGPARAM
#endif

#define SETCGPARAM(paramname, value)	{graphics::EffectParameter* param = effect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

	for (int32 i = 0; i < 4; ++i)
	{
		f32 x = -1.0f + (0.5f * i);

		static graphics::EFrameBufferAttachment Attachments[4] = { GBUFFER_DIFFUSE_SPEC, GBUFFER_POSITIONS, GBUFFER_NORMALS, GBUFFER_DEPTH_STENCIL };

		graphics::Effect* effect = 0;
		switch (i)
		{
		case 2:
			{
				effect = m_GBufferTargets_Normals;
				break;
			}
		case 3:
			{
				effect = m_GBufferTargets_Depth;
				SETCGPARAM("LKZFAR", components::CameraComponent::GetActiveCamera()->GetFarPlane());
				SETCGPARAM("LKZNEAR", components::CameraComponent::GetActiveCamera()->GetNearPlane());
				break;
			}
		default:
			{
				effect = m_GBufferTargets_General;
				break;
			}
		}
		SETCGPARAM("LKGBUFFERTEX", m_GBuffer->GetAttachmentTexture(Attachments[i]));

		while (effect->HasNextPass())
		{
			// TODO: Remove immediate mode. Make a displaylist or something?
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(x, -1.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(x, -0.5f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(x + 0.5f, -0.5f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(x + 0.5f, -1.0f);
			glEnd();
		}
	}

#undef SETCGPARAM
}

bool LkRenderer::_ConstructGBuffer()
{
	int32 WindowWidth = m_Window->GetWidth();
	int32 WindowHeight = m_Window->GetHeight();

	std::vector<graphics::FrameBuffer::FrameBufferAttachmentInfo> RenderBuffersInfo;
	{
		// Diffuse and specular color buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		info.m_Attachment = GBUFFER_DIFFUSE_SPEC;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
		info.m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_8_8_8_8;
		RenderBuffersInfo.push_back(info);
	}

	{
		// Positions buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		info.m_Attachment = GBUFFER_POSITIONS;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA32F;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
		info.m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_8_8_8_8;
		RenderBuffersInfo.push_back(info);
	}

	{
		// Normals buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		info.m_Attachment = GBUFFER_NORMALS;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA32F;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
		info.m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_8_8_8_8;
		RenderBuffersInfo.push_back(info);
	}

	{
		// Depth + Stencil buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		//info.m_GenerateTexture = false;
		info.m_Attachment = GBUFFER_DEPTH_STENCIL;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_DEPTH24_STENCIL8;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_DEPTH_STENCIL;
		info.m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_24_8;
		RenderBuffersInfo.push_back(info);
	}

	{
		// Light accumulation buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		info.m_Attachment = GBUFFER_LIGHTACCUM;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA32F;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
		info.m_TextureType = graphics::TEXTURE_TYPE_FLOAT;
		RenderBuffersInfo.push_back(info);
	}

	m_GBuffer = graphics::FrameBuffer::Create(WindowWidth, WindowHeight, RenderBuffersInfo);
	// 	m_GBuffer->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// 	m_GBuffer->SetClearDepth(1.0f);
	// 	m_GBuffer->SetClearStencil(0);
	
	return (m_GBuffer != 0);
}

#undef GBUFFER_DIFFUSE_SPEC
#undef GBUFFER_POSITIONS
#undef GBUFFER_NORMALS
#undef GBUFFER_DEPTH_STENCIL
#undef GBUFFER_LIGHTACCUM
