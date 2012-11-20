#pragma once

#ifndef RIGIDBODYINFO_H
#define RIGIDBODYINFO_H

namespace loki
{

namespace graphics
{
class SubMesh;
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

	f32 m_Mass;	// A mass of zero means the object has an infinite mass and is not movable.
	f32 m_Friction;
	f32 m_Restitution;
	mat4 m_InitialTransform;
	mat4 m_CenterOfMassOffset;
	
	vec3 m_LocalInertia;
	f32 m_LinearDamping;
	f32 m_AngularDamping;
	f32 m_LinearSleepingThreshold;
	f32 m_AngularSleepingThreshold;
	bool m_AdditionalDamping;
	f32 m_AdditionalDampingFactor;
	f32 m_AdditionalLinearDampingThresholdSqr;
	f32 m_AdditionalAngularDampingThresholdSqr;
	f32 m_AdditionalAngularDampingFactor;

	//////////////////////////////////////////////////////////////////////////
	// Capsule data.
	struct
	{
		EShapeAxis m_Axis;
		f32 m_Radius;
		f32 m_Height;
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
		f32 m_Radius;
	} m_SphereData;

	//////////////////////////////////////////////////////////////////////////
	// Triangle mesh data.
	struct  
	{
		graphics::SubMesh* m_Mesh;	// TODO: Change this so that we use a mesh loaded with 
								// the express purpose of being used for physics hulls.
	} m_MeshData;

	//////////////////////////////////////////////////////////////////////////
	// (Infinite) Plane data.
	struct  
	{
		vec3 m_Normal;
		f32 m_Constant;	// ?Distance from the origin along the normal?
	} m_StaticPlaneData;

	//////////////////////////////////////////////////////////////////////////
	// Cone data.
	struct
	{
		EShapeAxis m_Axis;
		f32 m_Radius;
		f32 m_Height;
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