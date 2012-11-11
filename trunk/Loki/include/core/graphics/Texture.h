#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/graphics/Enums.h"

namespace loki
{

namespace graphics
{

class Texture
{
public:
	ETextureTarget GetTextureTarget() const;

	bool Is1DTexture() const;
	bool Is2DTexture() const;
	bool Is3DTexture() const;
	bool IsCubeMapTexture() const;

	uint32 GetTextureHandle() const;

	void Bind() const;
	void Unbind() const;
protected:
	Texture( ETextureTarget _TextureTarget );
	virtual ~Texture() = 0;

	uint32 m_GLTextureHandle;
	std::string m_File;
private:
	Texture();

	ETextureTarget m_TextureTarget;
};

}

}

#endif