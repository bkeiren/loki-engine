#include <GLEW\\glew.h>
#include "core/renderer/texture/texture.h"

namespace loki
{

namespace renderer
{

LkTexture::LkTexture()	:
	m_Width(-1),
	m_Height(-1),
	m_OpenGLTextureID(0),
	m_OpenGLInternalFormat(0),
	m_OpenGLTextureDepth(0)
{
	ILLEGAL_CTOR_ERROR("Texture");
}

LkTexture::LkTexture( const char* _Name, const unsigned int _OpenGLTextureID )	:
	LkResource(_Name),
	m_Width(-1),
	m_Height(-1),
	m_OpenGLTextureID(_OpenGLTextureID),
	m_OpenGLInternalFormat(0),
	m_OpenGLTextureDepth(0)
{
	// Obtain texture information from OpenGL.
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, m_OpenGLTextureID);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, const_cast<int*>(&m_Width));
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, const_cast<int*>(&m_Height));
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &m_OpenGLInternalFormat);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH, &m_OpenGLTextureDepth);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameter(TEXTURE_WRAP_S, REPEAT);
	SetTextureParameter(TEXTURE_WRAP_T, REPEAT);
}

LkTexture::~LkTexture()
{

}

void LkTexture::SetTextureParameter( LkTexture::TextureParameter _Parameter, LkTexture::TextureParameterValue _Value )
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_OpenGLTextureID);
	glTexParameteri(GL_TEXTURE_2D, _Parameter, _Value);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}

}