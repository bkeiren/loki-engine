#pragma once

#ifndef FREECAM_H
#define FREECAM_H

#include "core/actor/camera/camera.h"

namespace loki
{

namespace game
{
class LkLevel;
}

LkCamera* CameraFactoryFreeCam( const char* _Name, game::LkLevel* _Level );

class LkFreeCam	: public LkCamera
{
	friend LkCamera* CameraFactoryFreeCam( const char* _Name, game::LkLevel* _Level );
public:
private:
	void _OnEvent( const LkEvent& _Event );

	LkFreeCam( const char* _Name, game::LkLevel* _Level );
	LkFreeCam();
	~LkFreeCam();
};

}

#endif