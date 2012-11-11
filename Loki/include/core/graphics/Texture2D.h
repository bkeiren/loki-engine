#pragma once

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "core/graphics/Texture.h"

namespace loki
{

namespace graphics
{

class Texture2D	: public Texture
{
public:
	~Texture2D();

	static Texture2D* Load( const std::string& _File );
	static Texture2D* Create();

	//////////////////////////////////////////////////////////////////////////
	// NOTE: OpenGL state is not guaranteed to be unchanged after a call to
	// this function.
	//////////////////////////////////////////////////////////////////////////
	void SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value );

	void UpdateGLInformation();

	int32 GetWidth() const;
	int32 GetHeight() const;

	void UploadData( EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, int32 _Height, const void* _Data );
	void UploadSubData( ETextureFormat _Format, ETextureType _Type, int32 _XOffset, int32 _YOffset, int32 _Width, int32 _Height, const void* _Data );
private:
	Texture2D();

	EInternalFormat m_InternalFormat;
	int32 m_Width;
	int32 m_Height;
};

}

}

#endif