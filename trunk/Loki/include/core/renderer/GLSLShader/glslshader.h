#pragma once

#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include "GLEW\\glew.h"

namespace loki
{

namespace renderer
{

// The maximum number of objects of both types (vertex and fragment) that can be attached
// to a program. Simply a define so that there's a hard limit instead of having to work 
// with dynamic arrays.
#define GLSLSHADER_MAX_OBJECTS	5

extern const char* ATTRIB_LOCATION_NAMES[];

class LkGLSLShader
{
public:
	enum ATTRIB_LOCATIONS
	{
		//AL_START_INDEX = 0,	// 0 is invalid.

		ATTRIB_Position = 0,
		ATTRIB_Normal,
		ATTRIB_Tangent,
		ATTRIB_BiTangent,
		ATTRIB_Texcoord,
		ATTRIB_ZFar,
		ATTRIB_ZNear,

		ATTRIB_Count
	};

	LkGLSLShader();
	~LkGLSLShader();

	//////////////////////////////////////////////////////////////////////////
	// Binds the shader program to be used by following draw calls.
	//////////////////////////////////////////////////////////////////////////
	void Activate() const;

	//////////////////////////////////////////////////////////////////////////
	// Unbinds the shader program. NOTE: Does not restore the previously bound
	// shader. If the previous shader must be used, you can also just
	// activate that one without de-activating this shader.
	//////////////////////////////////////////////////////////////////////////
	static void Deactivate();

	//bool LoadFromFile( const char* _Vertex, const char* _Fragment );
	//////////////////////////////////////////////////////////////////////////
	// AttachVertexShader and AttachFragmentShader attempt to load,
	// attach and compile vertex or fragment shaders, respectively.
	// When the vertex and fragment shaders for a program have been
	// attached and compiled, LinkProgram() can be called to 
	// finalize the creation of the program object.
	// NOTE: If attaching multiple shaders, each type (vertex and shader)
	// can have only one main() function in any shader.
	//////////////////////////////////////////////////////////////////////////
	bool AttachVertexShader( const char* _Shader );
	bool AttachFragmentShader( const char* _Shader );

	//////////////////////////////////////////////////////////////////////////
	// Links all attached and compiled vertex and fragment shaders.
	//////////////////////////////////////////////////////////////////////////
	bool LinkProgram();

// 	GLhandleARB GetVertexHandle();
// 	GLhandleARB GetFragmentHandle();
	GLhandleARB GetProgramHandle();

	int32 GetUniformLocation( const char* _Name );
private:
	//////////////////////////////////////////////////////////////////////////
	// Binds attributes to fixed locations in the shader.
	// The ATTRIB_LOCATIONS enum and the string array ATTRIB_LOCATION_NAMES
	// indicate which type of vertex attributes are bound to which variable
	// names in the shader.
	//////////////////////////////////////////////////////////////////////////
	void _BindAttributeLocations();

	GLhandleARB m_VertexShaders[GLSLSHADER_MAX_OBJECTS];
	GLhandleARB m_FragmentShaders[GLSLSHADER_MAX_OBJECTS];
	GLhandleARB m_ProgramHandle;
	uint32 m_NumVertexShaders;
	uint32 m_NumFragmentShaders;
	char* m_VertexPaths[GLSLSHADER_MAX_OBJECTS];
	char* m_FragmentPaths[GLSLSHADER_MAX_OBJECTS];
};

}

}

#endif // GLSLSHADER_H