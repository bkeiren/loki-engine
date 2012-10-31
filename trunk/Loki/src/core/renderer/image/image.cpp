#include "core/renderer/image/image.h"
#include "core/graphics/Texture.h"
#include "core/renderer/renderer.h"
#include "core/resourcemanager/texturemanager.h"

#include "core/renderer/effect/effectmanager.h"

#include "core/engine.h"
#include "core/game/game.h"

#include "core/entitysystem/component/default/CameraComponent.h"

#ifdef USE_PBO
#include "core/graphics/PixelBuffer.h"
#endif

namespace loki
{

namespace renderer
{

LkEffect* LkImage::m_CgEffect = NULL;

LkImage::LkImage( const char* _Texture, const vec3& _Position, const vec2& _Size, bool _PositionIsAbsolute /*= false*/, bool _SizeIsAbsolute /*= false*/ )	:
	m_Position(_Position),
	m_Size(_Size),
	m_Is3D(false),
	m_TextureIndex(0),
	m_UVTopLeft(vec2(0.0f, 0.0f)),
	m_UVBottomRight(vec2(1.0f, 1.0f)),
	m_FlipX(false),
	m_FlipY(false)
{
	_Init(_Texture, _Position, _Size, _PositionIsAbsolute, _SizeIsAbsolute);
}

LkImage::LkImage( const char* _Texture, const vec2& _Position, const vec2& _Size, bool _PositionIsAbsolute /*= false*/, bool _SizeIsAbsolute /*= false*/ )	:
	m_Position(vec3(_Position, 0.0f)),
	m_Size(_Size),
	m_Is3D(false),
	m_TextureIndex(0),
	m_UVTopLeft(vec2(0.0f, 0.0f)),
	m_UVBottomRight(vec2(1.0f, 1.0f)),
	m_FlipX(false),
	m_FlipY(false)
{
	_Init(_Texture, vec3(_Position, 0.0f), _Size, _PositionIsAbsolute, _SizeIsAbsolute);
}

LkImage::LkImage()
{
	ILLEGAL_CTOR_ERROR("Image");
}

LkImage::~LkImage()
{
	for (TexturesIter it = m_Textures.begin(); it != m_Textures.end(); ++it)
	{
		if ((*it).first)	// Only release through the resource manager if the texture was created through the resource manager.
		{
			//g_TextureManager->ReleaseResource(&((*it).second));
			delete (*it).second;
		}
		else
		{
			delete (*it).second;
		}
	}

#ifdef USE_PBO
	delete m_PBO;
	m_PBO = 0;
#endif
}

void LkImage::_Init( const char* _Texture, const vec3& _Position, const vec2& _Size, bool _PositionIsAbsolute /*= false*/, bool _SizeIsAbsolute /*= false*/ )
{
#ifdef USE_PBO
	m_PBO = graphics::PixelBuffer::Create(graphics::PixelBuffer::PIXEL_BUFFER_UNPACK__CPU_TO_GPU);
	m_PBO->Resize(1);
	//m_PBO = new LkPixelBufferObject(PBO_UNPACK, PBO_DYNAMIC_DRAW, 1);
#endif

	//assert(_Texture);
	static bool CgEffectLoaded = false;
	if (!CgEffectLoaded)
	{
		m_CgEffect = renderer::g_EffectManager->CreateEffectFromFile("resources//shaders//ui.cgfx", "ImageEffect");
		if (m_CgEffect)
		{
			CgEffectLoaded = true;
		}
	}

	if (_PositionIsAbsolute)
	{
		SetAbsolutePosition(_Position);
	}
	else
	{
		SetRelativePosition(_Position);
	}

	if (_SizeIsAbsolute)
	{
		SetAbsoluteSize(_Size);
	}
	else
	{
		SetRelativeSize(_Size);
	}

	AddTexture(_Texture);

	SetAnchorPoint(AP_TOPLEFT);
}

bool LkImage::AddTexture( const char* _Texture )
{
	if (!_Texture)
	{
		return false;
	}

//	LkTexture* tex = g_TextureManager->GetResource(_Texture);
	graphics::Texture* tex = graphics::Texture::Load(_Texture);
	
	if (!tex)
	{
		return false;
	}

	m_Textures.push_back(std::pair<bool, graphics::Texture*>(true, tex));
	return true;
}

bool LkImage::AddTextureFromMemory( const void* _Data, uint32 _Width, uint32 _Height, EInternalFormat _InternalFormat, ETextureFormat _Format, ETextureType _Type )
{
	// TODO: Maybe use the resource manager some how?

	GLuint texture;
	glGenTextures(1, &texture);

	if (!texture)
	{
		return false;
	}

	//LkTexture* tex = new LkTexture("LkImage::AddTextureFromMemory:Texture", texture);
	graphics::Texture* tex = graphics::Texture::Create();

	tex->Bind();

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// This line indicates that if any change occurs to the base mipmap level, the other levels should be regenerated.
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	
	tex->UploadData(_InternalFormat, _Format, _Type, _Width, _Height, _Data);
//	glTexImage2D(GL_TEXTURE_2D, 0, _InternalFormat, _Width, _Height, 0, _Format, _Type, _Data);

	tex->Bind();
	glGenerateMipmap(GL_TEXTURE_2D);	// Apparently this is hardware accelerated, which gluBuild2DMipMaps is not.
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, _Width, _Height, _Format, _Type, _Data);

	tex->Unbind();

	tex->UpdateGLInformation();

	m_Textures.push_back(std::pair<bool, graphics::Texture*>(false, tex));
	//*(const_cast<int32*>(&(tex->m_Width))) = _Width;
	//*(const_cast<int32*>(&(tex->m_Height))) = _Height;

#ifdef USE_PBO
	m_PBO->Resize(tex->GetWidth() * tex->GetHeight() * 4);	//m_PBO->Resize(tex->m_Width * tex->m_Height * 4);
#endif

	return true;
}

void LkImage::SetSubTextureFromMemory( const void* _Data, int32 _XOffset, int32 _YOffset, uint32 _Width, uint32 _Height, ETextureFormat _Format, ETextureType _Type )
{
	graphics::Texture* tex = m_Textures[m_TextureIndex].second;
#ifdef USE_PBO

#define USE_ORPHANANDMAP
	tex->Bind();	//glBindTexture(GL_TEXTURE_2D, tex->GetTextureHandle());
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->GetWidth());
#ifdef USE_ORPHANANDMAP
	m_PBO->UploadData(m_PBO->GetSize(), 0, graphics::Buffer::BUFFER_USAGE_DYNAMIC_DRAW);	//m_PBO->BufferData(0);
	void* m = m_PBO->Map(graphics::Buffer::BUFFER_MAPPING_WRITE_ONLY);	//void* m = m_PBO->MapBuffer(PBO_MAP_WRITE_ONLY);
	memcpy_s(m, m_PBO->GetSize(), _Data, _Width * _Height * 4);
	m_PBO->Unmap();	//m_PBO->UnmapBuffer();
	m_PBO->Bind();
	tex->UploadSubData(_Format, _Type, _XOffset, _YOffset, _Width, _Height, (void*)0);	//glTexSubImage2D(GL_TEXTURE_2D, 0, _XOffset, _YOffset, _Width, _Height, _Format, _Type, (void*)0);
	m_PBO->Unbind();
#else
#define USE_BUFFERSUB	// If defined, uses BufferSubData instead of orphaning the buffer and copying the data.
#ifdef USE_BUFFERSUB
	m_PBO->BufferSubData(0, _Width * _Height * 4, _Data);
#else
	m_PBO->BufferData(0);
	m_PBO->BufferData(_Data);
#endif
	glTexSubImage2D(GL_TEXTURE_2D, 0, _XOffset, _YOffset, _Width, _Height, _Format, _Type, (void*)0);
	m_PBO->Unbind();
#endif
#undef USE_ORPHANANDMAP
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	tex->Unbind();	//glBindTexture(GL_TEXTURE_2D, 0);
#else
	glBindTexture(GL_TEXTURE_2D, tex->m_OpenGLTextureID);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->m_Width);
	glTexSubImage2D(GL_TEXTURE_2D, 0, _XOffset, _YOffset, _Width, _Height, _Format, _Type, _Data);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
}

void LkImage::SetTextureIndex( uint32 _Index )
{
	if (_Index >= 0 && _Index < m_Textures.size())
	{
		m_TextureIndex = _Index;
	}
}

uint32 LkImage::GetTextureIndex() const
{
	return m_TextureIndex;
}

uint32 LkImage::GetNumTextureIndices() const
{
	return m_Textures.size();
}

void LkImage::SetFlipX( bool _Flip )
{
	m_FlipX = _Flip;
}

void LkImage::SetFlipY( bool _Flip )
{
	m_FlipY = _Flip;
}

bool LkImage::IsFlippedX() const
{
	return m_FlipX;
}

bool LkImage::IsFlippedY() const
{
	return m_FlipY;
}

void LkImage::Render()
{
	if (!(m_TextureIndex < GetNumTextureIndices()))
	{
		return;
	}

	LkEffectParameter* param = m_CgEffect->GetParameterBySemantic("LKDIFFUSETEX");
	if (param)
	{
		param->Set(m_Textures[m_TextureIndex].second->GetTextureHandle());
	}
	
	param = m_CgEffect->GetParameterBySemantic("LKMODELVIEWPROJ");
	if (param)
	{
		param->Set(Get3DModelViewProjectionMatrix());
	}

	while (m_CgEffect->HasNextPass())
	{
		// Need to convert the size to actual coordinates.
		vec2 size;
		if (Is3D())
		{
			size = m_Size * 2.0f;
		}
		else
		{
			size = vec2(((m_Size.x * g_Renderer->GetRenderHeight()) / g_Renderer->GetRenderWidth()), m_Size.y) * 2.0f;
		}
		vec3 pos = ((vec3(m_Position.x, 1.0f - m_Position.y, m_Position.z) + vec3(m_AnchorPointVector, 0.0f)) * 2.0f) - 1.0f;

		f32 uv_x_left = 0.0f;
		f32 uv_x_right = 0.0f;
		f32 uv_y_top = 0.0f;
		f32 uv_y_bottom = 0.0f;

		if (m_FlipX)
		{
			uv_x_left = m_UVBottomRight.x;
			uv_x_right = m_UVTopLeft.x;
		}
		else
		{
			uv_x_left = m_UVTopLeft.x;
			uv_x_right = m_UVBottomRight.x;
		}

		if (m_FlipY)
		{
			uv_y_top = m_UVBottomRight.y;
			uv_y_bottom = m_UVTopLeft.y;
		}
		else
		{
			uv_y_top = m_UVTopLeft.y;
			uv_y_bottom = m_UVBottomRight.y;			
		}

		glBegin(GL_QUADS);
			glTexCoord2f(uv_x_left, uv_y_bottom);
			glVertex3f(pos.x, pos.y + size.y, pos.z);
			glTexCoord2f(uv_x_right, uv_y_bottom);
			glVertex3f(pos.x + size.x, pos.y + size.y, pos.z);
			glTexCoord2f(uv_x_right, uv_y_top);
			glVertex3f(pos.x + size.x, pos.y, pos.z);
			glTexCoord2f(uv_x_left, uv_y_top);
			glVertex3f(pos.x, pos.y, pos.z);
		glEnd();
	}
}

vec3 LkImage::GetAbsolutePosition() const
{
	return vec3(m_Position.x * g_Renderer->GetRenderWidth(), m_Position.y * g_Renderer->GetRenderHeight(), m_Position.z);
}

const vec3& LkImage::GetRelativePosition() const
{
	return m_Position;
}

vec2 LkImage::GetAbsoluteSize() const
{
	// Size values are always relative to the window HEIGHT (Not the width, not both).
	return vec2(m_Size.x * g_Renderer->GetRenderHeight(), m_Size.y * g_Renderer->GetRenderHeight());
}

const vec2& LkImage::GetRelativeSize() const
{
	return m_Size;
}

vec2 LkImage::GetAbsoluteAnchorOffset() const
{
	return vec2(m_AnchorPointVector.x * g_Renderer->GetRenderHeight(), m_AnchorPointVector.y * g_Renderer->GetRenderHeight());
}

const vec2& LkImage::GetRelativeAnchorOffset() const
{
	return m_AnchorPointVector;
}

bool LkImage::Is3D() const
{
	return m_Is3D;
}

void LkImage::Set3D( bool _State )
{
	m_Is3D = _State;
}

void LkImage::SetAbsolutePosition( const vec3& _Position )
{
	m_Position.x = _Position.x / g_Renderer->GetRenderWidth();
	m_Position.y = _Position.y / g_Renderer->GetRenderHeight();
}

void LkImage::SetAbsolutePosition( const vec2& _Position )
{
	m_Position.x = _Position.x / g_Renderer->GetRenderWidth();
	m_Position.y = _Position.y / g_Renderer->GetRenderHeight();
}

void LkImage::SetRelativePosition( const vec3& _Position )
{
	m_Position = _Position;
}

void LkImage::SetRelativePosition( const vec2& _Position )
{
	m_Position.x = _Position.x;
	m_Position.y = _Position.y;
}

void LkImage::SetAbsoluteSize( const vec2& _Size )
{
	m_Size = vec2(_Size.x / g_Renderer->GetRenderHeight(), _Size.y / g_Renderer->GetRenderHeight());

	// Recalculate anchor point vector.
	SetAnchorPoint(m_AnchorPoint);
}

void LkImage::SetRelativeSize( const vec2& _Size )
{
	m_Size = _Size;

	// Recalculate anchor point vector.
	SetAnchorPoint(m_AnchorPoint);
}

// void LkImage::SetTexture( const char* _Texture )
// {
// 	assert(_Texture);
// 
// 	g_TextureManager->ReleaseResource(&m_Texture);
// 	m_Texture = g_TextureManager->GetResource(_Texture);
// }

void LkImage::SetAnchorPoint( EAnchorPoint _AnchorPoint )
{
	m_AnchorPoint = _AnchorPoint;

	f32 size_x = (m_Size.x * g_Renderer->GetRenderHeight()) / g_Renderer->GetRenderWidth();

	switch (m_AnchorPoint)
	{
	case AP_TOPLEFT:
		m_AnchorPointVector = vec2(0.0f, -m_Size.y);
		break;
	case AP_TOPMIDDLE:
		m_AnchorPointVector = vec2(-size_x * 0.5f, -m_Size.y);
		break;
	case AP_TOPRIGHT:
		m_AnchorPointVector = vec2(-size_x, -m_Size.y);
		break;
	case AP_MIDDLELEFT:
		m_AnchorPointVector = vec2(0.0f, -m_Size.y * 0.5f);
		break;
	case AP_CENTER:
		m_AnchorPointVector = vec2(-size_x * 0.5f, -m_Size.y * 0.5f);
		break;
	case AP_MIDDLERIGHT:
		m_AnchorPointVector = vec2(-size_x, -m_Size.y * 0.5f);
		break;
	case AP_BOTTOMLEFT:
		m_AnchorPointVector = vec2(0.0f, 0.0f);
		break;
	case AP_BOTTOMMIDDLE:
		m_AnchorPointVector = vec2(-size_x * 0.5f, 0.0f);
		break;
	case AP_BOTTOMRIGHT:
		m_AnchorPointVector = vec2(-size_x, 0.0f);
		break;
	}
}

int32 LkImage::GetWidth() const
{
	if (m_TextureIndex < GetNumTextureIndices())
	{
		return m_Textures[m_TextureIndex].second->GetWidth();
	}
	return 0;
}

int32 LkImage::GetHeight() const
{
	if (m_TextureIndex < GetNumTextureIndices())
	{
		return m_Textures[m_TextureIndex].second->GetHeight();
	}
	return 0;
}

mat4 LkImage::GetModelMatrix() const
{
	mat4 m = math::gtc::quaternion::mat4_cast(m_Orientation);
	m = math::gtc::matrix_transform::translate(m, m_Position);
	return m;
}

mat4 LkImage::Get3DModelViewProjectionMatrix() const
{
	if (Is3D())
	{
		components::CameraComponent* camera = components::CameraComponent::GetActiveCamera();

		mat4 proj = camera->GetProjectionMatrix();
		//proj = math::gtc::matrix_transform::ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 100.0f);
		mat4 view = camera->GetViewMatrix();
		mat4 model = GetModelMatrix();

		return (proj * view * model);
	}
	return mat4();
}

mat4 LkImage::Get3DModelViewProjectionMatrixInverse() const
{
	if (Is3D())
	{
		components::CameraComponent* camera = components::CameraComponent::GetActiveCamera();

		mat4 proj = camera->GetProjectionMatrix();
		//proj = math::gtc::matrix_transform::ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 100.0f);
		mat4 view = camera->GetViewMatrix();
		mat4 model = GetModelMatrix();

		return (math::inverse(model) * math::inverse(view) * math::inverse(proj));
	}
	return mat4();
}

mat4 LkImage::GetProjectionMatrix() const
{
	return components::CameraComponent::GetActiveCamera()->GetProjectionMatrix();
}

mat4 LkImage::GetViewMatrix() const
{
	return components::CameraComponent::GetActiveCamera()->GetViewMatrix();
}

const quat& LkImage::GetOrientation() const
{
	return m_Orientation;
}

void LkImage::SetOrientation( const quat& _Orientation )
{
	m_Orientation = _Orientation;
}

f32 LkImage::GetWindowWidthValue()
{
	return ((f32)g_Renderer->GetRenderWidth() / (f32)g_Renderer->GetRenderHeight());
}

}

}