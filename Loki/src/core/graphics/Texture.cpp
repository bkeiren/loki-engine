#include "core/graphics/Texture.h"
#include <GLEW\\glew.h>

namespace loki
{

namespace graphics
{

Texture::Texture()
{
	ILLEGAL_CTOR_ERROR("Texture")
}

Texture::Texture( ETextureTarget _TextureTarget )	:
	m_TextureTarget(_TextureTarget),
	m_GLTextureHandle(0),
	m_File(std::string(""))
{
	glGenTextures(1, &m_GLTextureHandle);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_GLTextureHandle);
}

ETextureTarget Texture::GetTextureTarget() const
{
	return m_TextureTarget;
}

bool Texture::Is1DTexture() const
{
	return GetTextureTarget() == TEXTURE_TARGET_1D;
}

bool Texture::Is2DTexture() const
{
	return GetTextureTarget() == TEXTURE_TARGET_2D;
}

bool Texture::Is3DTexture() const
{
	return GetTextureTarget() == TEXTURE_TARGET_3D;
}

bool Texture::IsCubeMapTexture() const
{
	return GetTextureTarget() == TEXTURE_TARGET_CUBE_MAP;
}

uint32 Texture::GetTextureHandle() const
{
	return m_GLTextureHandle;
}

void Texture::Bind() const
{
	glBindTexture(GLTextureTargets[m_TextureTarget], m_GLTextureHandle);
}

void Texture::Unbind() const
{
	glBindTexture(GLTextureTargets[m_TextureTarget], 0);
}

}

}