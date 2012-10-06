#include "core/graphics/Texture.h"
#include "SOIL/SOIL.h"

namespace loki
{

namespace graphics
{

Texture::Texture()	:
	m_GLTextureHandle(0),
	m_File(std::string("")),
	m_Width(1),
	m_Height(1),
	m_OpenGLInternalFormat(0),
	m_OpenGLTextureDepth(0)
{
	glGenTextures(1, &m_GLTextureHandle);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_GLTextureHandle);
}

Texture* Texture::Load( const std::string& _File )
{
	Texture* tex = Texture::Create();

	uint32 res = SOIL_load_OGL_texture(_File.c_str(), SOIL_LOAD_AUTO, tex->GetTextureHandle(), SOIL_FLAG_INVERT_Y);

	if (!res)
	{
		LOG(VL_ERROR, "Texture::Load: Failed to load texture '%s'", _File.c_str());
		return 0;
	}

	tex->Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex->Unbind();

	tex->UpdateGLInformation();

	return tex;
}

Texture* Texture::Create()
{
	Texture* tex = new Texture();
	return tex;
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_GLTextureHandle);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint32 Texture::GetTextureHandle() const
{
	return m_GLTextureHandle;
}

void Texture::SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value )
{
	glActiveTexture(GL_TEXTURE0);
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GLTextureParameterNames[_Parameter], GLTextureParameterValues[_Value]);
	
	Unbind();
}

void Texture::UpdateGLInformation()
{
	// Obtain texture information from OpenGL.
	glActiveTexture(GL_TEXTURE0);
	Bind();

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_Width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_Height);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &m_OpenGLInternalFormat);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH, &m_OpenGLTextureDepth);
	
	Unbind();

	SetTextureParameter(TEXTURE_WRAP_S, REPEAT);
	SetTextureParameter(TEXTURE_WRAP_T, REPEAT);
}

int32 Texture::GetWidth() const
{
	return m_Width;
}

int32 Texture::GetHeight() const
{
	return m_Height;
}

void Texture::UploadData( EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, int32 _Height, const void* _Data )
{
	Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GLInternalFormats[_InternalFormat], _Width, _Height, 0, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	UpdateGLInformation();

	Unbind();
}

}

}