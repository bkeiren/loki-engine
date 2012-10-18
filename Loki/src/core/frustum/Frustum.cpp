#include "core/frustum/Frustum.h"

namespace loki
{

Frustum::Frustum( float _Left, float _Right, float _Bottom, float _Top, float _Near, float _Far )
{
	m_FrustumMatrix = math::gtc::matrix_transform::frustum(_Left, _Right, _Bottom, _Top, _Near, _Far);
	_ComputePlaneNormals();
}

Frustum::Frustum( const mat4& _ProjectionMatrix )	:
	m_FrustumMatrix(_ProjectionMatrix)
{
	_ComputePlaneNormals();
}

Frustum::~Frustum()
{

}

const mat4& Frustum::GetMatrix() const
{
	return m_FrustumMatrix;
}

const vec4& Frustum::GetPlaneNormal( EFrustumPlanes _Plane ) const
{
	return m_Normals[_Plane];
}

void Frustum::_ComputePlaneNormals()
{
	mat4 transpose = math::transpose(m_FrustumMatrix);
	m_Normals[FRUSTUM_PLANE_LEFT] = math::normalize(vec4(1.0f, 0.0f, 0.0f, 1.0f) * transpose);
	m_Normals[FRUSTUM_PLANE_RIGHT] = math::normalize(vec4(-1.0f, 0.0f, 0.0f, 1.0f) * transpose);
	m_Normals[FRUSTUM_PLANE_BOTTOM] = math::normalize(vec4(0.0f, 1.0f, 0.0f, 1.0f) * transpose);
	m_Normals[FRUSTUM_PLANE_TOP] = math::normalize(vec4(0.0f, -1.0f, 0.0f, 1.0f) * transpose);
	m_Normals[FRUSTUM_PLANE_NEAR] = math::normalize(vec4(0.0f, 0.0f, 1.0f, 1.0f) * transpose);
	m_Normals[FRUSTUM_PLANE_FAR] = math::normalize(vec4(0.0f, 0.0f, -1.0f, 1.0f) * transpose);
}

}