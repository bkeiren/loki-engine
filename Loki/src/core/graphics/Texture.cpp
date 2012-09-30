#include "core/graphics/Texture.h"
#include "SOIL/SOIL.h"

namespace loki
{

namespace graphics
{

Texture::Texture()	:
	m_GLTextureHandle(0),
	m_Width(1),
	m_Height(1),
	m_OpenGLInternalFormat(0),
	m_OpenGLTextureDepth(0)
{
	ILLEGAL_CTOR_ERROR("Texture");
}

Texture::Texture( uint32 _GLTextureHandle, const std::string& _Filename )	:
	m_GLTextureHandle(_GLTextureHandle),
	m_File(_Filename),
	m_Width(1),
	m_Height(1),
	m_OpenGLInternalFormat(0),
	m_OpenGLTextureDepth(0)
{
	UpdateGLInformation();
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_GLTextureHandle);
}

Texture* Texture::Load( const std::string& _File )
{
	uint32 id = SOIL_load_OGL_texture(_File.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	if (id == 0)
	{
		LOG(VL_ERROR, "Texture::Load: Failed to load texture '%s'", _File.c_str());
		return 0;
	}

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture* tex = new Texture(id, _File);
	return tex;
}

Texture* Texture::Create()
{
	uint32 id = 0;
	glGenTextures(1, &id);

	Texture* tex = new Texture(id, "");
	return tex;
}

uint32 Texture::GetGLTextureHandle() const
{
	return m_GLTextureHandle;
}

void Texture::SetTextureParameter( TextureParameter _Parameter, TextureParameterValue _Value )
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GLTextureHandle);
	glTexParameteri(GL_TEXTURE_2D, _Parameter, _Value);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UpdateGLInformation()
{
	// Obtain texture information from OpenGL.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GLTextureHandle);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, const_cast<int*>(&m_Width));
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, const_cast<int*>(&m_Height));
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, const_cast<int*>(&m_OpenGLInternalFormat));
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH, const_cast<int*>(&m_OpenGLTextureDepth));
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameter(TEXTURE_WRAP_S, REPEAT);
	SetTextureParameter(TEXTURE_WRAP_T, REPEAT);
}

}

}