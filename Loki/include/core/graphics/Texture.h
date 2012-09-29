#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

namespace loki
{

namespace graphics
{

class Texture
{
public:
	~Texture();

	static Texture* Load( const std::string& _File );

private:
	Texture();

	uint32 m_GLTextureHandle;
	std::string m_File;
};

}

}

#endif