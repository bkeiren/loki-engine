#include "core/graphics/TextureCube.h"
#include "SOIL/SOIL.h"
#include <GLEW\\glew.h>

namespace loki
{

namespace graphics
{

TextureCube::TextureCube()	:
	Texture(TEXTURE_TARGET_CUBE_MAP),
	m_Width(1),
	m_Height(1),
	m_InternalFormat(INTERNAL_FORMAT_RGBA)
{

}

TextureCube::~TextureCube()
{

}

TextureCube* TextureCube::Load( const std::string& _File, bool _InvertY /*= false*/ )
{
	TextureCube* tex = TextureCube::Create();

	static const char FaceOrder[6] = { 'N', 'E', 'S', 'W', 'U', 'D' };
	uint32 res = SOIL_load_OGL_single_cubemap(_File.c_str(), FaceOrder, SOIL_LOAD_AUTO, tex->GetTextureHandle(), _InvertY ? SOIL_FLAG_INVERT_Y : 0);

	tex->m_File = _File;

	return _LoadHelper(res, tex);
}

TextureCube* TextureCube::Load( const std::string& _FileNorth, const std::string& _FileEast, const std::string& _FileSouth, const std::string& _FileWest, const std::string& _FileUp, const std::string& _FileDown, bool _InvertY /*= false*/ )
{
	TextureCube* tex = TextureCube::Create();

	uint32 res = SOIL_load_OGL_cubemap( _FileEast.c_str(), 
										_FileWest.c_str(), 
										_FileUp.c_str(),
										_FileDown.c_str(), 
										_FileNorth.c_str(), 
										_FileSouth.c_str(), 
										SOIL_LOAD_AUTO, tex->GetTextureHandle(), _InvertY ? SOIL_FLAG_INVERT_Y : 0);

	tex->m_File = _FileNorth;
	tex->m_File += _FileEast;
	tex->m_File += _FileSouth;
	tex->m_File += _FileWest;
	tex->m_File += _FileUp;
	tex->m_File += _FileDown;

	return _LoadHelper(res, tex);
}

TextureCube* TextureCube::Create()
{
	TextureCube* tex = new TextureCube();
	return tex;
}

void TextureCube::SetTextureParameter( ETextureParameterName _Parameter, ETextureParameterValue _Value )
{
	glActiveTexture(GL_TEXTURE0);
	Bind();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, 
					GLTextureParameterNames[_Parameter], 
					GLTextureParameterValues[_Value]);
	
	Unbind();
}

void TextureCube::UpdateGLInformation()
{
	// Obtain texture information from OpenGL.
	glActiveTexture(GL_TEXTURE0);
	Bind();

	int32 Widths[6];
	int32 Heights[6];
	int32 InternalFormats[6];

	for (int32 i = 0; i < 6; ++i)
	{
		glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
			0, 
			GL_TEXTURE_WIDTH, 
			&Widths[i]);
		glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_TEXTURE_HEIGHT, &Heights[i]);
		glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_TEXTURE_INTERNAL_FORMAT, &InternalFormats[i]);
	}

	for (int32 i = 1; i < 6; ++i)
	{
		if (Widths[i] != Widths[0])
		{
			LOG(VL_WARN, "TextureCube::UpdateGLInformation: Not all sides have the same width.");
		}
		if (Heights[i] != Heights[0])
		{
			LOG(VL_WARN, "TextureCube::UpdateGLInformation: Not all sides have the same height.");
		}
		if (InternalFormats[i] != InternalFormats[0])
		{
			LOG(VL_WARN, "TextureCube::UpdateGLInformation: Not all sides have the same internal format.");
		}
	}
		
	Unbind();

	m_Width = Widths[0];
	m_Height = Heights[0];
	m_InternalFormat = GetEnumInternalFormat(InternalFormats[0]);
}

int32 TextureCube::GetWidth() const
{
	return m_Width;
}

int32 TextureCube::GetHeight() const
{
	return m_Height;
}

void TextureCube::UploadData( ECubeMapFace _Face, EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type, int32 _Width, int32 _Height, const void* _Data )
{
	Bind();

	glTexImage2D(GLCubeMapFaces[_Face], 0, GLInternalFormats[_InternalFormat], _Width, _Height, 0, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);	// Generate mipmaps.
	UpdateGLInformation();

	Unbind();
}

void TextureCube::UploadSubData( ECubeMapFace _Face, ETextureFormat _Format, ETextureType _Type, int32 _XOffset, int32 _YOffset, int32 _Width, int32 _Height, const void* _Data )
{
	Bind();

	glTexSubImage2D(GLCubeMapFaces[_Face], 0, _XOffset, _YOffset, _Width, _Height, GLTextureFormats[_Format], GLTextureTypes[_Type], _Data);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);	// Generate mipmaps.
	UpdateGLInformation();

	Unbind();
}

TextureCube* TextureCube::_LoadHelper( int32 _SOIL_Load_Result, TextureCube* _Tex )
{
	if (!_SOIL_Load_Result)
	{
		LOG(VL_ERROR, "TextureCube::Load: Failed to load texture '%s'", _Tex->m_File.c_str());
		delete _Tex;
		return 0;
	}

	_Tex->SetTextureParameter(TEXTURE_MIN_FILTER, LINEAR);
	_Tex->SetTextureParameter(TEXTURE_MAG_FILTER, LINEAR);
	_Tex->SetTextureParameter(TEXTURE_WRAP_S, CLAMP_TO_EDGE);
	_Tex->SetTextureParameter(TEXTURE_WRAP_T, CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	_Tex->UpdateGLInformation();

	return _Tex;
}

}

}