#include "core/graphics/Texture2D.h"
#include "SOIL/SOIL.h"
#include <GLEW\\glew.h>

namespace loki
{

namespace graphics
{

Texture2D::Texture2D()	:
	Texture(TEXTURE_TARGET_2D),
	m_Width(1),
	m_Height(1),
	m_InternalFormat(INTERNAL_FORMAT_RGBA)
{
	
}

Texture2D::~Texture2D()
{

}

Texture2D* Texture2D::Load( const std::string& _File, bool _InvertY /*= true*/ )
{
	Texture2D* tex = Texture2D::Create();

	uint32 res = SOIL_load_OGL_texture(_File.c_str(), SOIL_LOAD_AUTO, tex->GetTextureHandle(), _InvertY ? SOIL_FLAG_INVERT_Y : 0);

	tex->m_File = _File;

	if (!res)
	{
		LOG(VL_ERROR, "Texture2D::Load: Failed to load texture '%s'", _File.c_str());
		delete tex;
		return 0;
	}

	tex->Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex->Unbind();

	glGenerateMipmap(GL_TEXTURE_2D);

	tex->UpdateGLInformation();

	return tex;
}

Texture2D* Texture2D::Create()
{
	Texture2D* tex = new Texture2D();
	return tex;
}

void Texture2D::SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value )
{
	glActiveTexture(GL_TEXTURE0);
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GLTextureParameterNames[_Parameter], GLTextureParameterValues[_Value]);
	
	Unbind();
}

void Texture2D::UpdateGLInformation()
{
	// Obtain texture information from OpenGL.

	int32 InternalFormat = 0;

	glActiveTexture(GL_TEXTURE0);
	Bind();

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_Width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_Height);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &InternalFormat);
	
	Unbind();

	// NOTE: Why is this here...?
	SetTextureParameter(TEXTURE_WRAP_S, REPEAT);
	SetTextureParameter(TEXTURE_WRAP_T, REPEAT);

	m_InternalFormat = GetEnumInternalFormat(InternalFormat);
}

int32 Texture2D::GetWidth() const
{
	return m_Width;
}

int32 Texture2D::GetHeight() const
{
	return m_Height;
}

void Texture2D::UploadData( EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, int32 _Height, const void* _Data )
{
	Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GLInternalFormats[_InternalFormat], _Width, _Height, 0, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	glGenerateMipmap(GL_TEXTURE_2D);	// Generate mipmaps.
	UpdateGLInformation();

	Unbind();
}

void Texture2D::UploadSubData( ETextureFormat _Format, ETextureType _Type, int32 _XOffset, int32 _YOffset, int32 _Width, int32 _Height, const void* _Data )
{
	Bind();

	glTexSubImage2D(GL_TEXTURE_2D, 0, _XOffset, _YOffset, _Width, _Height, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	glGenerateMipmap(GL_TEXTURE_2D);	// Generate mipmaps.
	UpdateGLInformation();

	Unbind();
}

}

}