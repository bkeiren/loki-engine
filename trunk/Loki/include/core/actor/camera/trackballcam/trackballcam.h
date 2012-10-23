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
	void SetTrackBallDistance( f32 _Distance );
	f32 GetTrackBallDistance() const;

	void SetTrackBallCenter( const vec3& _Center );
	const vec3& GetTrackBallCenter() const;
private:
	LkTrackBallCam( const char* _Name, game::LkLevel* _Level );
	LkTrackBallCam();
	~LkTrackBallCam();

	void _OnEvent( const LkEvent& _Event );

	f32 m_Distance;
	vec3 m_Center;
};

}

#endif