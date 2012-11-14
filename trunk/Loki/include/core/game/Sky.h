#pragma once

#ifndef SKY_H
#define SKY_H

namespace loki
{

namespace graphics
{
	class TextureCube;
	class DisplayList;
}

namespace renderer
{
	class LkEffect;
}

namespace game
{

class Sky
{
public:
	static void SetCubeMap( graphics::TextureCube* _CubeMap );
	static graphics::TextureCube* GetCubeMap();

	static void Render();
private:
	Sky();
	~Sky();

	static graphics::TextureCube* m_CubeMap;
	static graphics::DisplayList* m_DisplayList;
	static renderer::LkEffect* m_Shader;
};

}

}

#endif