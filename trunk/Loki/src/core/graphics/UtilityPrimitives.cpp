#include "core/graphics/UtilityPrimitives.h"
#include <GLEW\\glew.h>

namespace loki
{

namespace graphics
{

namespace
{


void IcoSphereSubdivisionHelper( f32 _Radius, int32 _Subdivision, int32 _MaxSubdivisions, const vec3& _V0, const vec3& _V1, const vec3& _V2 )
{
	//////////////////////////////////////////////////////////////////////////
	//       V0
	//      /  \
	//     /    \
	//    V01___V02
	//   / \    / \
	//  /   \  /   \
	// V1____V12____V2
	//////////////////////////////////////////////////////////////////////////

	// Calculate 3 new vertices that lie half-way on each edge.
	// The vertices are not exactly half-way since they are scaled so that they are of the proper length so
	// we forming an icosphere and not simply a subdivided icosahedron.
	vec3 V01 = math::normalize(_V0 + _V1);
	vec3 V02 = math::normalize(_V0 + _V2);
	vec3 V12 = math::normalize(_V1 + _V2);

	// Subdivide each of the 4 new triangles again if required.
	if (_Subdivision < _MaxSubdivisions)
	{
		IcoSphereSubdivisionHelper(_Radius, _Subdivision + 1, _MaxSubdivisions, _V0, V01, V02);
		IcoSphereSubdivisionHelper(_Radius, _Subdivision + 1, _MaxSubdivisions, V01, _V1, V12);
		IcoSphereSubdivisionHelper(_Radius, _Subdivision + 1, _MaxSubdivisions, V01, V12, V02);
		IcoSphereSubdivisionHelper(_Radius, _Subdivision + 1, _MaxSubdivisions, V02, V12, _V2);
	}
	else
	{
		// Draw the 4 triangles since we don't need to subdivide more.

#define TRIDRAWHELPER(v0, v1, v2)	{	vec3 V0 = v0 * _Radius;	\
										vec3 V1 = v1 * _Radius;	\
										vec3 V2 = v2 * _Radius;	\
										glVertex3f( V0.x, V0.y, V0.z );	\
										glVertex3f( V1.x, V1.y, V1.z );	\
										glVertex3f( V2.x, V2.y, V2.z );	}

		TRIDRAWHELPER(_V0, V01, V02)
		TRIDRAWHELPER(V01, _V1, V12)
		TRIDRAWHELPER(V01, V12, V02)
		TRIDRAWHELPER(V02, V12, _V2)

#undef TRIDRAWHELPER
	}
}

}

void DrawCone( f32 _Base, f32 _Height, int32 _Slices )
{
	static const int MaxSlices = 127;
	static float X[MaxSlices + 1];
	static float Y[MaxSlices + 1];
	if (_Slices > MaxSlices)
	{
		_Slices = MaxSlices;
	}
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);	// Top of the cone.
	float a = math::radians(360.0f / _Slices);
	for (int32 i = _Slices; i >= 0; --i)
	{
		X[i] = cos(a * i) * _Base;
		Y[i] = sin(a * i) * _Base;

		glVertex3f(X[i], Y[i], _Height);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, _Height);
	for (int32 i = 0; i <= _Slices; ++i)	// Reversed order.
	{
		glVertex3f(X[i], Y[i], _Height);
	}
	glEnd();
}

void DrawIcoSphere( f32 _Radius, int32 _Subdivisions /*= 1*/ )
{
	static const float t = (float)((1.0 + math::sqrt(5.0)) / 2.0);	// Golden ratio calculation.
	static vec3 BaseVertices[12] = {	vec3(-1.0f,  t,  0.0f),
		vec3( 1.0f,  t,  0.0f),
		vec3(-1.0f, -t,  0.0f),
		vec3( 1.0f, -t,  0.0f),

		vec3( 0.0f, -1.0f,  t),
		vec3( 0.0f,  1.0f,  t),
		vec3( 0.0f, -1.0f, -t),
		vec3( 0.0f,  1.0f, -t),

		vec3( t,  0.0f, -1.0f),
		vec3( t,  0.0f,  1.0f),
		vec3(-t,  0.0f, -1.0f),
		vec3(-t,  0.0f,  1.0f)	};
	static int3 BaseFaceIndices[20] = {	// 5 faces around point 0
		int3(0, 11, 5),
		int3(0, 5, 1),
		int3(0, 1, 7),
		int3(0, 7, 10),
		int3(0, 10, 11),
		// 5 adjacent faces
		int3(1, 5, 9),
		int3(5, 11, 4),
		int3(11, 10, 2),
		int3(10, 7, 6),
		int3(7, 1, 8),
		// 5 faces around point 3
		int3(3, 9, 4),
		int3(3, 4, 2),
		int3(3, 2, 6),
		int3(3, 6, 8),
		int3(3, 8, 9),
		// 5 adjacent faces
		int3(4, 9, 5),
		int3(2, 4, 11),
		int3(6, 2, 10),
		int3(8, 6, 7),
		int3(9, 8, 1) };

	glBegin(GL_TRIANGLES);
	for (int32 face = 0; face < 20; ++face)
	{
		int3 Indices = BaseFaceIndices[face];

		// Instead outputting the base face, the face is subdivided into 4 triangles for each subdivision level.

		vec3 v0 = math::normalize(BaseVertices[Indices.x]);
		vec3 v1 = math::normalize(BaseVertices[Indices.y]);
		vec3 v2 = math::normalize(BaseVertices[Indices.z]);
		IcoSphereSubdivisionHelper(_Radius, 0, _Subdivisions, v0, v1, v2);
	}
	glEnd();
}

}

}
