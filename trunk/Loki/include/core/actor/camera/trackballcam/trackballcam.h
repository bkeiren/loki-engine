#pragma once

#ifndef TRACKBALLCAM_H
#define TRACKBALLCAM_H

#include "core/actor/camera/camera.h"

namespace loki
{

namespace game
{
class LkLevel;
}

LkCamera* CameraFactoryTrackBallCam( const char* _Name, game::LkLevel* _Level );

class LkTrackBallCam	: public LkCamera
{
	friend LkCamera* CameraFactoryTrackBallCam( const char* _Name, game::LkLevel* _Level );
public:
	void SetTrackBallDistance( float _Distance );
	float GetTrackBallDistance() const;

	void SetTrackBallCenter( const glm::vec3& _Center );
	const glm::vec3& GetTrackBallCenter() const;
private:
	LkTrackBallCam( const char* _Name, game::LkLevel* _Level );
	LkTrackBallCam();
	~LkTrackBallCam();

	void _OnEvent( const LkEvent& _Event );

	float m_Distance;
	glm::vec3 m_Center;
};

}

#endif