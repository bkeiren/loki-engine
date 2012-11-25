#include <Rocket/Core/renderinterface.h>
#include "core/gui/RenderInterface.h"
#include <GLEW\\glew.h>
//#include <GL\\glut.h>
#include "core/graphics/DisplayList.h"
#include "core/graphics/Texture2D.h"
#include "core/renderer/renderer.h"

namespace loki
{

namespace gui
{

namespace
{
	void SetupMatrices()
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0f, (float)renderer::g_Renderer->GetRenderWidth(), (float)renderer::g_Renderer->GetRenderHeight(), 0.0f, -1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}

	void ClearMatrices()
	{
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
}

RenderInterface::RenderInterface()
{

}

RenderInterface::~RenderInterface()
{

}

void RenderInterface::RenderGeometry( Rocket::Core::Vertex* _Vertices, int _NumVertices, int* _Indices, int _NumIndices, Rocket::Core::TextureHandle _Texture, const Rocket::Core::Vector2f& _Translation )
{
	SetupMatrices();
	
	glTranslatef(_Translation.x, _Translation.y, 0.0f);
	
	graphics::Texture2D* tex = (graphics::Texture2D*)_Texture;
	tex->Bind();

	glBegin(GL_TRIANGLES);
		for (int i = 0; i < _NumIndices; ++i)
		{
			Rocket::Core::Vertex v = _Vertices[ _Indices[i] ];
			glColor4f(v.colour.red, v.colour.green, v.colour.blue, v.colour.alpha);
			glTexCoord2f(v.tex_coord.x, v.tex_coord.y);
			glVertex2f(v.position.x, v.position.y);
		}
	glEnd();
	
	ClearMatrices();
}

Rocket::Core::CompiledGeometryHandle RenderInterface::CompileGeometry( Rocket::Core::Vertex* _Vertices, int _NumVertices, int* _Indices, int _NumIndices, Rocket::Core::TextureHandle _Texture )
{
	GeometryData* data = new GeometryData();
	data->m_DisplayList = graphics::DisplayList::Create();
	data->m_Texture = (graphics::Texture2D*)_Texture;

	data->m_DisplayList->BeginList();

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < _NumIndices; ++i)
		{
			Rocket::Core::Vertex v = _Vertices[ _Indices[i] ];
			//glColor4f(v.colour.red, v.colour.green, v.colour.blue, v.colour.alpha);
			glColor4f(v.tex_coord.x, v.tex_coord.y, 0.0f, v.colour.alpha);
			glTexCoord2f(v.tex_coord.x, v.tex_coord.y);
			glVertex2f(v.position.x, v.position.y);
		}
		glEnd();

	data->m_DisplayList->EndList();

	if (!data->m_DisplayList->IsCompiled())
	{
		LOG(VL_ERROR, "gui::RenderInterface::CompileGeometry: Failed to compile display list.");
		return 0;
	}

	return (Rocket::Core::CompiledGeometryHandle)data;
}

void RenderInterface::RenderCompiledGeometry( Rocket::Core::CompiledGeometryHandle _Geometry, const Rocket::Core::Vector2f& _Translation )
{
	SetupMatrices();

	glTranslatef(_Translation.x, _Translation.y, 0.0f);

	GeometryData* data = (GeometryData*)_Geometry;

	data->m_Texture->Bind();
	data->m_DisplayList->Draw();

	ClearMatrices();
}

void RenderInterface::ReleaseCompiledGeometry( Rocket::Core::CompiledGeometryHandle _Geometry )
{
	GeometryData* data = (GeometryData*)_Geometry;
	delete data->m_DisplayList;
	// Texture data is released in a different location.

	delete data;
}

void RenderInterface::EnableScissorRegion( bool _Enable )
{
	_Enable ? ( glEnable(GL_SCISSOR_TEST) ) : ( glDisable(GL_SCISSOR_TEST) );
}

void RenderInterface::SetScissorRegion( int _X, int _Y, int _Width, int _Height )
{
	glScissor(_X, _Y, _Width, _Height);
}

bool RenderInterface::LoadTexture( Rocket::Core::TextureHandle& _TextureHandle, Rocket::Core::Vector2i& _TextureDimensions, const Rocket::Core::String& _Source )
{
	graphics::Texture2D* tex = graphics::Texture2D::Load(std::string(_Source.CString()));

	if (!tex)
	{
		return false;
	}

	_TextureDimensions.x = tex->GetWidth();
	_TextureDimensions.y = tex->GetHeight();
	_TextureHandle = (Rocket::Core::TextureHandle)tex;

	return true;
}

bool RenderInterface::GenerateTexture( Rocket::Core::TextureHandle& _TextureHandle, const byte* _Source, const Rocket::Core::Vector2i& _SourceDimensions )
{
	graphics::Texture2D* tex = graphics::Texture2D::Create();

	if (!tex)
	{
		return false;
	}

	tex->UploadData(graphics::INTERNAL_FORMAT_RGBA8, 
					graphics::TEXTURE_FORMAT_RGBA, 
					graphics::TEXTURE_TYPE_UNSIGNED_BYTE, 
					_SourceDimensions.x, 
					_SourceDimensions.y, 
					(void*)_Source);

	_TextureHandle = (Rocket::Core::TextureHandle)tex;

	return true;
}

void RenderInterface::ReleaseTexture( Rocket::Core::TextureHandle _Texture )
{
	graphics::Texture2D* tex = (graphics::Texture2D*)_Texture;
	delete tex;
}

}

}