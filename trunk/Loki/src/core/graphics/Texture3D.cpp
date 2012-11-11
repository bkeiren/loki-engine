#include "core/graphics/Texture3D.h"
#include "SOIL/SOIL.h"
#include <GLEW\\glew.h>

namespace loki
{

namespace graphics
{

Texture3D::Texture3D()	:
	Texture(TEXTURE_TARGET_3D),
	m_Width(1),
	m_Height(1),
	m_Depth(1),
	m_InternalFormat(INTERNAL_FORMAT_RGBA)
{

}

Texture3D::~Texture3D()
{

}

Texture3D* Texture3D::Load( const std::string& _File )
{
	Texture3D* tex = Texture3D::Create();

	uint32 res = SOIL_load_OGL_texture(_File.c_str(), SOIL_LOAD_AUTO, tex->GetTextureHandle(), SOIL_FLAG_INVERT_Y);

	tex->m_File = _File;

	if (!res)
	{
		LOG(VL_ERROR, "Texture3D::Load: Failed to load texture '%s'", _File.c_str());
		delete tex;
		return 0;
	}

	tex->Bind();
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex->Unbind();

	glGenerateMipmap(GL_TEXTURE_3D);

	tex->UpdateGLInformation();

	return tex;
}

Texture3D* Texture3D::Create()
{
	Texture3D* tex = new Texture3D();
	return tex;
}

void Texture3D::SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value )
{
	glActiveTexture(GL_TEXTURE0);
	Bind();

	glTexParameteri(GL_TEXTURE_3D, GLTextureParameterNames[_Parameter], GLTextureParameterValues[_Value]);
	
	Unbind();
}

void Texture3D::UpdateGLInformation()
{
	// Obtain texture information from OpenGL.

	int32 InternalFormat = 0;

	glActiveTexture(GL_TEXTURE0);
	Bind();

	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &m_Width);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &m_Height);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &m_Depth);
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_INTERNAL_FORMAT, &InternalFormat);
	
	Unbind();

	// NOTE: Why is this here...?
	SetTextureParameter(TEXTURE_WRAP_S, REPEAT);
	SetTextureParameter(TEXTURE_WRAP_T, REPEAT);

	m_InternalFormat = GetEnumInternalFormat(InternalFormat);
}

int32 Texture3D::GetWidth() const
{
	return m_Width;
}

int32 Texture3D::GetHeight() const
{
	return m_Height;
}

int32 Texture3D::GetDepth() const
{
	return m_Depth;
}

void Texture3D::UploadData( EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, int32 _Height, int32 _Depth,  const void* _Data )
{
	Bind();

	glTexImage3D(GL_TEXTURE_3D, 0, GLInternalFormats[_InternalFormat], _Width, _Height, _Depth, 0, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	glGenerateMipmap(GL_TEXTURE_3D);	// Generate mipmaps.
	UpdateGLInformation();

	Unbind();
}

void Texture3D::UploadSubData( ETextureFormat _Format, ETextureType _Type, int32 _XOffset, int32 _YOffset, int32 _ZOffset, int32 _Width, int32 _Height, int32 _Depth, const void* _Data )
{
	Bind();

	glTexSubImage3D(GL_TEXTURE_3D, 0, _XOffset, _YOffset, _ZOffset, _Width, _Height, _Depth, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	glGenerateMipmap(GL_TEXTURE_3D);	// Generate mipmaps.
	UpdateGLInformation();

	Unbind();
}

}

}