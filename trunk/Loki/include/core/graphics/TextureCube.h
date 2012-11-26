#pragma once

#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H

#include "core/graphics/Texture.h"

namespace loki
{

namespace graphics
{

class TextureCube	: public Texture
{
public:
	~TextureCube();

	// Load a cubemap from a single image. The cubemap's faces are expected to be layed out consecutively in the image
	// in the width. This means that the total image width equals 6 * FaceWidth.
	// The order of the faces is: North, East, South, West, Up, Down (+Z, +X, -Z, -X, +Y, -Y).
	static TextureCube* Load( const std::string& _File, bool _InvertY = false );
	static TextureCube* Load( const std::string& _FileNorth,
							  const std::string& _FileEast,
							  const std::string& _FileSouth,
							  const std::string& _FileWest,
							  const std::string& _FileUp,
							  const std::string& _FileDown, bool _InvertY = false );
	static TextureCube* Create();

	//////////////////////////////////////////////////////////////////////////
	// NOTE: OpenGL state is not guaranteed to be unchanged after a call to
	// this function.
	//////////////////////////////////////////////////////////////////////////
	void SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value );

	void UpdateGLInformation();

	int32 GetWidth() const;
	int32 GetHeight() const;

	void UploadData( ECubeMapFace _Face, EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, int32 _Height, const void* _Data );
	void UploadSubData( ECubeMapFace _Face, ETextureFormat _Format, ETextureType _Type, int32 _XOffset, int32 _YOffset, int32 _Width, int32 _Height, const void* _Data );
private:
	TextureCube();

	// Exists to counter duplicate code.
	static TextureCube* _LoadHelper( int32 _SOIL_Load_Result, TextureCube* _Tex );

	EInternalFormat m_InternalFormat;
	int32 m_Width;
	int32 m_Height;
};

}

}

#endif