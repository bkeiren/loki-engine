#include "core/renderer/deferredrenderobject.h"
#include "core/renderer/mrtobject.h"
#include "core/renderer/renderer.h"

using namespace loki::renderer;

LkDeferredRenderObject::LkDeferredRenderObject( int _Width, int _Height, LkMRTObject* _MultipleRenderTargetObject )	:
	m_MRTObject(_MultipleRenderTargetObject),
	m_Width(_Width),
	m_Height(_Height),
	m_VisualizeRenderTargets(false)
{
	if (!m_Shader.AttachVertexShader("resources//shaders//deferredShading.vert"))
	{
		LOG(VL_ERROR, "Could not load deferred render vertex shader!");
	}
	if (!m_Shader.AttachFragmentShader("resources//shaders//deferredShading.frag"))
	{
		LOG(VL_ERROR, "Could not load deferred render fragment shader!");
	}
	m_Shader.AttachFragmentShader("resources//shaders//lensflare.frag");
	m_Shader.AttachFragmentShader("resources//shaders//ssao.frag");
	if (!m_Shader.LinkProgram())
	{
		LOG(VL_ERROR, "Could not link deferred render shader");
	}
	m_RT0_ID				= glGetUniformLocationARB(m_Shader.GetProgramHandle(), "tRT0");
	m_RT1_ID				= glGetUniformLocationARB(m_Shader.GetProgramHandle(), "tRT1");
	m_RT2_ID				= glGetUniformLocationARB(m_Shader.GetProgramHandle(), "tRT2");
	m_RTDepth_ID			= glGetUniformLocationARB(m_Shader.GetProgramHandle(), "tRTDepth");
	m_LightingID			= glGetUniformLocationARB(m_Shader.GetProgramHandle(), "tLighting");
	m_ScreenDimensionsID	= glGetUniformLocationARB(m_Shader.GetProgramHandle(), "ScreenDimensions");


	// Load debug shaders.
	m_DebugShaderDiffuse.AttachVertexShader("resources//shaders//deferredShading.vert");
	m_DebugShaderDiffuse.AttachFragmentShader("resources//shaders//deferred_dbg_image.frag");
	m_DebugShaderDiffuse.LinkProgram();

	m_DebugShaderPosition.AttachVertexShader("resources//shaders//deferredShading.vert");
	m_DebugShaderPosition.AttachFragmentShader("resources//shaders//deferred_dbg_position.frag");
	m_DebugShaderPosition.LinkProgram();
	
	m_DebugShaderNormal.AttachVertexShader("resources//shaders//deferredShading.vert");
	m_DebugShaderNormal.AttachFragmentShader("resources//shaders//deferred_dbg_normals.frag");
	m_DebugShaderNormal.LinkProgram();
	
	m_DebugShaderDepth.AttachVertexShader("resources//shaders//deferredShading.vert");
	m_DebugShaderDepth.AttachFragmentShader("resources//shaders//deferred_dbg_depth.frag");
	m_DebugShaderDepth.LinkProgram();

	m_DebugShaderLight.AttachVertexShader("resources//shaders//deferredShading.vert");
	m_DebugShaderLight.AttachFragmentShader("resources//shaders//deferred_dbg_light.frag");
	m_DebugShaderLight.LinkProgram();

	m_DebugDiffuseID	= glGetUniformLocationARB(m_DebugShaderDiffuse.GetProgramHandle(), "tDiffuse");
	m_DebugPositionID	= glGetUniformLocationARB(m_DebugShaderPosition.GetProgramHandle(), "tPosition");
	m_DebugNormalsID	= glGetUniformLocationARB(m_DebugShaderNormal.GetProgramHandle(), "tNormals");
	m_DebugDepthID		= glGetUniformLocationARB(m_DebugShaderDepth.GetProgramHandle(), "tDepth");
	m_DebugLightID		= glGetUniformLocationARB(m_DebugShaderLight.GetProgramHandle(), "tLight");
}

LkDeferredRenderObject::~LkDeferredRenderObject()
{
	
}

void LkDeferredRenderObject::Render()
{
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity ();
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);	// Disable perspective correction when sampling a texture because
														// it isn't needed now anyway (An orthographic projection is used).


	glUseProgramObjectARB(m_Shader.GetProgramHandle());

	{
		glEnable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE0_ARB);	// Set the active texture to TEXTURE0 (This is the texture that future
												// operations will affect.
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT0());	// Bind a texture ID to the active texture.
		glUniform1iARB(m_RT0_ID, 0);			// Pass the texture unit number to the shader. (First argument is the ID
												// of the uniform value in the shader).

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT1());
		glUniform1iARB(m_RT1_ID, 1);

		glActiveTextureARB(GL_TEXTURE2_ARB);
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT2());
		glUniform1iARB(m_RT2_ID, 2);

		glActiveTextureARB(GL_TEXTURE3_ARB);
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRTDepthStencil());
		glUniform1iARB(m_RTDepth_ID, 3);

		glActiveTextureARB(GL_TEXTURE4_ARB);
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRTLightAccumulation());
		glUniform1iARB(m_LightingID, 4);

		glUniform2iARB(m_ScreenDimensionsID, m_Width, m_Height);
	}

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

	{
		glActiveTextureARB(GL_TEXTURE4_ARB);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTextureARB(GL_TEXTURE3_ARB);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTextureARB(GL_TEXTURE2_ARB);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glDisable(GL_TEXTURE_2D);
	}

	glUseProgramObjectARB(0);


	if (m_VisualizeRenderTargets)
	{
		glUseProgramObjectARB(m_DebugShaderDiffuse.GetProgramHandle());
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT0());
		glUniform1iARB(m_DebugDiffuseID, 1);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.0f, 0.25f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.25f, 0.25f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.25f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glUseProgramObjectARB(0);

		// 		glUseProgramObjectARB(m_DebugShaderPosition.GetProgramHandle());
		// 		glActiveTextureARB(GL_TEXTURE0_ARB);
		// 		glEnable(GL_TEXTURE_2D);
		// 		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT1());
		// 		glUniform1iARB(m_DebugPositionID, 0);
		// 		glBegin(GL_QUADS);
		// 		glTexCoord2f(0.0f, 1.0f);
		// 		glVertex3f(0.5f, 1.0f, 0.0f);
		// 		glTexCoord2f(1.0f, 1.0f);
		// 		glVertex3f(1.0f, 1.0f, 0.0f);
		// 		glTexCoord2f(1.0f, 0.0f);
		// 		glVertex3f(1.0f, 0.5f, 0.0f);
		// 		glTexCoord2f(0.0f, 0.0f);
		// 		glVertex3f(0.5f, 0.5f, 0.0f);
		// 		glEnd();
		// 		glBindTexture(GL_TEXTURE_2D, 0);
		// 		glDisable(GL_TEXTURE_2D);
		// 		glUseProgramObjectARB(0);
		
		glUseProgramObjectARB(m_DebugShaderLight.GetProgramHandle());
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRTLightAccumulation());
		glUniform1iARB(m_DebugLightID, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.75f, 0.25f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 0.25f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.75f, 0.0f, 0.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glUseProgramObjectARB(0);

		glUseProgramObjectARB(m_DebugShaderNormal.GetProgramHandle());
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRT2());
		glUniform1iARB(m_DebugNormalsID, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.25f, 0.25f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5f, 0.25f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.25f, 0.0f, 0.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glUseProgramObjectARB(0);

		glUseProgramObjectARB(m_DebugShaderDepth.GetProgramHandle());
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_MRTObject->GetRTDepthStencil());
		glUniform1iARB(m_DebugDepthID, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.5f, 0.25f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.75f, 0.25f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.75f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.5f, 0.0f, 0.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glUseProgramObjectARB(0);
	}


	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();
}

void LkDeferredRenderObject::ToggleVisualizeRenderTargets()
{
	m_VisualizeRenderTargets = !m_VisualizeRenderTargets;
}