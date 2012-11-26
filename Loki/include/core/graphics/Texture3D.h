#pragma once

#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "core/graphics/Texture.h"

namespace loki
{

namespace graphics
{

class Texture3D	: public Texture
{
public:
	~Texture3D();

	static Texture3D* Load( const std::string& _File, bool _InvertY = true );
	static Texture3D* Create();

	//////////////////////////////////////////////////////////////////////////
	// NOTE: OpenGL state is not guaranteed to be unchanged after a call to
	// this function.
	//////////////////////////////////////////////////////////////////////////
	void SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value );

	void UpdateGLInformation();

	int32 GetWidth() const;
	int32 GetHeight() const;
	int32 GetDepth() const;

	void UploadData( EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, int32 _Height, int32 _Depth, const void* _Data );
	void UploadSubData( ETextureFormat _Format, ETextureType _Type, int32 _XOffset, int32 _YOffset, int32 _ZOffset, int32 _Width, int32 _Height, int32 _Depth, const void* _Data );
private:
	Texture3D();

	EInternalFormat m_InternalFormat;
	int32 m_TextureDepth;
	int32 m_Width;
	int32 m_Height;
	int32 m_Depth;
};

}

}

#endif