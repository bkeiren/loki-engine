#pragma once

#ifndef EFFECTPARAMETER_H
#define EFFECTPARAMETER_H

#include "GL/gl.h"

#include <string>

namespace loki
{

namespace renderer
{

class LkEffectParameter
{
	friend class LkEffect;
public:
	//////////////////////////////////////////////////////////////////////////
	// Get this parameter's name.
	const std::string& GetName() const;

	//////////////////////////////////////////////////////////////////////////
	// Set functions.
	void Set( float _P );
	void Set( int _P );
	void Set( const glm::vec2& _P );
	void Set( const glm::vec3& _P );
	void Set( const glm::vec4& _P );
	void Set( const glm::mat2& _P );
	void Set( const glm::mat3& _P );
	void Set( const glm::mat4& _P );
	void Set( const GLuint _P );

	//////////////////////////////////////////////////////////////////////////
	// Get functions.
	void Get( float* _V );
	void Get( int* _V );
	void Get( glm::vec2* _P );
	void Get( glm::vec3* _P );
	void Get( glm::vec4* _P );
	void Get( glm::mat2* _P );
	void Get( glm::mat3* _P );
	void Get( glm::mat4* _P );
	void Get( GLuint* _P );
private:
	LkEffectParameter( void* _Parameter, const std::string& _ParameterName );
	LkEffectParameter();
	~LkEffectParameter();

	std::string m_Name;
	void* m_CGParameter;
};

}

}

#endif