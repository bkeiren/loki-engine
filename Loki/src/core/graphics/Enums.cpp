#include "core/graphics/Enums.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{


	//#define USE_EXT_DEPTH_STENCIL	// If defined, certain OpenGL defined preprocessor defines will have _EXT appended to them in order to use 
	// the extension version instead of core.
	// Example: RBIF_DEPTH_STENCIL is defined to GL_DEPTH_STENCIL without USE_EXT_DEPTH_STENCIL, but it's defined
	// to GL_DEPTH_STENCIL_EXT *with* USE_EXT_DEPTH_STENCIL.

#ifdef USE_EXT_DEPTH_STENCIL
#define EXT(v)	v##_EXT
#else
#define EXT(v)	v
#endif

uint32 GLInternalFormats[_INTERNAL_FORMAT_COUNT] = { GL_DEPTH_COMPONENT,
													 GL_DEPTH_COMPONENT16,
													 GL_DEPTH_COMPONENT24,
													 GL_DEPTH_COMPONENT32,
													 EXT(GL_DEPTH_STENCIL),
													 EXT(GL_DEPTH24_STENCIL8),
													 GL_LUMINANCE,
													 GL_LUMINANCE4,
													 GL_LUMINANCE8,
													 GL_LUMINANCE12,
													 GL_LUMINANCE16,
													 GL_LUMINANCE_ALPHA,
													 GL_LUMINANCE4_ALPHA4,
													 GL_LUMINANCE6_ALPHA2,
													 GL_LUMINANCE8_ALPHA8,
													 GL_LUMINANCE12_ALPHA4,
													 GL_LUMINANCE12_ALPHA12,
													 GL_LUMINANCE16_ALPHA16,
													 GL_INTENSITY,
													 GL_INTENSITY4,
													 GL_INTENSITY8,
													 GL_INTENSITY12,
													 GL_INTENSITY16,
													 GL_R3_G3_B2,
													 GL_RGB,
													 GL_RGB4,
													 GL_RGB5,
													 GL_RGB8,
													 GL_RGB10,
													 GL_RGB12,
													 GL_RGB16,
													 GL_RGB32F,
													 GL_RGBA,
													 GL_RGBA12,
													 GL_RGBA4,
													 GL_RGB5_A1,
													 GL_RGBA8,
													 GL_RGB10_A2,
													 GL_RGBA12,
													 GL_RGBA16,
													 GL_RGBA32F };

uint32 GLTextureFormats[_TEXTURE_FORMAT_COUNT] = {	GL_COLOR_INDEX,
													GL_RED,
													GL_GREEN,
													GL_BLUE,
													GL_ALPHA,
													GL_RGB,
													GL_BGR,
													GL_RGBA,
													GL_BGRA,
													GL_LUMINANCE,
													GL_LUMINANCE_ALPHA,
													GL_DEPTH_STENCIL };

uint32 GLTextureTypes[_TEXTURE_TYPE_COUNT] = {	GL_UNSIGNED_BYTE,
												GL_BYTE,
												GL_BITMAP,
												GL_UNSIGNED_SHORT,
												GL_SHORT,
												GL_UNSIGNED_INT,
												GL_INT,
												GL_FLOAT,
												GL_UNSIGNED_BYTE_3_3_2,
												GL_UNSIGNED_BYTE_2_3_3_REV,
												GL_UNSIGNED_SHORT_5_6_5,
												GL_UNSIGNED_SHORT_5_6_5_REV,
												GL_UNSIGNED_SHORT_4_4_4_4,
												GL_UNSIGNED_SHORT_4_4_4_4_REV,
												GL_UNSIGNED_SHORT_5_5_5_1,
												GL_UNSIGNED_SHORT_1_5_5_5_REV,
												GL_UNSIGNED_INT_8_8_8_8,
												GL_UNSIGNED_INT_8_8_8_8_REV,
												GL_UNSIGNED_INT_10_10_10_2,
												GL_UNSIGNED_INT_2_10_10_10_REV,
												GL_UNSIGNED_INT_24_8 };

uint32 GLFrameBufferAttachments[_FRAMEBUFFER_ATTACHMENT_COUNT] = {	GL_COLOR_ATTACHMENT0,
																	GL_COLOR_ATTACHMENT1,
																	GL_COLOR_ATTACHMENT2,
																	GL_COLOR_ATTACHMENT3,
																	GL_COLOR_ATTACHMENT4,
																	GL_COLOR_ATTACHMENT5,
																	GL_COLOR_ATTACHMENT6,
																	GL_COLOR_ATTACHMENT7,
																	GL_COLOR_ATTACHMENT8,
																	GL_COLOR_ATTACHMENT9,
																	GL_COLOR_ATTACHMENT10,
																	GL_COLOR_ATTACHMENT11,
																	GL_COLOR_ATTACHMENT12,
																	GL_COLOR_ATTACHMENT13,
																	GL_COLOR_ATTACHMENT14,
																	GL_COLOR_ATTACHMENT15,
																	GL_DEPTH_ATTACHMENT,
																	GL_STENCIL_ATTACHMENT,
																	GL_DEPTH_STENCIL_ATTACHMENT };

uint32 GLTextureParameterNames[_TEXTURE_PARAMETER_NAME_COUNT] = {	GL_TEXTURE_WRAP_S,
																	GL_TEXTURE_WRAP_T,
																	GL_TEXTURE_MIN_FILTER,
																	GL_TEXTURE_MAG_FILTER,
																	GL_TEXTURE_PRIORITY,
																	GL_TEXTURE_BORDER_COLOR };

uint32 GLTextureParameterValues[_TEXTURE_PARAMETER_VALUE_COUNT] = {	GL_REPEAT,
																	GL_CLAMP,
																	GL_CLAMP_TO_EDGE,
																	GL_NEAREST,
																	GL_LINEAR,
																	GL_NEAREST_MIPMAP_NEAREST,
																	GL_LINEAR_MIPMAP_NEAREST,
																	GL_NEAREST_MIPMAP_LINEAR,
																	GL_LINEAR_MIPMAP_LINEAR };

uint32 GLCubeMapFaces[_CUBEMAP_FACE_COUNT] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
											   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
											   GL_TEXTURE_CUBE_MAP_POSITIVE_X,
											   GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
											   GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
											   GL_TEXTURE_CUBE_MAP_NEGATIVE_Y };

uint32 GLTextureTargets[_TEXTURE_TARGET_COUNT] = { GL_TEXTURE_1D, 
												   GL_TEXTURE_2D,
												   GL_TEXTURE_3D,
												   GL_TEXTURE_CUBE_MAP };

#ifdef EXT
#undef EXT
#endif

#define GETENUM_FUNCTION_HELPER(maxcount, array, returntype)	{ for (int32 i = 0; i < maxcount; ++i) { if (_GLEnum == array[i]) { return (returntype)i; } } }

EInternalFormat GetEnumInternalFormat( uint32 _GLEnum )
{
	GETENUM_FUNCTION_HELPER(_INTERNAL_FORMAT_COUNT, GLInternalFormats, EInternalFormat);
	return INTERNAL_FORMAT_RGBA;	// Just return something.
}

ETextureFormat GetEnumTextureFormat( uint32 _GLEnum )
{
	GETENUM_FUNCTION_HELPER(_TEXTURE_FORMAT_COUNT, GLTextureFormats, ETextureFormat);
	return TEXTURE_FORMAT_RGBA;	// Just return something.
}

ETextureType GetEnumTextureType( uint32 _GLEnum )
{
	GETENUM_FUNCTION_HELPER(_TEXTURE_TYPE_COUNT, GLTextureTypes, ETextureType);
	return TEXTURE_TYPE_INT;	// Just return something.
}

EFrameBufferAttachment GetEnumFrameBufferAttachment( uint32 _GLEnum )
{
	GETENUM_FUNCTION_HELPER(_FRAMEBUFFER_ATTACHMENT_COUNT, GLFrameBufferAttachments, EFrameBufferAttachment);
	return FRAMEBUFFER_COLOR_ATTACHMENT0;	// Just return something.
}

ETextureParameterName GetEnumTextureParameterName( uint32 _GLEnum )
{
	GETENUM_FUNCTION_HELPER(_TEXTURE_PARAMETER_NAME_COUNT, GLTextureParameterNames, ETextureParameterName);
	return TEXTURE_BORDER_COLOR;	// Just return something.
}

ETextureParameterValue GetEnumTextureParameterValue( uint32 _GLEnum )
{
	GETENUM_FUNCTION_HELPER(_TEXTURE_PARAMETER_VALUE_COUNT, GLTextureParameterValues, ETextureParameterValue);
	return CLAMP;	// Just return something.
}

ECubeMapFace GetEnumCubeMapFacesValue( uint32 _GLEnum )
{
	GETENUM_FUNCTION_HELPER(_CUBEMAP_FACE_COUNT, GLCubeMapFaces, ECubeMapFace);
	return CUBEMAP_FACE_NORTH;	// Just return something.
}

ETextureTarget GetEnumTextureTarget( uint32 _GLEnum )
{
	GETENUM_FUNCTION_HELPER(_TEXTURE_TARGET_COUNT, GLTextureTargets, ETextureTarget);
	return TEXTURE_TARGET_2D;
}

#undef GETENUM_FUNCTION_HELPER

}

}
