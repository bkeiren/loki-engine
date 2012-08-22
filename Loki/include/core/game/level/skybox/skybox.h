#pragma once

#ifndef SKYBOX_H
#define	SKYBOX_H

namespace loki
{

namespace renderer
{

class LkTexture;

}

namespace game
{

enum ESkyBoxSide
{
	SBS_WEST = 0,
	SBS_EAST,
	SBS_UP,
	SBS_DOWN,
	SBS_SOUTH,
	SBS_NORTH
};

class LkSkyBox
{
public:
	//////////////////////////////////////////////////////////////////////////
	// Example usage:
	// _Skybox equals 'sky.bmp' when all texture files are called 'sky_#.bmp' 
	// where # equals 'east', 'west', 'up', 'down', 'south' and 'north'.
	//////////////////////////////////////////////////////////////////////////
	LkSkyBox( const std::string& _Skybox );
	~LkSkyBox();

	renderer::LkTexture* GetTexture( ESkyBoxSide _Side ) const;
private:
	LkSkyBox();

	renderer::LkTexture* m_Textures[6];
};

}

}

#endif