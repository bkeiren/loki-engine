#include "core/physics/physics.inl"

#include "core/physics/rigidbody.h"

#include "Bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
//#include "Bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btBoxShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btSphereShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btConeShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btCylinderShape.h"
#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"
#include "Bullet/BulletCollision/CollisionShapes/btCompoundShape.h"

#include "Bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"

#include "Bullet/BulletCollision/Gimpact/btGImpactShape.h"

#include "Bullet/LinearMath/btDefaultMotionState.h"

//#include "core/renderer/geometry/vertex/vertex.h"
//#include "core/renderer/geometry/mesh/mesh.h"
//#include "core/renderer/geometry/submesh/submesh.h"

#include "core/graphics/IndexBuffer.h"
#include "core/graphics/VertexBuffer.h"
#include "core/graphics/Mesh.h"

namespace loki
{

namespace physics
{

LkRigidBody::LkRigidBody( const RigidBodyInfo& _Info )	:
	m_Shape(_Info.m_Shape)
{
	btCollisionShape* shape = NULL;
	switch (m_Shape)
	{
	case CS_MESH_TRIANGLE_MESH:
		{
			if (_Info.m_MeshData.m_Mesh == NULL)
			{
				LOG(VL_WARN, "RigidBody::RigidBody: Geometric data required to build a triangle mesh is missing, using a normalized box hull instead");
				// We don't break here because we want to roll over into the CS_BOX case because our triangle mesh is missing an actual
				// mesh, so we don't have any geometric data.
			}
			else
			{
#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))

				btTriangleIndexVertexArray* meshinterface = new btTriangleIndexVertexArray();

// 				for (unsigned int i = 0; i < _Info.m_MeshData.m_Mesh->GetNumSubMeshes(); ++i)
// 				{
// 					const graphics::Mesh* mesh = _Info.m_MeshData.m_Mesh->GetSubMesh(i);
// 					
// 					btIndexedMesh btmesh;
// 
// 					btmesh.m_numTriangles = mesh->GetIndexBufferObject()->GetNumIndices() / 3;
// 					btmesh.m_numVertices = mesh->GetVertexBufferObject()->GetNumVertices();
// 					btmesh.m_triangleIndexBase = (unsigned char*)mesh->GetIndexBufferObject()->GetIndicesRAM();
// 					btmesh.m_triangleIndexStride = 3 * sizeof(int);
// 					btmesh.m_vertexBase = (unsigned char*)((int)mesh->GetVertexBufferObject()->GetVerticesRAM() + (int)MEMBER_OFFSET(renderer::LkVertex, pos));
// 					btmesh.m_vertexStride = sizeof(renderer::LkVertex);
// 
// 					meshinterface->addIndexedMesh(btmesh, PHY_INTEGER);
// 				}

				// Is this static?
				shape = new btBvhTriangleMeshShape(meshinterface, true, true);
				break;
			}
		}
	case CS_MESH_CONVEXHULL:
		{
			shape = new btCompoundShape();

// 			for (unsigned int i = 0; i < _Info.m_MeshData.m_Mesh->GetNumSubMeshes(); ++i)
// 			{
// 				const graphics::Mesh* mesh = _Info.m_MeshData.m_Mesh->GetSubMesh(i);
// 				mat4 submeshtransform;	// Identity, for now?
// 
// 				// The original, non-reduced, shape.
// 				// A btConvexHullShape simply takes a point cloud (No triangles are defined) and from this constructs
// 				// a basic shape primitive that most tightly fits this cloud (Which could be a box, sphere, cylinder, etc.).
// 				btConvexHullShape* original = new btConvexHullShape((btScalar*)((int)mesh->GetVertexBufferObject()->GetVerticesRAM() + (int)MEMBER_OFFSET(renderer::LkVertex, pos)), 
// 																	mesh->GetVertexBufferObject()->GetNumVertices(),
// 																	sizeof(renderer::LkVertex));
// 				btShapeHull* hull = new btShapeHull(original);
// 
// 				// Build a new hull with less vertices. We do this because we don't want to have to construct a shape for a huge number
// 				// of vertices when only some of these will be of any actual affect to the final shape.
// 				// The Bullet documentation says that the number of vertices should be ideally kept below 100...
// 				hull->buildHull(original->getMargin());
// 
// 				// Store the hull.
// 				((btCompoundShape*)shape)->addChildShape(BTTransform(submeshtransform), new btConvexHullShape((btScalar*)hull->getVertexPointer(), hull->numVertices()));
// 			}

			//delete original;	// Required?
			break;
		}
	case CS_MESH_CONVEXTRIANGLEMESH:
		{
#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))

			btTriangleIndexVertexArray* meshinterface = new btTriangleIndexVertexArray();

// 			for (unsigned int i = 0; i < _Info.m_MeshData.m_Mesh->GetNumSubMeshes(); ++i)
// 			{
// 				const graphics::Mesh* mesh = _Info.m_MeshData.m_Mesh->GetSubMesh(i);
// 
// 				btIndexedMesh btmesh;
// 
// 				btmesh.m_numTriangles = mesh->GetIndexBufferObject()->GetNumIndices() / 3;
// 				btmesh.m_numVertices = mesh->GetVertexBufferObject()->GetNumVertices();
// 				btmesh.m_triangleIndexBase = (unsigned char*)mesh->GetIndexBufferObject()->GetIndicesRAM();
// 				btmesh.m_triangleIndexStride = 3 * sizeof(int);
// 				btmesh.m_vertexBase = (unsigned char*)((int)mesh->GetVertexBufferObject()->GetVerticesRAM() + (int)MEMBER_OFFSET(renderer::LkVertex, pos));
// 				btmesh.m_vertexStride = sizeof(renderer::LkVertex);
// 
// 				meshinterface->addIndexedMesh(btmesh, PHY_INTEGER);
// 			}

			shape = new btConvexTriangleMeshShape(meshinterface, true);
			//shape = new btGImpactMeshShape(meshinterface);
			break;
		}
	case CS_BOX:
		{
			shape = new btBoxShape(BTVec3(_Info.m_BoxData.m_HalfExtents));
			break;
		}
	case CS_SPHERE:
		{
			shape = new btSphereShape(_Info.m_SphereData.m_Radius);
			break;
		}
	case CS_CAPSULE:
		{
			switch (_Info.m_CapsuleData.m_Axis)
			{
			case SA_X:
				{
					shape = new btCapsuleShapeX(_Info.m_CapsuleData.m_Radius, _Info.m_CapsuleData.m_Height);
					break;
				}
			case SA_Y:
				{
					shape = new btCapsuleShape(_Info.m_CapsuleData.m_Radius, _Info.m_CapsuleData.m_Height);
					break;
				}
			case SA_Z:
				{
					shape = new btCapsuleShapeZ(_Info.m_CapsuleData.m_Radius, _Info.m_CapsuleData.m_Height);
					break;
				}
			}
			break;
		}
	case CS_STATICPLANE:
		{
			shape = new btStaticPlaneShape(BTVec3(_Info.m_StaticPlaneData.m_Normal), _Info.m_StaticPlaneData.m_Constant);
			break;
		}
	case CS_CONE:
		{
			switch (_Info.m_ConeData.m_Axis)
			{
			case SA_X:
				{
					shape = new btConeShapeX(_Info.m_ConeData.m_Radius, _Info.m_ConeData.m_Height);
					break;
				}
			case SA_Y:
				{
					shape = new btConeShape(_Info.m_ConeData.m_Radius, _Info.m_ConeData.m_Height);
					break;
				}
			case SA_Z:
				{
					shape = new btConeShapeZ(_Info.m_ConeData.m_Radius, _Info.m_ConeData.m_Height);
					break;
				}
			}
			break;
		}
	case CS_CYLINDER:
		{
			switch (_Info.m_ConeData.m_Axis)
			{
			case SA_X:
				{
					shape = new btCylinderShapeX(BTVec3(_Info.m_CylinderData.m_HalfExtents));
					break;
				}
			case SA_Y:
				{
					shape = new btCylinderShape(BTVec3(_Info.m_CylinderData.m_HalfExtents));
					break;
				}
			case SA_Z:
				{
					shape = new btCylinderShapeZ(BTVec3(_Info.m_CylinderData.m_HalfExtents));
					break;
				}
			}
			break;
		}
	}

	btVector3 inertia;
	shape->calculateLocalInertia(_Info.m_Mass, inertia);

	btMotionState* motionstate = new btDefaultMotionState(BTTransform(_Info.m_InitialTransform), BTTransform(_Info.m_CenterOfMassOffset));
	btRigidBody::btRigidBodyConstructionInfo info = btRigidBody::btRigidBodyConstructionInfo(_Info.m_Mass, motionstate, shape, inertia);
	info.m_friction								= _Info.m_Friction;
	info.m_restitution							= _Info.m_Restitution;
	info.m_linearDamping						= _Info.m_LinearDamping;
	info.m_angularDamping						= _Info.m_AngularDamping;
	info.m_linearSleepingThreshold				= _Info.m_LinearSleepingThreshold;
	info.m_angularSleepingThreshold				= _Info.m_AngularSleepingThreshold;
	info.m_additionalDamping					= _Info.m_AdditionalDamping;
	info.m_additionalDampingFactor				= _Info.m_AdditionalDampingFactor;
	info.m_additionalLinearDampingThresholdSqr	= _Info.m_AdditionalLinearDampingThresholdSqr;
	info.m_additionalAngularDampingThresholdSqr = _Info.m_AdditionalAngularDampingThresholdSqr;
	info.m_additionalAngularDampingFactor		= _Info.m_AdditionalAngularDampingFactor;

	m_RigidBody = new btRigidBody(info);
}

LkRigidBody::LkRigidBody()
{
	ILLEGAL_CTOR_ERROR("RigidBody");
}

LkRigidBody::~LkRigidBody()
{
	delete m_RigidBody;
	// Delete collision shape?
}

void LkRigidBody::SetPosition( const vec3& _Position, bool _PreserveForces /*= false*/ )
{
	mat4 trans = GLMMat(m_RigidBody->getCenterOfMassTransform());
	trans[3] = vec4(_Position, 0.0f);
	m_RigidBody->setCenterOfMassTransform(BTTransform(trans));

	if (!_PreserveForces)
	{
		m_RigidBody->clearForces();
	}

	m_RigidBody->activate(true);
}

vec3 LkRigidBody::GetPosition() const
{
	return GLMVec3(m_RigidBody->getCenterOfMassPosition());
}

void LkRigidBody::SetOrientation( const quat& _Orientation, bool _PreserveForces /*= false*/ )
{
	mat4 m = math::gtc::quaternion::mat4_cast(_Orientation);
	m[3] = vec4(GLMVec3(m_RigidBody->getCenterOfMassPosition()), 1.0f);
	m_RigidBody->setCenterOfMassTransform(BTTransform(m));

	if (!_PreserveForces)
	{
		m_RigidBody->clearForces();
	}

	m_RigidBody->activate(true);
}

void LkRigidBody::SetTransformation( const mat4& _Transformation, bool _PreserveForces /*= false*/ )
{
	m_RigidBody->setCenterOfMassTransform(BTTransform(_Transformation));

	if (!_PreserveForces)
	{
		m_RigidBody->clearForces();
	}

	m_RigidBody->activate(true);
}

void LkRigidBody::ApplyCentralForce( const vec3& _Force )
{
	m_RigidBody->applyCentralForce(BTVec3(_Force));
}

void LkRigidBody::ApplyCentralImpulse( const vec3& _Impulse )
{
	m_RigidBody->applyCentralImpulse(BTVec3(_Impulse));
}

void LkRigidBody::ApplyDamping( float _TimeStep )
{
	m_RigidBody->applyDamping(_TimeStep);
}

void LkRigidBody::ApplyForce( const vec3& _Force, const vec3& _Point )
{
	m_RigidBody->applyForce(BTVec3(_Force), BTVec3(_Point));
}

void LkRigidBody::ApplyGravity()
{
	m_RigidBody->applyGravity();
}

void LkRigidBody::ApplyImpulse( const vec3& _Impulse, const vec3& _Point )
{
	m_RigidBody->applyImpulse(BTVec3(_Impulse), BTVec3(_Point));
}

void LkRigidBody::ApplyTorque( const vec3& _Torque )
{
	m_RigidBody->applyTorque(BTVec3(_Torque));
}

void LkRigidBody::ApplyTorqueImpulse( const vec3& _Torque )
{
	m_RigidBody->applyTorqueImpulse(BTVec3(_Torque));
}

void LkRigidBody::ClearForces()
{
	m_RigidBody->clearForces();
}

void LkRigidBody::GetAABB( vec3& _AABBMin, vec3& _AABBMax ) const
{
	btVector3 aabbmin, aabbmax;
	m_RigidBody->getAabb(aabbmin, aabbmax);
	_AABBMin = GLMVec3(aabbmin);
	_AABBMax = GLMVec3(aabbmax);
}

vec3 LkRigidBody::GetCenterOfMass() const
{
	return GLMVec3(m_RigidBody->getCenterOfMassPosition());
}

vec3 LkRigidBody::GetDeltaAngularVelocity() const
{
	return GLMVec3(m_RigidBody->getDeltaAngularVelocity());
}

vec3 LkRigidBody::GetDeltaLinearVelocity() const
{
	return GLMVec3(m_RigidBody->getDeltaLinearVelocity());
}

float LkRigidBody::GetFriction() const
{
	return m_RigidBody->getFriction();
}

vec3 LkRigidBody::GetGravity() const
{
	return GLMVec3(m_RigidBody->getGravity());
}

float LkRigidBody::GetLinearDamping() const
{
	return m_RigidBody->getLinearDamping();
}

vec3 LkRigidBody::GetLinearFactor() const
{
	return GLMVec3(m_RigidBody->getLinearFactor());
}

vec3 LkRigidBody::GetLinearVelocity() const
{
	return GLMVec3(m_RigidBody->getLinearVelocity());
}

quat LkRigidBody::GetOrientation() const
{
	return GLMQuat(m_RigidBody->getOrientation());
}

float LkRigidBody::GetRestitution() const
{
	return m_RigidBody->getRestitution();
}

vec3 LkRigidBody::GetTotalForce() const
{
	return GLMVec3(m_RigidBody->getTotalForce());
}

vec3 LkRigidBody::GetTotalTorque() const
{
	return GLMVec3(m_RigidBody->getTotalTorque());
}

vec3 LkRigidBody::GetVelocityInLocalPoint( const vec3& _Point ) const
{
	return GLMVec3(m_RigidBody->getVelocityInLocalPoint(BTVec3(_Point)));
}

mat4 LkRigidBody::GetWorldTransform() const
{
	return GLMMat(m_RigidBody->getWorldTransform());
}

void LkRigidBody::Translate( const vec3& _Translation )
{
	m_RigidBody->translate(BTVec3(_Translation));
}

bool LkRigidBody::IsActive() const
{
	return m_RigidBody->isActive();
}

void LkRigidBody::Activate( bool _ForceActivation /*= false*/ )
{
	m_RigidBody->activate(_ForceActivation);
}

void LkRigidBody::SetMass( float _Mass )
{
	btVector3 inertia;
	m_RigidBody->getCollisionShape()->calculateLocalInertia(_Mass, inertia);
	m_RigidBody->setMassProps(_Mass, inertia);
}

}

}