#include "core/sphere/Sphere.h"

namespace loki
{

Sphere::Sphere()
{
	ILLEGAL_CTOR_ERROR("Sphere")
}

Sphere::Sphere( const vec3& _Center, f32 _Radius )	:
	 m_Center(_Center)
	,m_Radius(_Radius)
{

}
	
Sphere::~Sphere()
{

}

const vec3& Sphere::GetCenter() const
{
	return m_Center;
}

f32 Sphere::GetRadius() const
{
	return m_Radius;
}

}
