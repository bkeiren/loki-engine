#include "core/game/level/skybox/skybox.h"
//#include "core/renderer/texture/texture.h"
#include "core/graphics/Texture.h"
#include "core/resourcemanager/texturemanager.h"

namespace loki
{

namespace game
{

LkSkyBox::LkSkyBox( const std::string& _Skybox )
{
	int i = _Skybox.find_last_of('.');
	std::string s = _Skybox.substr(0, i);
	std::string ext = _Skybox.substr(i);
	std::string paths[6];
	paths[0] = paths[1] = paths[2] = paths[3] = paths[4] = paths[5] = s;

	paths[0] += "w";
	paths[1] += "e";
	paths[2] += "u";
	paths[3] += "d";
	paths[4] += "s";
	paths[5] += "n";

	for (int i = 0; i < 6; ++i)
	{
		paths[i] += ext;
		//m_Textures[i] = g_TextureManager->GetResource(paths[i].c_str());
		m_Textures[i] = graphics::Texture::Load(paths[i]);

		if (m_Textures[i])
		{
			m_Textures[i]->SetTextureParameter(graphics::TEXTURE_WRAP_S, graphics::CLAMP_TO_EDGE);
		}
	}
}

LkSkyBox::LkSkyBox()
{
	ILLEGAL_CTOR_ERROR("SkyBox");
}

LkSkyBox::~LkSkyBox()
{
	for (int i = 0; i < 6; ++i)
	{
		//g_TextureManager->ReleaseResource(&(m_Textures[i]));
		delete m_Textures[i];
	}
}

graphics::Texture* LkSkyBox::GetTexture( ESkyBoxSide _Side ) const
{
	return m_Textures[_Side];
}

}

}