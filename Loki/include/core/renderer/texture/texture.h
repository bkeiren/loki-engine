#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/resourcemanager/resource/resource.h"
#include "core/resourcemanager/resourcemanager.h"
#include <GLEW\\glew.h>

namespace loki
{

namespace renderer
{

class LkImage;

class LkTexture	: public LkResource
{
	friend class ::loki::LkResourceManager<LkTexture*>;
	friend class ::loki::renderer::LkImage;
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
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};

	// This data is const because it may not be altered externally after the Texture object
	// is created. It is easier if it can just be accessed directly though, which is why it 
	// is public.
	const int m_Width;
	const int m_Height;
	const unsigned int m_OpenGLTextureID;
	
	//////////////////////////////////////////////////////////////////////////
	// NOTE: OpenGL state is not guaranteed to be unchanged after a call to
	// this function.
	//////////////////////////////////////////////////////////////////////////
	void SetTextureParameter( TextureParameter _Parameter, TextureParameterValue _Value );
private:
	LkTexture( const char* _Name, const unsigned int _OpenGLTextureID );
	LkTexture();
	~LkTexture();

	int m_OpenGLInternalFormat;
	int m_OpenGLTextureDepth;

};

}

}

#endif