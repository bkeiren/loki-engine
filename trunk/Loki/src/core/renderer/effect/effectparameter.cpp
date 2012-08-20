#include "core/renderer/effect/effectparameter.h"
#include "Cg/cgGL.h"

namespace loki
{

namespace renderer
{

LkEffectParameter::LkEffectParameter( void* _Parameter, const std::string& _ParameterName )	:
	m_Name(_ParameterName),
	m_CGParameter(_Parameter)
{

}

LkEffectParameter::LkEffectParameter()
{

}

LkEffectParameter::~LkEffectParameter()
{

}

const std::string& LkEffectParameter::GetName() const
{
	return m_Name;
}

void LkEffectParameter::Set( float _P )
{
	cgSetParameter1f((CGparameter)m_CGParameter, _P);
}

void LkEffectParameter::Set( int _P )
{
	cgSetParameter1i((CGparameter)m_CGParameter, _P);
}

void LkEffectParameter::Set( const glm::vec2& _P )
{
	cgSetParameter2fv((CGparameter)m_CGParameter, glm::value_ptr(_P));
}

void LkEffectParameter::Set( const glm::vec3& _P )
{
	cgSetParameter3fv((CGparameter)m_CGParameter, glm::value_ptr(_P));
}

void LkEffectParameter::Set( const glm::vec4& _P )
{
	cgSetParameter4fv((CGparameter)m_CGParameter, glm::value_ptr(_P));
}

void LkEffectParameter::Set( const glm::mat2& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 4, glm::value_ptr(_P));
}

void LkEffectParameter::Set( const glm::mat3& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 9, glm::value_ptr(_P));
}

void LkEffectParameter::Set( const glm::mat4& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 16, glm::value_ptr(_P));
}

void LkEffectParameter::Set( const GLuint _P )
{
	cgGLSetTextureParameter((CGparameter)m_CGParameter, _P);
}

void LkEffectParameter::Get( float* _V )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 1, _V);
}

void LkEffectParameter::Get( int* _V )
{
	cgGetParameterValueic((CGparameter)m_CGParameter, 1, _V);
}

void LkEffectParameter::Get( glm::vec2* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 2, (float*)_P);
}

void LkEffectParameter::Get( glm::vec3* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 3, (float*)_P);
}

void LkEffectParameter::Get( glm::vec4* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 4,(float*) _P);
}

void LkEffectParameter::Get( glm::mat2* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 4,(float*) _P);
}

void LkEffectParameter::Get( glm::mat3* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 9, (float*)_P);
}

void LkEffectParameter::Get( glm::mat4* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 16, (float*)_P);
}

void LkEffectParameter::Get( GLuint* _P )
{
	(*_P) = cgGLGetTextureParameter((CGparameter)m_CGParameter);
}

}

}