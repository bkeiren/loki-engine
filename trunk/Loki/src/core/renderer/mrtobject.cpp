#define GL_GLEXT_PROTOTYPES		// Needs to be defined because otherwise functions such as glGenRenderbuffersEXT are not defined in glext.h

#include <windows.h>
#include <exception>
#include "core\logger.h"
#include "core\renderer\mrtobject.h"
#include "core\renderer\effect\effectmanager.h"

#include "core/renderer/renderer.h"	// For CheckGL().

using namespace loki::renderer;

// #define GL_DEPTH_STENCIL_ATTACHMENT		0x821A
// #define GL_DEPTH24_STENCIL8				0x88F0
// #define GL_UNSIGNED_INT_24_8			0x84FA

LkMRTObject::LkMRTObject( int _Width, int _Height )	:
	m_FBOWidth(_Width),
	m_FBOHeight(_Height),
	m_LightAccumulationToBackBufferEffect(NULL)
{
	// Ask OpenGL to create off-screen render buffers.
	glGenFramebuffers(1, &m_FBO);
	//m_FBO = 0;	// Uncomment to render to the screen instead of an alternative buffer. Can be helpful for testing.
	glGenRenderbuffers(1, &m_RT0);
	glGenRenderbuffers(1, &m_RT1);
	glGenRenderbuffers(1, &m_RT2);
	glGenRenderbuffers(1, &m_RTDepth);
	glGenRenderbuffers(1, &m_RTLightAccumulation);

	// Bind the frame buffer object (m_FBO) so that the next operations will affect it.
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	CheckGL();

	// Bind the diffuse color render target.
	glBindRenderbuffer(GL_RENDERBUFFER, m_RT0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, m_FBOWidth, m_FBOHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RT0);	// Indicate that the framebuffer's COLOR_ATTACHMENT0 attachment should be render target 0.
	CheckGL();

	// Bind the position render target.
	glBindRenderbuffer(GL_RENDERBUFFER, m_RT1);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F_ARB, m_FBOWidth, m_FBOHeight);		// Floating point.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, m_RT1);
	CheckGL();

	// Bind the normal render target.
	glBindRenderbuffer(GL_RENDERBUFFER, m_RT2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F_ARB, m_FBOWidth, m_FBOHeight);	// Floating point.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_RENDERBUFFER, m_RT2);
	CheckGL();

#define DEPTH_STENCIL_SHARED
#ifdef DEPTH_STENCIL_SHARED
	// Bind the depth buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, m_RTDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_FBOWidth, m_FBOHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RTDepth);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RTDepth);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RTDepth);
#else
	glBindRenderbuffer(GL_RENDERBUFFER, m_RTDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_FBOWidth, m_FBOHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RTDepth);
#endif
	CheckGL();

	// Bind the light accumulation render target.
	glBindRenderbuffer(GL_RENDERBUFFER, m_RTLightAccumulation);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F_ARB, m_FBOWidth, m_FBOHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_RENDERBUFFER, m_RTLightAccumulation);
	CheckGL();

	// Generate and bind the OpenGL texture for the diffuse color.
	glGenTextures(1, &m_RT0Texture);
	glBindTexture(GL_TEXTURE_2D, m_RT0Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_FBOWidth, m_FBOHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Attach the texture to the FBO.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RT0Texture, 0);
	CheckGL();

	// Generate and bind the OpenGL texture for the positions.
	glGenTextures(1, &m_RT1Texture);
	glBindTexture(GL_TEXTURE_2D, m_RT1Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_FBOWidth, m_FBOHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Attach the texture to the FBO.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_RT1Texture, 0);
	CheckGL();

	// Generate and bind the OpenGL texture for the normals.
	glGenTextures(1, &m_RT2Texture);
	glBindTexture(GL_TEXTURE_2D, m_RT2Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_FBOWidth, m_FBOHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Attach the texture to the FBO.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_RT2Texture, 0);
	CheckGL();

#ifdef DEPTH_STENCIL_SHARED
	// Generate and bind the OpenGL texture for the depth buffer.
	glGenTextures(1, &m_RTDepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_RTDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_FBOWidth, m_FBOHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	// Attach the texture to the FBO.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_RTDepthTexture, 0);	// 0x821A = GL_DEPTH_STENCIL_ATTACHMENT.
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_RTDepthTexture, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_RTDepthTexture, 0);
#else
	// Generate and bind the OpenGL texture for the depth buffer.
	glGenTextures(1, &m_RTDepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_RTDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_FBOWidth, m_FBOHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Attach the texture to the FBO.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_RTDepthTexture, 0);
#endif
	CheckGL();

	// Generate and bind the OpenGL texture for the light accumulation.
	glGenTextures(1, &m_RTLightAccumulationTexture);
	glBindTexture(GL_TEXTURE_2D, m_RTLightAccumulationTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_FBOWidth, m_FBOHeight, 0, GL_RGBA, GL_FLOAT/*GL_UNSIGNED_INT_8_8_8_8*/, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Attach the texture to the FBO.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_RTLightAccumulationTexture, 0);
	CheckGL();

	// Check if all went well and unbind the FBO.
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		{
			// Do nothing, just here for fun.
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{
			LOG(VL_ERROR, "MRTObject::MRTObject: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{
			LOG(VL_ERROR, "MRTObject::MRTObject: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{
			LOG(VL_ERROR, "MRTObject::MRTObject: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		{
			LOG(VL_ERROR, "MRTObject::MRTObject: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			break;
		}
	case GL_FRAMEBUFFER_UNSUPPORTED:
		{
			LOG(VL_ERROR, "MRTObject::MRTObject: GL_FRAMEBUFFER_UNSUPPORTED");
			throw new std::exception("GL_FRAMEBUFFER_UNSUPPORTED");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		{
			LOG(VL_ERROR, "MRTObject::MRTObject: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			break;
		}
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		{
			LOG(VL_ERROR, "MRTObject::MRTObject: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
			throw new std::exception("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
			break;
		}
	default:
		{
			LOG(VL_WARN, "MRTObject::MRTObject: Unhandled error (Framebuffer status is %i)", (int)status);
			break;
		}
	}

	// Unbind the frame buffer object from future operations.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_LightAccumulationToBackBufferEffect = g_EffectManager->CreateEffectFromMemory(
	#include "core/renderer/lightaccumtobackbuffer_cgeffect.inl"
	, "MRTObject");
}

LkMRTObject::~LkMRTObject()
{
	// Delete the textures and buffers that were allocated.
	glDeleteTextures(1, &m_RT0Texture);
	glDeleteTextures(1, &m_RT1Texture);
	glDeleteTextures(1, &m_RT2Texture);
	glDeleteTextures(1, &m_RTDepthTexture);
	glDeleteTextures(1, &m_RTLightAccumulationTexture);
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteRenderbuffers(1, &m_RT0);
	glDeleteRenderbuffers(1, &m_RT1);
	glDeleteRenderbuffers(1, &m_RT2);
	glDeleteRenderbuffers(1, &m_RTDepth);
	glDeleteRenderbuffers(1, &m_RTLightAccumulation);
}

void LkMRTObject::StartGBuffer()
{
	// Bind the FBO and set the viewport to the correct size.
	// At this point, the FBO should have a diffuse, position, normal and depth buffer attached to it
	// as render targets to which OpenGL can render.
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, m_FBOWidth, m_FBOHeight);

	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//glEnable(GL_TEXTURE_2D);

	// Specify render targets.
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, buffers);	// Tells OpenGL which buffers should be made available to render to.

	// Clear the render targets.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	glEnable(GL_DEPTH);
	glEnable(GL_STENCIL);
}

void LkMRTObject::StopGBuffer()
{
	// Stop using the buffers to render to and unbind the FBO.
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}

void LkMRTObject::StartLightAccumulation()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT3);

	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void LkMRTObject::RenderLightAccumulationToBackBuffer()
{
#define SETCGPARAM(paramname, value)	{LkEffectParameter* param = m_LightAccumulationToBackBufferEffect->GetParameterBySemantic(paramname);if(param){param->Set(value);}}

	SETCGPARAM("LKLIGHTACCUMULATIONTEX", m_RTLightAccumulationTexture);

	while (m_LightAccumulationToBackBufferEffect->HasNextPass())
	{
		// TODO: Remove immediate mode. Make a displaylist or something?
		glBegin(GL_QUADS);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glEnd();
	}
}

unsigned int LkMRTObject::GetRT0()
{
	return m_RT0Texture;
}

unsigned int LkMRTObject::GetRT1()
{
	return m_RT1Texture;
}

unsigned int LkMRTObject::GetRT2()
{
	return m_RT2Texture;
}

unsigned int LkMRTObject::GetRTDepthStencil()
{
	return m_RTDepthTexture;
}

unsigned int LkMRTObject::GetRTLightAccumulation()
{
	return m_RTLightAccumulationTexture;
}