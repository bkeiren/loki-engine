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

void LkEffectParameter::Set( f32 _P )
{
	cgSetParameter1f((CGparameter)m_CGParameter, _P);
}

void LkEffectParameter::Set( int32 _P )
{
	cgSetParameter1i((CGparameter)m_CGParameter, _P);
}

void LkEffectParameter::Set( const vec2& _P )
{
	cgSetParameter2fv((CGparameter)m_CGParameter, math::value_ptr(_P));
}

void LkEffectParameter::Set( const vec3& _P )
{
	cgSetParameter3fv((CGparameter)m_CGParameter, math::value_ptr(_P));
}

void LkEffectParameter::Set( const vec4& _P )
{
	cgSetParameter4fv((CGparameter)m_CGParameter, math::value_ptr(_P));
}

void LkEffectParameter::Set( const mat2& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 4, math::value_ptr(_P));
}

void LkEffectParameter::Set( const mat3& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 9, math::value_ptr(_P));
}

void LkEffectParameter::Set( const mat4& _P )
{
	cgSetParameterValuefc((CGparameter)m_CGParameter, 16, math::value_ptr(_P));
}

void LkEffectParameter::Set( const GLuint _P )
{
	cgGLSetTextureParameter((CGparameter)m_CGParameter, _P);
}

void LkEffectParameter::Get( f32* _V )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 1, _V);
}

void LkEffectParameter::Get( int32* _V )
{
	cgGetParameterValueic((CGparameter)m_CGParameter, 1, _V);
}

void LkEffectParameter::Get( vec2* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 2, (f32*)_P);
}

void LkEffectParameter::Get( vec3* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 3, (f32*)_P);
}

void LkEffectParameter::Get( vec4* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 4,(f32*) _P);
}

void LkEffectParameter::Get( mat2* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 4,(f32*) _P);
}

void LkEffectParameter::Get( mat3* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 9, (f32*)_P);
}

void LkEffectParameter::Get( mat4* _P )
{
	cgGetParameterValuefc((CGparameter)m_CGParameter, 16, (f32*)_P);
}

void LkEffectParameter::Get( GLuint* _P )
{
	(*_P) = cgGLGetTextureParameter((CGparameter)m_CGParameter);
}

}

}