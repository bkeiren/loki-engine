#pragma once

#ifndef SPHERE_H
#define SPHERE_H

namespace loki
{

class Sphere
{
public:
	Sphere( const vec3& _Center, f32 _Radius );
	~Sphere();

	const vec3& GetCenter() const;
	f32 GetRadius() const;
private:
	Sphere();

	vec3 m_Center;
	f32 m_Radius;
};

}

#endif