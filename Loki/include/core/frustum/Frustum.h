#pragma once

#ifndef FRUSTUM_H
#define FRUSTUM_H

namespace loki
{

class Frustum
{
public:
	enum EFrustumPlanes
	{
		FRUSTUM_PLANE_LEFT = 0,
		FRUSTUM_PLANE_RIGHT,
		FRUSTUM_PLANE_BOTTOM,
		FRUSTUM_PLANE_TOP,
		FRUSTUM_PLANE_NEAR,
		FRUSTUM_PLANE_FAR,

		_FRUSTUM_PLANE_COUNT
	};
	
	Frustum( float _Left, float _Right, float _Bottom, float _Top, float _Near, float _Far );
	Frustum( const mat4& _ProjectionMatrix );
	~Frustum();

	const mat4& GetMatrix() const;

	//////////////////////////////////////////////////////////////////////////
	// Normals point inward.
	//////////////////////////////////////////////////////////////////////////
	const vec4& GetPlaneNormal( EFrustumPlanes _Plane ) const;
private:
	Frustum();

	void _ComputePlaneNormals();

	mat4 m_FrustumMatrix;
	vec4 m_Normals[_FRUSTUM_PLANE_COUNT];
};

}

#endif