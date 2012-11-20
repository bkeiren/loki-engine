#include "core/graphics/effect/EffectParameter.h"
#include "Cg/cgGL.h"

namespace loki
{

namespace graphics
{

EffectParameter::EffectParameter( void* _Parameter, const std::string& _ParameterName )	:
	m_Name(_ParameterName),
	m_CGParameter(_Parameter)
{

}

EffectParameter::EffectParameter()
{

}

EffectParameter::~EffectParameter()
{

}

const std::string& EffectParameter::GetName() const
{
	return m_Name;
}

void EffectParameter::Set( f32 _P )
{
	cgSetParameter1f((CGparameter)m_CGParameter, _P);
}

void EffectParameter::Set( int32 _P )
{
	cgSetParameter1i((CGparameter)m_CGParameter, _P);
}

void EffectParameter::Set( const vec2& _P )
{
	cgSetParameter2fv((CGparameter)m_CGParameter, math::value_ptr(_P));
}

void EffectParameter::Set( const vec3& _P )
{
	cgSetParameter3fv((CGparameter)m_CGParameter, math::value_ptr(_P));
}

void EffectParameter::Set( const vec4& _P )
{
	cgSetParameter4fv((CGparameter)m_CGParameter, math::value_ptr(_P));
}

void EffectParameter::Set( const mat2& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 4, math::value_ptr(_P));
}

void EffectParameter::Set( const mat3& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 9, math::value_ptr(_P));
}

void EffectParameter::Set( const mat4& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 16, math::value_ptr(_P));
}

void EffectParameter::Set( const GLuint _P )
{
	cgGLSetTextureParameter((CGparameter)m_CGParameter, _P);
}

void EffectParameter::Get( f32* _V )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 1, _V);
}

void EffectParameter::Get( int32* _V )
{
	cgGetParameterValueic((CGparameter)m_CGParameter, 1, _V);
}

void EffectParameter::Get( vec2* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 2, (f32*)_P);
}

void EffectParameter::Get( vec3* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 3, (f32*)_P);
}

void EffectParameter::Get( vec4* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 4,(f32*) _P);
}

void EffectParameter::Get( mat2* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 4,(f32*) _P);
}

void EffectParameter::Get( mat3* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 9, (f32*)_P);
}

void EffectParameter::Get( mat4* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 16, (f32*)_P);
}

void EffectParameter::Get( GLuint* _P )
{
	(*_P) = cgGLGetTextureParameter((CGparameter)m_CGParameter);
}

}

}