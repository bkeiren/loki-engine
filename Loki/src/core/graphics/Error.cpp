#include "core/graphics/Error.h"
#include "glew/glew.h"

namespace loki
{

namespace graphics
{

void CheckGLErrors( int _Line, const char* _File )
{
#define SUFFIX_HELPER	" - %s : (%i)", _File, _Line

	GLenum error = 0;
	do
	{
		error = glGetError();
		switch (error)
		{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			LOG(VL_ERROR, "CheckGLErrors: GL_INVALID_ENUM"SUFFIX_HELPER);
			break;
		case GL_INVALID_VALUE:
			LOG(VL_ERROR, "CheckGLErrors: GL_INVALID_VALUE"SUFFIX_HELPER);
			break;
		case GL_INVALID_OPERATION:
			LOG(VL_ERROR, "CheckGLErrors: GL_INVALID_OPERATION (Invalid function called within glBegin(), glEnd() pair?)"SUFFIX_HELPER);
			break;
		case GL_STACK_OVERFLOW:
			LOG(VL_ERROR, "CheckGLErrors: GL_STACK_OVERFLOW"SUFFIX_HELPER);
			break;
		case GL_STACK_UNDERFLOW:
			LOG(VL_ERROR, "CheckGLErrors: GL_STACK_UNDERFLOW"SUFFIX_HELPER);
			break;
		case GL_OUT_OF_MEMORY:
			LOG(VL_ERROR, "CheckGLErrors: GL_OUT_OF_MEMORY"SUFFIX_HELPER);
			break;
		case GL_TABLE_TOO_LARGE:
			LOG(VL_ERROR, "CheckGLErrors: GL_TABLE_TOO_LARGE"SUFFIX_HELPER);
			break;
		default:
			LOG(VL_ERROR, "CheckGLErrors: Unhandled error (%i) - %s : (%i)", (int)error, _File, _Line);
			break;
		}
	} while (error != GL_NO_ERROR);

#undef SUFFIX_HELPER
}

}

}
