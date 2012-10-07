#include <Windows.h>	// Required for wglGetProcAddress
#include <GLEW\\glew.h>
#include <GL\\glut.h>
//#include "core/renderer/geometry/model/model.h"
//#include "core/renderer/framebufferobject.h"
#include "core/renderer/deferredrenderobject.h"
#include "core/actor/camera/camera.h"
#include "core/renderer/renderer.h"
#include "core/renderer/scene/scene.h"
#include "core/renderer/GLSLShader/glslshader.h"
#include "core/actor/light/point/pointlight.h"
#include "core/actor/light/spot/spotlight.h"
#include "core/actor/light/directional/directionallight.h"
//#include "core/renderer/texture/texture.h"
#include "core/graphics/Texture.h"
#include "core/resourcemanager/texturemanager.h"
#include "core/resourcemanager/modelmanager.h"
#include "core/renderer/effect/effectmanager.h"
#include "core/renderer/debugrenderer.h"
#include "core/actor/components/rendercomponent/rendercomponent.h"
#include "core/renderer/effect/effectmanager.h"
#include "core/window.h"

#include "core/actor/handle/handle.h"

#include "core/game/level/level.h"
#include "core/game/level/skybox/skybox.h"

#include "core/actor/psystem/particlesystem.h"

#include "core/graphics/Model.h"

#include "core/graphics/FrameBuffer.h"

using namespace loki;
using namespace loki::renderer;
//using namespace loki::graphics;

#define ASSIMP_LOGFILE	"logs//log_assimp.txt"

namespace loki
{

namespace renderer
{

LkRenderer* g_Renderer = NULL;

}

}

LkRenderer::LkRenderer( LkWindow* _Window )	:
	m_CurrentLevelToRender(NULL),
	m_LightEffect(0),
	m_GBuffer(0),
#ifdef DBG_VISUALIZATIONS
	m_DBG_VisualizeGBufferTargets(false),
	m_DBG_VisualizeLightVolumes(false),
#endif
	m_Window(_Window)
{
	assert(_Window != 0);

	_Init(_Window);
}

LkRenderer::LkRenderer()
{
	ILLEGAL_CTOR_ERROR("Renderer");
}

LkRenderer::~LkRenderer()
{
	_Shutdown();
}

bool LkRenderer::_Init( LkWindow* _Window )
{
	_GetAPIInformation();
	_LogAPIInformation();

	// In order to avoid crashes (because certain OpenGL methods aren't fully defined), we need to explicitly bind certain
	// methods to their implementations.
	_BindGLFunctions();

	int WindowWidth = _Window->GetWidth();
	int WindowHeight = _Window->GetHeight();

	// Initialize the effect manager.
	// NOTE: Must be done before instantiating an object of class LkMRTObject because this class accesses g_EffectManager.
	g_EffectManager = new LkEffectManager();


	std::vector<graphics::FrameBuffer::FrameBufferAttachmentInfo> RenderBuffersInfo;
	{
		// Diffuse and specular color buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		info.m_Attachment = graphics::FRAMEBUFFER_COLOR_ATTACHMENT0;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
		info.m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_8_8_8_8;
		RenderBuffersInfo.push_back(info);
	}

	{
		// Positions buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		info.m_Attachment = graphics::FRAMEBUFFER_COLOR_ATTACHMENT1;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA32F;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
		info.m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_8_8_8_8;
		RenderBuffersInfo.push_back(info);
	}

	{
		// Normals buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		info.m_Attachment = graphics::FRAMEBUFFER_COLOR_ATTACHMENT2;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA32F;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
		info.m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_8_8_8_8;
		RenderBuffersInfo.push_back(info);
	}

	{
		// Depth + Stencil buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		//info.m_GenerateTexture = false;
		info.m_Attachment = graphics::FRAMEBUFFER_DEPTH_STENCIL_ATTACHMENT;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_DEPTH24_STENCIL8;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_DEPTH_STENCIL;
		info.m_TextureType = graphics::TEXTURE_TYPE_UNSIGNED_INT_24_8;
		RenderBuffersInfo.push_back(info);
	}

	{
		// Light accumulation buffer.
		graphics::FrameBuffer::FrameBufferAttachmentInfo info;
		info.m_Attachment = graphics::FRAMEBUFFER_COLOR_ATTACHMENT3;
		info.m_InternalFormat = graphics::INTERNAL_FORMAT_RGBA32F;
		info.m_TextureFormat = graphics::TEXTURE_FORMAT_RGBA;
		info.m_TextureType = graphics::TEXTURE_TYPE_FLOAT;
		RenderBuffersInfo.push_back(info);
	}

	//m_GBuffer = new LkFramebufferObject(WindowWidth, WindowHeight, RenderBuffersInfo);
	m_GBuffer = graphics::FrameBuffer::Create(WindowWidth, WindowHeight, RenderBuffersInfo);
// 	m_GBuffer->SetClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
// 	m_GBuffer->SetClearDepth(1.0f);
// 	m_GBuffer->SetClearStencil(0);
//	if (!m_GBuffer->CheckFramebufferStatus())
	if (!m_GBuffer)
	{
		LOG(VL_ERROR, "Renderer::Init: G-Buffer creation failed");
		//delete m_GBuffer;
		m_GBuffer = NULL;
	}


	// Initialize the texture manager.
	//g_TextureManager = new LkTextureManager();

	// Initialize the model manager.
	//g_ModelManager = new LkModelManager();


	m_LightEffect = g_EffectManager->CreateEffectFromFile("resources//shaders//light.cgfx", "LightEffect");
	if (!m_LightEffect)
	{
		LOG(VL_ERROR, "Renderer::Init: Failed to load light effect");
	}

	m_LightAccumulationToBackBufferEffect = g_EffectManager->CreateEffectFromMemory(
	#include "core/renderer/lightaccumtobackbuffer_cgeffect.inl"
	, "LightAccumulationToBackBuffer");

#ifdef DBG_VISUALIZATIONS
	m_GBufferTargets_General = g_EffectManager->CreateEffectFromMemory(
	#include "core/renderer/gbuffertargets_general_cgeffect.inl"
		, "GBufferTargets_General");

	m_GBufferTargets_Normals = g_EffectManager->CreateEffectFromMemory(
	#include "core/renderer/gbuffertargets_normals_cgeffect.inl"
		, "GBufferTargets_Normals");

	m_GBufferTargets_Depth = g_EffectManager->CreateEffectFromMemory(
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


	LOG(VL_ALWAYS, "Renderer::Init: Renderer initialized");
	return true;
}

void LkRenderer::_Shutdown()
{
	// Shutdown AssImp.
	aiDetachAllLogStreams();

// 	delete g_ModelManager;
// 	g_ModelManager = NULL;

// 	delete g_TextureManager;
// 	g_TextureManager = NULL;

	delete g_EffectManager;
	g_EffectManager = NULL;

	delete m_GBuffer;
	m_GBuffer = NULL;
	
	LOG(VL_ALWAYS, "Renderer::Shutdown: Renderer terminated");
}

void LkRenderer::_GetAPIInformation()
{
	m_OpenGL_Vendor = std::string((const char*)glGetString(GL_VENDOR));
	m_OpenGL_Renderer = std::string((const char*)glGetString(GL_RENDERER));
	m_OpenGL_Version = std::string((const char*)glGetString(GL_VERSION));
	std::string extensions = std::string((const char*)glGetString(GL_EXTENSIONS));
	
	while (extensions.length() > 0)
	{
		int next_space = extensions.find((char)32);
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
void LkRenderer::Render( game::LkLevel* _Level )
{
	m_CurrentLevelToRender = _Level;

	m_CurrentLevelToRender->GetCurrentCamera()->ApplyViewport();
	//m_CurrentLevelToRender->GetCurrentCamera()->ApplyProjectionMatrix();
	//m_CurrentLevelToRender->GetCurrentCamera()->ApplyViewTransformation();

	/*
	m_MRTObject->StartGBuffer();
	_RenderSky();

	glPushMatrix();

	{
		_RenderOpaqueGeometry();
		debug::DrawItems(m_CurrentLevelToRender->GetCurrentCamera()->GetViewMatrix());

		m_MRTObject->StartLightAccumulation();
		_RenderLighting();
		

		m_MRTObject->StopGBuffer();
		m_DRObject->Render();

		_RenderTransparentGeometry();
	}

	glPopMatrix();

	//RenderPreUIPostProcess();	// Handled through m_DRObject's shader.

	{
		_RenderUIObjects();
		_RenderUI();
	}

	//RenderPostUIPostProcess();

	*/

	//m_MRTObject->StartGBuffer();
	static graphics::EFrameBufferAttachment DrawBuffersP0[] = { graphics::FRAMEBUFFER_COLOR_ATTACHMENT3 };
	static graphics::EFrameBufferAttachment DrawBuffersP1[] = { graphics::FRAMEBUFFER_COLOR_ATTACHMENT0, graphics::FRAMEBUFFER_COLOR_ATTACHMENT1, graphics::FRAMEBUFFER_COLOR_ATTACHMENT2, graphics::FRAMEBUFFER_COLOR_ATTACHMENT3 }; 

	m_GBuffer->Bind();
	m_GBuffer->SetDrawBuffers(DrawBuffersP1, 4);
	//m_GBuffer->ClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_GBuffer->SetDrawBuffers(DrawBuffersP0, 1);
	_RenderSky();

	m_GBuffer->SetDrawBuffers(DrawBuffersP1, 4);
	glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	
		_RenderOpaqueGeometry();

		_RenderParticles();

		// Draw debug stuff.
		debug::DrawItems(m_CurrentLevelToRender->GetCurrentCamera()->GetProjectionMatrix(), m_CurrentLevelToRender->GetCurrentCamera()->GetViewMatrix());

		//m_MRTObject->StartLightAccumulation();
		m_GBuffer->SetDrawBuffer(graphics::FRAMEBUFFER_COLOR_ATTACHMENT3);
		
		_RenderLighting();

	//m_MRTObject->StopGBuffer();
	m_GBuffer->Unbind();

	//m_MRTObject->RenderLightAccumulationToBackBuffer();
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
			LOG(VL_ERROR, "CheckGLError: Unhandled error (%i)", (int)error);
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

void LkRenderer::DrawPixels( int _Width, int _Height, graphics::EInternalFormat _Format, const void* _Buffer )
{
	glDrawPixels(_Width, _Height, graphics::GLInternalFormats[_Format], GL_UNSIGNED_BYTE, _Buffer);
}

int LkRenderer::GetWindowWidth()
{
	return m_Window->GetWidth();
}

int LkRenderer::GetWindowHeight()
{
	return m_Window->GetHeight();
}

int LkRenderer::GetRenderWidth()
{
	return m_GBuffer->GetWidth();
}

int LkRenderer::GetRenderHeight()
{
	return m_GBuffer->GetHeight();
}

vec2 LkRenderer::GetPixelScale()
{
	return vec2((float)renderer::g_Renderer->GetRenderWidth() / renderer::g_Renderer->GetWindowWidth(),
					 (float)renderer::g_Renderer->GetRenderHeight() / renderer::g_Renderer->GetWindowHeight());
}

void LkRenderer::_RenderSky()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)&(m_CurrentLevelToRender->GetCurrentCamera()->GetProjectionMatrix()));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	mat4 m = m_CurrentLevelToRender->GetCurrentCamera()->GetViewMatrix();
	m[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glLoadMatrixf(math::value_ptr(m));

	glDisable(GL_CULL_FACE);
	glDepthMask( GL_FALSE );  // Don't write to the depth buffer
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	const game::LkSkyBox* skybox = m_CurrentLevelToRender->GetSkyBox();
	//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE ); // Don't do any blending on the cube map textures

	{
		glBindTexture( GL_TEXTURE_2D, skybox->GetTexture(game::SBS_WEST)->GetTextureHandle());
		glBegin( GL_QUADS );
		// +X
		glTexCoord2f(  0.0f, 0.0f );
		glVertex3f(    1.0f, -1.0f, -1.0f );

		glTexCoord2f(  0.0f,  1.0f );
		glVertex3f(    1.0f,  1.0f, -1.0f );

		glTexCoord2f(  1.0f,  1.0f );
		glVertex3f(    1.0f,  1.0f,  1.0f );

		glTexCoord2f(  1.0f, 0.0f );
		glVertex3f(    1.0f, -1.0f,  1.0f );

		glEnd();
		
		glBindTexture( GL_TEXTURE_2D, skybox->GetTexture(game::SBS_EAST)->GetTextureHandle() );
		glBegin( GL_QUADS );
		// -X
		glTexCoord2f(  1.0f, 0.0f );
		glVertex3f(   -1.0f, -1.0f, -1.0f );

		glTexCoord2f(  1.0f,  1.0f );
		glVertex3f(   -1.0f,  1.0f, -1.0f );

		glTexCoord2f(  0.0f,  1.0f );
		glVertex3f(   -1.0f,  1.0f,  1.0f );

		glTexCoord2f(  0.0f, 0.0f );
		glVertex3f(   -1.0f, -1.0f,  1.0f );

		glEnd();
		
		glBindTexture( GL_TEXTURE_2D, skybox->GetTexture(game::SBS_UP)->GetTextureHandle() );
		glBegin( GL_QUADS );
		// +Y
		glTexCoord2f(  0.0f, 0.0f );
		glVertex3f(   -1.0f,  1.0f, -1.0f );

		glTexCoord2f(  1.0f, 0.0f );
		glVertex3f(    1.0f,  1.0f, -1.0f );

		glTexCoord2f(  1.0f,  1.0f );
		glVertex3f(    1.0f,  1.0f,  1.0f );

		glTexCoord2f(  0.0f,  1.0f );
		glVertex3f(   -1.0f, 1.0f,  1.0f );

		glEnd();
		
		glBindTexture( GL_TEXTURE_2D, skybox->GetTexture(game::SBS_DOWN)->GetTextureHandle() );
		glBegin( GL_QUADS );
		// -Y
		glTexCoord2f(  0.0f,  1.0f );
		glVertex3f(   -1.0f, -1.0f, -1.0f );

		glTexCoord2f(  1.0f,  1.0f );
		glVertex3f(    1.0f, -1.0f, -1.0f );

		glTexCoord2f(  1.0f, 0.0f );
		glVertex3f(    1.0f, -1.0f,  1.0f );

		glTexCoord2f(  0.0f, 0.0f );
		glVertex3f(   -1.0f, -1.0f,  1.0f );

		glEnd();
		
		glBindTexture( GL_TEXTURE_2D, skybox->GetTexture(game::SBS_NORTH)->GetTextureHandle() );
		glBegin( GL_QUADS );
		// +Z
		glTexCoord2f(  1.0f, 0.0f );
		glVertex3f(   -1.0f, -1.0f,  1.0f );

		glTexCoord2f(  0.0f, 0.0f );
		glVertex3f(    1.0f, -1.0f,  1.0f );

		glTexCoord2f(  0.0f,  1.0f );
		glVertex3f(    1.0f,  1.0f,  1.0f );

		glTexCoord2f(  1.0f,  1.0f );
		glVertex3f(   -1.0f,  1.0f,  1.0f );

		glEnd();
		
		glBindTexture( GL_TEXTURE_2D, skybox->GetTexture(game::SBS_SOUTH)->GetTextureHandle() );
		glBegin( GL_QUADS );
		// -Z
		glTexCoord2f(  0.0f, 0.0f );
		glVertex3f(   -1.0f, -1.0f, -1.0f );

		glTexCoord2f(  1.0f, 0.0f );
		glVertex3f(    1.0f, -1.0f, -1.0f );

		glTexCoord2f(  1.0f,  1.0f );
		glVertex3f(    1.0f,  1.0f, -1.0f );

		glTexCoord2f(  0.0f,  1.0f );
		glVertex3f(   -1.0f,  1.0f, -1.0f );

		glEnd();
	}

	glBindTexture( GL_TEXTURE_2D, 0 );
	glDepthMask( GL_TRUE );
	glEnable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void LkRenderer::_RenderOpaqueGeometry()
{
	//glCullFace(GL_BACK);	// Handled by Cg.
	glDepthFunc( GL_LEQUAL );
	glColorMask(true, true, true, true);
	glDepthMask(true);

	mat4 viewmatrix = m_CurrentLevelToRender->GetCurrentCamera()->GetViewMatrix();
	mat4 projectionmatrix = m_CurrentLevelToRender->GetCurrentCamera()->GetProjectionMatrix();

// 	for (std::list<LkRenderComponent*>::iterator it = LkRenderComponent::m_RenderComponents.begin(); it != LkRenderComponent::m_RenderComponents.end(); ++it)
// 	{
// 		LkMovableComponent* movcomp = (*it)->GetActor()->GetComponent<LkMovableComponent>();
// 
// 		if ((*it)->m_Model)	// TODO: Replace this check with a default model to indicate missing models.
// 		{
// 			mat4 t = (movcomp)?(movcomp->GetTransformation()):(mat4(1.0f, 0.0f, 0.0f, 0.0f, 
// 																			  0.0f, 1.0f, 0.0f, 0.0f, 
// 																			  0.0f, 0.0f, 1.0f, 0.0f,
// 																			  0.0f, 0.0f, 0.0f, 1.0f));
// 			(*it)->m_Model->_Render(t, viewmatrix, projectionmatrix, m_CurrentLevelToRender->GetCurrentCamera()->GetZFar(), m_CurrentLevelToRender->GetCurrentCamera()->GetZNear());
// 		}
// 	}

	static graphics::Model* mdl = graphics::Model::Load("resources//lmo//test.lmo");
	mdl->Render(mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), viewmatrix, projectionmatrix, m_CurrentLevelToRender->GetCurrentCamera()->GetZFar(), m_CurrentLevelToRender->GetCurrentCamera()->GetZNear());
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

	_RenderLightingPointLights();
	_RenderLightingDirectionalLights();
	_RenderLightingSpotLights();

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
	const game::LkLevel::ParticleSystems* systems = m_CurrentLevelToRender->GetParticleSystems();
	for (game::LkLevel::ParticleSystems::const_iterator it = systems->begin(); it != systems->end(); ++it)
	{
		(*it).second->Render(m_CurrentLevelToRender->GetCurrentCamera()->GetViewMatrix(), m_CurrentLevelToRender->GetCurrentCamera()->GetProjectionMatrix());
	}
}

void LkRenderer::_RenderLightingPointLights()
{
#define SETCGPARAM(paramname, value)	{LkEffectParameter* param = m_LightEffect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

	mat4 _ProjectionMatrix = m_CurrentLevelToRender->GetCurrentCamera()->GetProjectionMatrix();
	mat4 _ViewMatrix = m_CurrentLevelToRender->GetCurrentCamera()->GetViewMatrix();
	mat4 _ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	vec3 _EyePosition = vec3(math::inverse(_ViewMatrix)[3]);

	SETCGPARAM("LKEYEPOSITION", _EyePosition);			// Set the eye position.
	SETCGPARAM("LKRT0", m_GBuffer->GetAttachmentTexture(graphics::FRAMEBUFFER_COLOR_ATTACHMENT0));
	SETCGPARAM("LKRT1", m_GBuffer->GetAttachmentTexture(graphics::FRAMEBUFFER_COLOR_ATTACHMENT1));
	SETCGPARAM("LKRT2", m_GBuffer->GetAttachmentTexture(graphics::FRAMEBUFFER_COLOR_ATTACHMENT2));
	//SETCGPARAM("LKRT3", m_GBuffer->GetRenderbufferTexture(3));

	for (game::LkLevel::PointLights::const_iterator it = m_CurrentLevelToRender->GetPointLights()->begin(); it != m_CurrentLevelToRender->GetPointLights()->end(); ++it)
	{
		const LkPointLight* pointlight = (*it).second;

		if (!pointlight->IsEnabled())
		{
			continue;
		}

		LkMovableComponent* comp = pointlight->GetComponent<LkMovableComponent>();
		mat4 _ModelMatrix = comp->GetTransformation();
		bool CameraInsideVolume = math::length(_EyePosition - comp->GetPosition()) < pointlight->GetRadius();
		_ModelMatrix = math::gtc::matrix_transform::rotate(_ModelMatrix, 90.0f, vec3(1.0f, 0.0f, 0.0f));
		SETCGPARAM("LKMODELVIEWPROJ", _ViewProjectionMatrix * _ModelMatrix);		// Set the model view projection matrix.
		SETCGPARAM("LKMODELMATRIX", _ModelMatrix);			// Set the model matrix.
		SETCGPARAM("LKMODELMATRIXIT", mat3(math::transpose(math::inverse(_ModelMatrix))));		// Set the inverse transpose of the model matrix.	
		SETCGPARAM("LKVIEWMATRIX", _ViewMatrix);
		SETCGPARAM("LKLIGHTPOSITION", comp->GetPosition());
		SETCGPARAM("LKLIGHTRADIUS", pointlight->GetRadius());
		SETCGPARAM("LKLIGHTCOLOR", pointlight->GetColor());
		SETCGPARAM("LKLIGHTSPECULAR", pointlight->GetSpecular());
		SETCGPARAM("LKLIGHTCONSTANTATTENUATION", pointlight->GetConstantAttenuation());
		SETCGPARAM("LKLIGHTLINEARATTENUATION", pointlight->GetLinearAttenuation());
		SETCGPARAM("LKLIGHTQUADRATICATTENUATION", pointlight->GetQuadraticAttenuation());

		// TODO: Replace this with a VBO or something. Atleast not emmediate mode.
		static GLUquadric* quadric = gluNewQuadric();
		int PassID = 0;
		while (m_LightEffect->HasNextPass())
		{
#ifndef DBG_VISUALIZATIONS
			if (PassID == 4)
			{
				continue;
			}
#else
			if (!m_DBG_VisualizeLightVolumes && PassID == 4)
			{
				continue;				
			}
#endif
			
			// Awesomely condensed code...
			(CameraInsideVolume && (PassID == 1 || PassID == 3) || ((!CameraInsideVolume) && PassID == 2))?(0):(gluSphere(quadric, pointlight->GetRadius(), 20, 15));
			++PassID;
		}

		//glClear(GL_STENCIL_BUFFER_BIT);
	}
}

void LkRenderer::_RenderLightingDirectionalLights()
{
	/*
	glColorMask(true, true, true, true);
	glDepthMask(false);
	glDisable(GL_STENCIL_TEST);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	
	glUseProgramObjectARB(m_DirectionalLightShader.GetProgramHandle());

	glUniform2fARB(m_DirectionalLightShaderScreenDimensionsID, (float)m_WindowWidth, (float)m_WindowHeight);
	// Set the input textures for the shader.
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT0());
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT1());
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT2());
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRTDepthStencil());
	glUniform1iARB(m_DirectionalLightShaderRT0ID, 0);
	glUniform1iARB(m_DirectionalLightShaderRT1ID, 1);
	glUniform1iARB(m_DirectionalLightShaderRT2ID, 2);
	glUniform1iARB(m_DirectionalLightShaderRTDepthID, 3);

	for (game::LkLevel::DirectionalLights::const_iterator it = m_CurrentLevelToRender->GetDirectionalLights()->begin(); it != m_CurrentLevelToRender->GetDirectionalLights()->end(); ++it)
	{
		const LkDirectionalLight* directionallight1 = (*it).second;

		if (!directionallight1->IsEnabled())
		{
			continue;
		}

		// Pass the light color to the shader.
		Color color = directionallight1->GetColor();
		glUniform3fARB(m_DirectionalLightShaderColorID, color.r, color.g, color.b);

		// Pass the light direction to the shader.
		vec3 direction = directionallight1->GetDirection();
		glUniform3fARB(m_DirectionalLightShaderDirectionID, direction.x, direction.y, direction.z);

		// Render a full-screen quad.
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
	}

	// Unbind all bound shader textures.
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgramObjectARB(0);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();
	*/
}

void LkRenderer::_RenderLightingSpotLights()
{

}

void LkRenderer::_RenderLightAccumulationToBackBuffer()
{
#ifdef SETCGPARAM
#undef SETCGPARAM
#endif

#define SETCGPARAM(paramname, value)	{LkEffectParameter* param = m_LightAccumulationToBackBufferEffect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

	//SETCGPARAM("LKLIGHTACCUMULATIONTEX", m_GBuffer->GetRenderbufferTexture(4));
	SETCGPARAM("LKLIGHTACCUMULATIONTEX", m_GBuffer->GetAttachmentTexture(graphics::FRAMEBUFFER_COLOR_ATTACHMENT3));

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

#define SETCGPARAM(paramname, value)	{LkEffectParameter* param = effect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

	for (int i = 0; i < 4; ++i)
	{
		float x = -1.0f + (0.5f * i);

		static graphics::EFrameBufferAttachment Attachments[4] = { graphics::FRAMEBUFFER_COLOR_ATTACHMENT0, graphics::FRAMEBUFFER_COLOR_ATTACHMENT1, graphics::FRAMEBUFFER_COLOR_ATTACHMENT2, graphics::FRAMEBUFFER_DEPTH_STENCIL_ATTACHMENT };

		LkEffect* effect = (i == 2)?(m_GBufferTargets_Normals):((i == 3)?(m_GBufferTargets_Depth):(m_GBufferTargets_General));
		SETCGPARAM("LKGBUFFERTEX", m_GBuffer->GetAttachmentTexture(Attachments[i]));

		if (i == 3)
		{
			SETCGPARAM("LKZFAR", m_CurrentLevelToRender->GetCurrentCamera()->GetZFar());
			SETCGPARAM("LKZNEAR", m_CurrentLevelToRender->GetCurrentCamera()->GetZNear());
		}

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