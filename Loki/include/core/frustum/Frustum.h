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
		FP_LEFT = 0,
		FP_RIGHT,
		FP_BOTTOM,
		FP_TOP,
		FP_NEAR,
		FP_FAR,

		_FP_COUNT
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
	vec4 m_Normals[_FP_COUNT];
};

}

#endif