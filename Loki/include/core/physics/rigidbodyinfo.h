#pragma once

#ifndef RIGIDBODYINFO_H
#define RIGIDBODYINFO_H

namespace loki
{

namespace graphics
{
class Mesh;
}

namespace physics
{

enum ECollisionShape
{
	CS_MESH_TRIANGLE_MESH = 0,	// Should be used mostly for static geometry. Dynamic mesh objects should use CS_MESH_CONVEXHULL or CS_MESH_CONVEXTRIANGLEMESH.
	CS_MESH_CONVEXHULL,			// Defined by a cloud of vertices, the shape itself is the smallest convex shape that encloses the vertices.
	CS_MESH_CONVEXTRIANGLEMESH,		// Same as CS_MESH_CONVEXHULL except the shape is defined exactly by the vertices. Less efficient than CS_MESH_CONVEXHULL.
	CS_BOX,
	CS_SPHERE,
	CS_CAPSULE,
	CS_STATICPLANE,
	CS_CONE,
	CS_CYLINDER
};

enum EShapeAxis
{
	SA_X = 0,
	SA_Y,
	SA_Z
};

struct RigidBodyInfo
{
	RigidBodyInfo();

	ECollisionShape m_Shape;

	float m_Mass;	// A mass of zero means the object has an infinite mass and is not movable.
	float m_Friction;
	float m_Restitution;
	mat4 m_InitialTransform;
	mat4 m_CenterOfMassOffset;
	
	vec3 m_LocalInertia;
	float m_LinearDamping;
	float m_AngularDamping;
	float m_LinearSleepingThreshold;
	float m_AngularSleepingThreshold;
	bool m_AdditionalDamping;
	float m_AdditionalDampingFactor;
	float m_AdditionalLinearDampingThresholdSqr;
	float m_AdditionalAngularDampingThresholdSqr;
	float m_AdditionalAngularDampingFactor;

	//////////////////////////////////////////////////////////////////////////
	// Capsule data.
	struct
	{
		EShapeAxis m_Axis;
		float m_Radius;
		float m_Height;
	} m_CapsuleData;

	//////////////////////////////////////////////////////////////////////////
	// Box data.
	struct  
	{
		vec3 m_HalfExtents;
	} m_BoxData;

	//////////////////////////////////////////////////////////////////////////
	// Sphere data.
	struct  
	{
		float m_Radius;
	} m_SphereData;

	//////////////////////////////////////////////////////////////////////////
	// Triangle mesh data.
	struct  
	{
		graphics::Mesh* m_Mesh;	// TODO: Change this so that we use a mesh loaded with 
								// the express purpose of being used for physics hulls.
	} m_MeshData;

	//////////////////////////////////////////////////////////////////////////
	// (Infinite) Plane data.
	struct  
	{
		vec3 m_Normal;
		float m_Constant;	// ?Distance from the origin along the normal?
	} m_StaticPlaneData;

	//////////////////////////////////////////////////////////////////////////
	// Cone data.
	struct
	{
		EShapeAxis m_Axis;
		float m_Radius;
		float m_Height;
	} m_ConeData;

	//////////////////////////////////////////////////////////////////////////
	// Cylinder data.
	struct  
	{
		vec3 m_HalfExtents;
	} m_CylinderData;
};

}

}

#endif