#pragma once

#ifndef PHYSICSDEBUGDRAWER_H
#define PHYSICSDEBUGDRAWER_H

#include "Bullet\LinearMath\btIDebugDraw.h"

namespace loki
{

namespace physics
{

class LkDebugDrawer	: public btIDebugDraw
{
public:
	LkDebugDrawer();
	~LkDebugDrawer();

	void drawLine( const btVector3& from, const btVector3& to, const btVector3& color );
	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int32 lifeTime,const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location,const char* textString);
	void setDebugMode(int32 debugMode);
	void addDebugModeFlag(int32 debugMode);
	void removeDebugModeFlag(int32 debugMode);
	int32	getDebugMode() const;
private:
	int32 m_DebugModeFlags;
};

}

}

#endif PHYSICSDEBUGDRAWER_H