#include "core/game/game.h"
#include "core/game/level/level.h"

#include "core/actor/camera/camera.h"
#include "core/actor/light/point/pointlight.h"
#include "core/actor/light/spot/spotlight.h"
#include "core/actor/light/directional/directionallight.h"
#include "core/actor/handle/handle.h"

namespace loki
{

namespace game
{

LkGame::LkGame()	:
	GameVersionMajor(0),
	GameVersionMinor(0),
	GameVersionBuild(0),
	GameName("<NO TITLE>"),
	m_Level(NULL)
{
	
}

LkGame::~LkGame()
{

}

bool LkGame::LoadLevel( const char* _Level )
{
	return true;
}

LkLevel* LkGame::GetLevel()
{
	return m_Level;
}

}

}