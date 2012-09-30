#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLEW\\glew.h>

namespace loki
{

namespace graphics
{

class Texture
{
public:
	enum TextureParameter
	{
		TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S,
		TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T,
		TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER,
		TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER,
		TEXTURE_PRIORITY = GL_TEXTURE_PRIORITY,
		TEXTURE_BORDER_COLOR = GL_TEXTURE_BORDER_COLOR
	};

	enum TextureParameterValue
	{
		REPEAT = GL_REPEAT,
		CLAMP = GL_CLAMP,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};

	~Texture();

	static Texture* Load( const std::string& _File );
	static Texture* Create();

	uint32 GetGLTextureHandle() const;

	//////////////////////////////////////////////////////////////////////////
	// NOTE: OpenGL state is not guaranteed to be unchanged after a call to
	// this function.
	//////////////////////////////////////////////////////////////////////////
	void SetTextureParameter( TextureParameter _Parameter, TextureParameterValue _Value );

	void UpdateGLInformation();

	const int m_Width;
	const int m_Height;
private:
	Texture();
	Texture( uint32 _GLTextureHandle, const std::string& _Filename );


	uint32 m_GLTextureHandle;
	std::string m_File;
	const int m_OpenGLInternalFormat;
	const int m_OpenGLTextureDepth;
};

}

}

#endif