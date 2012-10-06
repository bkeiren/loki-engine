#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/graphics/Enums.h"
#include <GLEW\\glew.h>

namespace loki
{

namespace graphics
{

class Texture
{
public:
	~Texture();

	static Texture* Load( const std::string& _File );
	static Texture* Create();

	uint32 GetTextureHandle() const;

	void Bind() const;
	void Unbind() const;

	//////////////////////////////////////////////////////////////////////////
	// NOTE: OpenGL state is not guaranteed to be unchanged after a call to
	// this function.
	//////////////////////////////////////////////////////////////////////////
	void SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value );

	void UpdateGLInformation();

	int32 GetWidth() const;
	int32 GetHeight() const;

	void UploadData( EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, int32 _Height, const void* _Data );
private:
	Texture();

	uint32 m_GLTextureHandle;
	std::string m_File;
	int32 m_OpenGLInternalFormat;
	int32 m_OpenGLTextureDepth;
	int32 m_Width;
	int32 m_Height;
};

}

}

#endif