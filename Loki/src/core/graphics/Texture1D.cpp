#include "core/graphics/Texture1D.h"
#include "SOIL/SOIL.h"
#include <GLEW\\glew.h>

namespace loki
{

namespace graphics
{

Texture1D::Texture1D()	:
	Texture(TEXTURE_TARGET_1D),
	m_Width(1),
	m_InternalFormat(INTERNAL_FORMAT_RGBA)
{

}

Texture1D::~Texture1D()
{

}

Texture1D* Texture1D::Load( const std::string& _File )
{
	Texture1D* tex = Texture1D::Create();

	uint32 res = SOIL_load_OGL_texture(_File.c_str(), SOIL_LOAD_AUTO, tex->GetTextureHandle(), SOIL_FLAG_INVERT_Y);

	tex->m_File = _File;

	if (!res)
	{
		LOG(VL_ERROR, "Texture1D::Load: Failed to load texture '%s'", _File.c_str());
		delete tex;
		return 0;
	}

	tex->Bind();
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex->Unbind();

	glGenerateMipmap(GL_TEXTURE_1D);

	tex->UpdateGLInformation();

	return tex;
}

Texture1D* Texture1D::Create()
{
	Texture1D* tex = new Texture1D();
	return tex;
}

void Texture1D::SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value )
{
	glActiveTexture(GL_TEXTURE0);
	Bind();

	glTexParameteri(GL_TEXTURE_1D, GLTextureParameterNames[_Parameter], GLTextureParameterValues[_Value]);
	
	Unbind();
}

void Texture1D::UpdateGLInformation()
{
	// Obtain texture information from OpenGL.

	int32 InternalFormat = 0;

	glActiveTexture(GL_TEXTURE0);
	Bind();

	glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_WIDTH, &m_Width);
	glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_INTERNAL_FORMAT, &InternalFormat);
	
	Unbind();

	// NOTE: Why is this here...?
	SetTextureParameter(TEXTURE_WRAP_S, REPEAT);
	SetTextureParameter(TEXTURE_WRAP_T, REPEAT);

	m_InternalFormat = GetEnumInternalFormat(InternalFormat);
}

int32 Texture1D::GetWidth() const
{
	return m_Width;
}

void Texture1D::UploadData( EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, const void* _Data )
{
	Bind();

	glTexImage1D(GL_TEXTURE_1D, 0, GLInternalFormats[_InternalFormat], _Width, 0, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	glGenerateMipmap(GL_TEXTURE_1D);	// Generate mipmaps.
	UpdateGLInformation();

	Unbind();
}

void Texture1D::UploadSubData( ETextureFormat _Format, ETextureType _Type, int32 _XOffset, int32 _Width, const void* _Data )
{
	Bind();

	glTexSubImage1D(GL_TEXTURE_1D, 0, _XOffset, _Width, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	glGenerateMipmap(GL_TEXTURE_1D);	// Generate mipmaps.
	UpdateGLInformation();

	Unbind();
}

}

}