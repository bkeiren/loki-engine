#include "core/graphics/Texture.h"

namespace loki
{

namespace graphics
{

Texture::Texture()	:
	m_GLTextureHandle(0)
{

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

	Texture* tex = new Texture();
	tex->m_GLTextureHandle = id;
	tex->m_File = _File;

	return tex;
}

uint32 Texture::GetGLTextureHandle() const
{
	return m_GLTextureHandle;
}

}

}