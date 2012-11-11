#pragma once

#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "core/graphics/Texture.h"

namespace loki
{

namespace graphics
{

class Texture1D	: public Texture
{
public:
	~Texture1D();

	static Texture1D* Load( const std::string& _File );
	static Texture1D* Create();

	//////////////////////////////////////////////////////////////////////////
	// NOTE: OpenGL state is not guaranteed to be unchanged after a call to
	// this function.
	//////////////////////////////////////////////////////////////////////////
	void SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value );

	void UpdateGLInformation();

	int32 GetWidth() const;

	void UploadData( EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, const void* _Data );
	void UploadSubData( ETextureFormat _Format, ETextureType _Type, int32 _XOffset, int32 _Width, const void* _Data );
private:
	Texture1D();

	EInternalFormat m_InternalFormat;
	int32 m_Width;
};

}

}

#endif