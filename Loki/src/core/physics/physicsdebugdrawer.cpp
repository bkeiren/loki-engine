#include "core/physics/physics.inl"
#include "core/physics/physicsdebugdrawer.h"
#include "core/renderer/debugrenderer.h"

namespace loki
{

namespace physics
{

LkDebugDrawer::LkDebugDrawer()	:
	m_DebugModeFlags(0)
{
	setDebugMode(DBG_DrawWireframe /*| DBG_FastWireframe*/ | DBG_DrawAabb | DBG_DrawConstraints | DBG_DrawConstraintLimits);

// 	DBG_DrawAabb=2,
// 		DBG_DrawFeaturesText=4,
// 		DBG_DrawContactPoints=8,
// 		DBG_NoDeactivation=16,
// 		DBG_NoHelpText = 32,
// 		DBG_DrawText=64,
// 		DBG_ProfileTimings = 128,
// 		DBG_EnableSatComparison = 256,
// 		DBG_DisableBulletLCP = 512,
// 		DBG_EnableCCD = 1024,
// 		DBG_DrawConstraints = (1 << 11),
// 		DBG_DrawConstraintLimits = (1 << 12),
// 		DBG_FastWireframe = (1<<13),
// 		DBG_DrawNormals = (1<<14),
}

LkDebugDrawer::~LkDebugDrawer()
{

}

void LkDebugDrawer::drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
{
	renderer::debug::DrawLine3D(GLMVec3(from), GLMVec3(to), true, GLMVec3(color));
}

void LkDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
	vec3 col = GLMVec3(color);
	renderer::debug::DrawSphere(GLMVec3(PointOnB), 0.1f, true, col);
	renderer::debug::DrawLine3D(GLMVec3(PointOnB), GLMVec3(PointOnB) + (GLMVec3(normalOnB) * distance), true, col);
}

void LkDebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG(VL_WARN, "Physics DebugDrawer: %s", warningString);
}

void LkDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{

}

void LkDebugDrawer::setDebugMode(int debugMode)
{
	m_DebugModeFlags = debugMode;
}

void LkDebugDrawer::addDebugModeFlag(int debugMode)
{
	m_DebugModeFlags |= debugMode;
}

void LkDebugDrawer::removeDebugModeFlag(int debugMode)
{
	m_DebugModeFlags ^= debugMode;	// Is this correct?
}

int	LkDebugDrawer::getDebugMode() const
{
	return m_DebugModeFlags;
}

}

}