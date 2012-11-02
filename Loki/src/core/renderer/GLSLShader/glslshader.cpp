#include "core/renderer/GLSLShader/glslshader.h"
//#include "core/filesystem/filesystem.h"

namespace loki
{

namespace renderer
{

const char* ATTRIB_LOCATION_NAMES[LkGLSLShader::ATTRIB_Count] = {	/*"AL_START_INDEX",*/
																"AttribPosition", 
																"AttribNormal", 
																"AttribTangent", 
																"AttribBitanget",
																"AttribTexcoord",
																"AttribZFar",
																"AttribZNear"
															  };

LkGLSLShader::LkGLSLShader()	:
	m_NumVertexShaders(0),
	m_NumFragmentShaders(0),
	m_ProgramHandle(0)
{
	
}

LkGLSLShader::~LkGLSLShader()
{

}

// bool GLSLShader::LoadFromFile( const char* _Vertex, const char* _Fragment )
// {
// 	
// 	LOG(VL_NORMAL, "Succesfully loaded shader from files:\n\t'%s'\n\t'%s'", _Vertex, _Fragment);
// 	return true;
// }

void LkGLSLShader::Activate() const
{
	glUseProgram(m_ProgramHandle);
}

void LkGLSLShader::Deactivate()
{
	glUseProgram(0);
}

bool LkGLSLShader::AttachVertexShader( const char* _Shader )
{
	if (m_NumVertexShaders >= GLSLSHADER_MAX_OBJECTS)
	{
		LOG(VL_WARN, "GLSLShader::AttachVertexShader: A maximum of %i vertex shaders can be attached", GLSLSHADER_MAX_OBJECTS);
		return false;
	}

	m_VertexShaders[m_NumVertexShaders]	= glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

	filesystem::LkFile* vertexfile = filesystem::OpenFile(_Shader);

	if (!vertexfile->IsOpen())
	{
		LOG(VL_ERROR, "GLSLShader::AttachVertexShader: Could not open vertex shader '%s'", _Shader);
		return false;
	}

	const char* vertexsource = vertexfile->GetBuffer();

	glShaderSourceARB(m_VertexShaders[m_NumVertexShaders], 1, &vertexsource, NULL);
	glCompileShaderARB(m_VertexShaders[m_NumVertexShaders]);

	delete[] vertexsource;	// Vertex source is no longer required.

	int32 result = false;
	glGetObjectParameterivARB( m_VertexShaders[m_NumVertexShaders], GL_OBJECT_COMPILE_STATUS_ARB, &result );
	if( result == false )
	{	
		GLsizei errorLogLength = 0;
		static const GLsizei errorLogMaxLength = 1024;
		static GLchar errorLog[errorLogMaxLength];
		glGetShaderInfoLog(m_VertexShaders[m_NumVertexShaders], errorLogMaxLength, &errorLogLength, errorLog);

		LOG(VL_ERROR, "(VERTEX) %s\n%s", _Shader, (char*)errorLog);
		return false;
	}

	++m_NumVertexShaders;
	return true;
}

bool LkGLSLShader::AttachFragmentShader( const char* _Shader )
{
	if (m_NumFragmentShaders >= GLSLSHADER_MAX_OBJECTS)
	{
		LOG(VL_WARN, "GLSLShader::AttachVertexShader: A maximum of %i fragment shaders can be attached", GLSLSHADER_MAX_OBJECTS);
		return false;
	}

	m_FragmentShaders[m_NumFragmentShaders]	= glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	filesystem::LkFile* fragmentfile = filesystem::OpenFile(_Shader);

	if (!fragmentfile->IsOpen())
	{
		LOG(VL_ERROR, "GLSLShader::AttachFragmentShader: Could not open fragment shader '%s'", _Shader);
		return false;
	}

	const char* fragmentsource = fragmentfile->GetBuffer();

	glShaderSourceARB(m_FragmentShaders[m_NumFragmentShaders], 1, &fragmentsource, NULL);
	glCompileShaderARB(m_FragmentShaders[m_NumFragmentShaders]);

	delete[] fragmentsource;	// Fragment source is no longer required,

	int32 result = 0;
	glGetObjectParameterivARB( m_FragmentShaders[m_NumFragmentShaders], GL_OBJECT_COMPILE_STATUS_ARB, &result );
	if( result == false )
	{	
		GLsizei errorLogLength = 0;
		static const GLsizei errorLogMaxLength = 1024;
		static GLchar errorLog[errorLogMaxLength];
		glGetShaderInfoLog(m_FragmentShaders[m_NumFragmentShaders], errorLogMaxLength, &errorLogLength, errorLog);

		LOG(VL_ERROR, "(FRAGMENT) %s\n%s", _Shader, (char*)errorLog);
		return false;
	}

	++m_NumFragmentShaders;
	return true;
}

bool LkGLSLShader::LinkProgram()
{
	if (m_NumVertexShaders == 0 || m_NumFragmentShaders == 0)
	{
		LOG(VL_ERROR, "GLSLShader::LinkProgram: Failed to link, vertex or fragment shader was not compiled succesfully");
		return false;
	}

	// Now that the programs are compiled, they can be attached to a shader program and compiled together.
	m_ProgramHandle = glCreateProgramObjectARB();

	for (uint32 i = 0; i < m_NumVertexShaders; ++i)
	{
		glAttachObjectARB(m_ProgramHandle, m_VertexShaders[i]);
	}
	for (uint32 i = 0; i < m_NumFragmentShaders; ++i)
	{
		glAttachObjectARB(m_ProgramHandle, m_FragmentShaders[i]);
	}

	_BindAttributeLocations();

	glLinkProgramARB(m_ProgramHandle);

// 	int32 test = glGetAttribLocation(m_ProgramHandle, "AttribPosition");
// 	test = glGetAttribLocation(m_ProgramHandle, "AttribNormal");

	int32 result = 0;
	glGetObjectParameterivARB(m_ProgramHandle, GL_LINK_STATUS, &result);
	if ( result == false )
	{
		GLsizei errorLogLength = 0;
		static const GLsizei errorLogMaxLength = 1024;
		static GLchar errorLog[errorLogMaxLength];
		glGetProgramInfoLog(m_ProgramHandle, errorLogMaxLength, &errorLogLength, errorLog);		// NOTE: If program crashes on this line
																								// with an unhandled exception at 0x00000000 error,
																								// make sure you don't have both a gl_FragData[] and a gl_FragColor
																								// assignment in the fragment shader. This has caused the error in the past.
																								// (For some reason).

		std::string test = std::string((char*)errorLog);	// Quickfix.
		LOG(VL_ERROR, "GLSLShader::LinkProgram: %s\0", test.c_str());
		return false;
	}

	LOG(VL_NORMAL, "Successfully linked program (%i vertex, %i fragment shaders)", m_NumVertexShaders, m_NumFragmentShaders);
	return true;
}

// GLhandleARB GLSLShader::GetVertexHandle()
// {
// 	return m_VertexShaders;
// }
// 
// GLhandleARB GLSLShader::GetFragmentHandle()
// {
// 	return m_FragmentShaders;
// }

GLhandleARB LkGLSLShader::GetProgramHandle()
{
	// If this assert fails, the shader has not been succesfully linked yet before this call was made.
	//assert(m_ProgramHandle != 0);

	return m_ProgramHandle;
}

int32 LkGLSLShader::GetUniformLocation( const char* _Name )
{
	return glGetUniformLocation(m_ProgramHandle, _Name);
}

void LkGLSLShader::_BindAttributeLocations()
{
	for (uint32 i = 0; i < LkGLSLShader::ATTRIB_Count; ++i)
	{
		glBindAttribLocation(m_ProgramHandle, i, ATTRIB_LOCATION_NAMES[i]);
	}
}

}

}