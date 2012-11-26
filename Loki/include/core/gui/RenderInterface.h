#pragma once

#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

// We don't include the header file for Rocket::Core::RenderInterface here because
// it is included anyway in the GUI files were we use this class.
// Besides, you shouldn't have to include this file manually anyway.

namespace loki
{

namespace graphics
{

class DisplayList;
class Texture2D;
class Effect;

}

namespace gui
{

class RenderInterface	: public Rocket::Core::RenderInterface
{
	friend class GUI;
public:
	void RenderGeometry( Rocket::Core::Vertex* _Vertices, int _NumVertices, int* _Indices, int _NumIndices, Rocket::Core::TextureHandle _Texture, const Rocket::Core::Vector2f& _Translation );

	Rocket::Core::CompiledGeometryHandle CompileGeometry( Rocket::Core::Vertex* _Vertices, int _NumVertices, int* _Indices, int _NumIndices, Rocket::Core::TextureHandle _Texture );

	void RenderCompiledGeometry( Rocket::Core::CompiledGeometryHandle _Geometry, const Rocket::Core::Vector2f& _Translation );
	
	void ReleaseCompiledGeometry( Rocket::Core::CompiledGeometryHandle _Geometry );

	void EnableScissorRegion( bool _Enable );

	void SetScissorRegion( int _X, int _Y, int _Width, int _Height );

	bool LoadTexture( Rocket::Core::TextureHandle& _TextureHandle, Rocket::Core::Vector2i& _TextureDimensions, const Rocket::Core::String& _Source );
	
	bool GenerateTexture( Rocket::Core::TextureHandle& _TextureHandle, const byte* _Source, const Rocket::Core::Vector2i& _SourceDimensions );

	void ReleaseTexture( Rocket::Core::TextureHandle _Texture );
private:
	RenderInterface();
	~RenderInterface();

	struct GeometryData
	{
		graphics::DisplayList* m_DisplayList;
		graphics::Texture2D* m_Texture;
	};

	graphics::Effect* m_Effect;
	mat4 m_ProjectionMatrix;
};

}

}

#endif