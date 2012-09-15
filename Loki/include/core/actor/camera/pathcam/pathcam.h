#pragma once

#ifndef PATHCAM_H
#define PATHCAM_H

#include "core/actor/camera/camera.h"

#include <vector>

namespace loki
{

namespace game
{
class LkLevel;
}

LkCamera* CameraFactoryPathCam( const char* _Name, game::LkLevel* _Level );

class LkPathCam	: public LkCamera
{
	friend LkCamera* CameraFactoryPathCam( const char* _Name, game::LkLevel* _Level );
public:

private:
	LkPathCam( const char* _Name, game::LkLevel* _Level );
	LkPathCam();
	~LkPathCam();

	void _OnEvent( const LkEvent& _Event );

	std::vector<vec3> m_PathControlPoints;
	float m_PathPosition;
	unsigned int m_PathSegment;
};

}

#endif