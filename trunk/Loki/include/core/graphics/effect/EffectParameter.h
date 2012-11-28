#pragma once

#ifndef EFFECTPARAMETER_H
#define EFFECTPARAMETER_H

#include "GL/gl.h"

#include <string>

namespace loki
{

namespace graphics
{

class EffectParameter
{
	friend class Effect;
	friend class EffectManager;
public:
	//////////////////////////////////////////////////////////////////////////
	// Get this parameter's name.
	const std::string& GetName() const;

	//////////////////////////////////////////////////////////////////////////
	// Set functions.
	void Set( f32 _P );
	void Set( int32 _P );
	void Set( const vec2& _P );
	void Set( const vec3& _P );
	void Set( const vec4& _P );
	void Set( const mat2& _P );
	void Set( const mat3& _P );
	void Set( const mat4& _P );
	void Set( const GLuint _P );

	//////////////////////////////////////////////////////////////////////////
	// Get functions.
	void Get( f32* _V );
	void Get( int32* _V );
	void Get( vec2* _P );
	void Get( vec3* _P );
	void Get( vec4* _P );
	void Get( mat2* _P );
	void Get( mat3* _P );
	void Get( mat4* _P );
	void Get( GLuint* _P );
private:
	EffectParameter( void* _Parameter, const std::string& _ParameterName );
	EffectParameter();
	~EffectParameter();

	std::string m_Name;
	void* m_CGParameter;
};

}

}

#endif