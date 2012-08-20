#include "core/renderer/renderbufferinfo.h"

namespace loki
{

namespace renderer
{

LkRenderBufferInfo::LkRenderBufferInfo()	:
	m_TexturePixels(0),
	//m_TextureHandle(0),
	m_GenerateTexture(true)
{
	m_InternalFormat = RBIF_RGBA;
	m_TextureFormat = RBF_RGBA;
	m_TextureType = RBT_UNSIGNED_INT_8_8_8_8;
	m_Attachment = RBA_COLOR_ATTACHMENT0;

	{
		// NOTE: If no mipmap levels are generated for the texture (Which is not done by default), then these parameters are required in order to
		// successfully sample the image in certain cases.
		AddTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		AddTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		AddTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		AddTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

LkRenderBufferInfo::~LkRenderBufferInfo()
{

}

void LkRenderBufferInfo::AddTextureParameter( GLenum _Enum, GLint _Value )
{
	m_TextureParametersInteger.push_back(std::pair<GLenum, GLint>(_Enum, _Value));	
}

void LkRenderBufferInfo::AddTextureParameter( GLenum _Enum, GLfloat _Value )
{
	m_TextureParametersFloat.push_back(std::pair<GLenum, GLfloat>(_Enum, _Value));
}

}

}