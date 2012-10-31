#include "core/game/game.h"

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