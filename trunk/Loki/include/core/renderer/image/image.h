#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "core/renderer/texture/texture.h"
#include "core/renderer/enums.h"

#define USE_PBO

namespace loki
{

namespace renderer
{

enum EAnchorPoint
{
	AP_TOPLEFT = 0,
	AP_TOPMIDDLE,
	AP_TOPRIGHT,
	AP_MIDDLELEFT,
	AP_CENTER,
	AP_MIDDLERIGHT,
	AP_BOTTOMLEFT,
	AP_BOTTOMMIDDLE,
	AP_BOTTOMRIGHT
};

//class Texture;
class LkEffect;

#ifdef USE_PBO
class LkPixelBufferObject;
#endif

class LkImage
{
	typedef std::vector<std::pair<bool, LkTexture*> >		Textures;
	typedef Textures::iterator								TexturesIter;
	typedef Textures::const_iterator						TexturesConstIter;
public:
	LkImage( const char* _Texture, const glm::vec3& _Position, const glm::vec2& _Size, bool _PositionIsAbsolute = false, bool _SizeIsAbsolute = false );
	LkImage( const char* _Texture, const glm::vec2& _Position, const glm::vec2& _Size, bool _PositionIsAbsolute = false, bool _SizeIsAbsolute = false );
	virtual ~LkImage();

	bool AddTexture( const char* _Texture );

	//////////////////////////////////////////////////////////////////////////
	// Adds a texture from data in memory.
	//////////////////////////////////////////////////////////////////////////
	bool AddTextureFromMemory( const void* _Data, unsigned int _Width, unsigned int _Height, EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type );

	//////////////////////////////////////////////////////////////////////////
	// Modifies a portion of the current texture by uploading new data for it.
	// _Data points to the start of the new data, _XOffset and _YOffset
	// indicate the offset from [0, 0] of the base texture.
	// _Width and _Height indicate the height of the sub image.
	// Example:
	// A base texture with a size of [1024, 512] in which the rectangle
	// from [64, 64] to [128, 256] must be re-uploaded by taking that 
	// rectangle area from a second texture that has a size of [1024, 512]:
	//
	// void* data = (char*)(SecondBuffer) + ((64 + 64 * 1024) * 4);	// address + ((x + y * img_width) * 4)
	// SetSubTextureFromMemory(data, 64, 64, 64, 192, format, type);
	//
	// WARNING: Potentially VERY slow.
	//////////////////////////////////////////////////////////////////////////
	void SetSubTextureFromMemory( const void* _Data, int _XOffset, int _YOffset, unsigned int _Width, unsigned int _Height, ETextureFormat _Format, ETextureType _Type );
	
	void SetTextureIndex( unsigned int _Index );
	unsigned int GetTextureIndex() const;
	unsigned int GetNumTextureIndices() const;

	void SetFlipX( bool _Flip );
	void SetFlipY( bool _Flip );
	bool IsFlippedX() const;
	bool IsFlippedY() const;

	void Render();

	glm::vec3 GetAbsolutePosition() const;
	const glm::vec3& GetRelativePosition() const;
	glm::vec2 GetAbsoluteSize() const;
	const glm::vec2& GetRelativeSize() const;
	glm::vec2 GetAbsoluteAnchorOffset() const;
	const glm::vec2& GetRelativeAnchorOffset() const;

	bool Is3D() const;
	void Set3D( bool _State );

	void SetAbsolutePosition( const glm::vec3& _Position );	// Z-coordinate is ignored.
	void SetAbsolutePosition( const glm::vec2& _Position );
	void SetRelativePosition( const glm::vec3& _Position );
	void SetRelativePosition( const glm::vec2& _Position );
	void SetAbsoluteSize( const glm::vec2& _Size );
	void SetRelativeSize( const glm::vec2& _Size );
	//void SetTexture( const char* _Texture );
	void SetAnchorPoint( EAnchorPoint _AnchorPoint );

	int GetWidth() const;
	int GetHeight() const;

	glm::mat4 GetModelMatrix() const;
	glm::mat4 Get3DModelViewProjectionMatrix() const;			// Projection * View * Model
	glm::mat4 Get3DModelViewProjectionMatrixInverse() const;	// inverse(Model) * inverse(View) * inverse(Projection)
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;
	const glm::quat& GetOrientation() const;

	void SetOrientation( const glm::quat& _Orientation );

	// Utility function if a value is required that represents the full
	// width of the screen (For example, when setting the size of an image).
	// For convenience, a macro ('FULL_WINDOW_WIDTH') is provided that simply calls this function, so it can be used as an argument or value.
#define FULL_WINDOW_WIDTH	(loki::renderer::LkImage::GetWindowWidthValue())
	static float GetWindowWidthValue();
protected:
	//////////////////////////////////////////////////////////////////////////
	// These two members are just here to convenience the implementation of
	// the AnimatedImage class.
	//////////////////////////////////////////////////////////////////////////
	glm::vec2 m_UVTopLeft;
	glm::vec2 m_UVBottomRight;

private:
	LkImage();

	// Implemented to avoid duplicate code.
	void _Init( const char* _Texture, const glm::vec3& _Position, const glm::vec2& _Size, bool _PositionIsAbsolute = false, bool _SizeIsAbsolute = false );

	Textures m_Textures;
	unsigned int m_TextureIndex;

	glm::vec3 m_Position;
	glm::quat m_Orientation;
	glm::vec2 m_Size;
	bool m_Is3D;	// Whether the image is to be drawn in 3D.
	EAnchorPoint m_AnchorPoint;	// Where the image's [0, 0] coordinate is located 
						// (Top left, top middle, top right, 
						//  left middle, center, right middle, 
						//  bottom left, bottom middle, bottom right).
	glm::vec2 m_AnchorPointVector;
	bool m_FlipX;
	bool m_FlipY;

#ifdef USE_PBO
	LkPixelBufferObject* m_PBO;
#endif

	static LkEffect* m_CgEffect;
};

}

}

#endif