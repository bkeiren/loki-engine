#include "core/graphics/Material.h"
#include "core/graphics/Texture2D.h"
#include "core/renderer/effect/effectmanager.h"

#include "util/json/json.h"

#include "core/renderer/effect/effectmanager.h"

namespace loki
{

namespace graphics
{

renderer::LkEffect* Material::m_DefaultEffect = 0;
Material::Materials Material::m_Materials;

Material::Material()	:
	m_Effect(0),
	m_Shininess(50.0f),
	m_Reflectivity(0.0f),
	m_UVScale(vec2(1.0f, 1.0f))
{
	for (uint32 i = 0; i < _TT_COUNT; ++i)
	{
		m_Textures[i] = 0;
	}

	if (!m_DefaultEffect)
	{
		m_DefaultEffect = renderer::g_EffectManager->CreateEffectFromFile(DEFAULT_RESOURCE("shaders//gbuffer_default.cgfx"), "DefaultMaterialEffect");
	}
}

Material::~Material()
{
	for (uint32 i = 0; i < _TT_COUNT; ++i)
	{
		delete m_Textures[i];
	}
}

Material* Material::LoadMaterial( const std::string& _MaterialFile )
{
	Material* material = _FindMaterial(_MaterialFile);
	if (!material)
	{
		material = _CreateMaterialFromLMAFile(_MaterialFile);
	}
	return material;
}

const Texture2D* Material::GetTexture( uint32 _Index ) const
{
	return m_Textures[_Index];
}

void Material::SetTexture( uint32 _Index, Texture2D* _Texture, bool _DeleteOldTexture /*= true*/ )
{
	if (_DeleteOldTexture)
	{
		delete m_Textures[_Index];
	}
	m_Textures[_Index] = _Texture;
}

renderer::LkEffect* Material::GetEffect() const
{
	return m_Effect == 0 ? m_DefaultEffect : m_Effect;
}

void Material::SetEffect( renderer::LkEffect* _Effect )
{
	m_Effect = _Effect;
}

f32 Material::GetShininess() const
{
	return m_Shininess;
}

void Material::SetShininess( f32 _Shininess )
{
	m_Shininess = _Shininess;
}

f32 Material::GetReflectivity() const
{
	return m_Reflectivity;
}

void Material::SetReflectivity( f32 _Reflectivity )
{
	m_Reflectivity = math::clamp(_Reflectivity, 0.0f, 1.0f);
}

const vec2& Material::GetUVScale() const
{
	return m_UVScale;
}

void Material::SetUVScale( const vec2& _Scale )
{
	m_UVScale = _Scale;
}

bool Material::HasDiffuse() const
{
	return m_Textures[TT_DIFFUSE] != 0;
}

bool Material::HasNormal() const
{
	return m_Textures[TT_NORMAL] != 0;
}

bool Material::HasSpecular() const
{
	return m_Textures[TT_SPECULAR] != 0;
}

bool Material::HasEmissive() const
{
	return m_Textures[TT_EMISSIVE] != 0;
}

bool Material::HasEffect() const
{
	return m_Effect != 0;
}

Material* Material::_CreateMaterialFromLMAFile( const std::string& _MaterialFile )
{
	util::general::JSONDocument* doc = util::general::JSONDocument::Open(_MaterialFile);
	if (!doc)
	{
		LOG(VL_ERROR, "Material::_CreateMaterialFromLMAFile: Failed to open LMA file '%s'", _MaterialFile.c_str());
		return 0;
	}

	util::general::JSONValue& root = doc->GetRoot();

	util::general::JSONValue diffuseTexString = root["diffuse"];
	util::general::JSONValue specularTexString = root["specular"];
	util::general::JSONValue normalTexString = root["normal"];
	util::general::JSONValue emissiveTexString = root["emissive"];
	util::general::JSONValue effectString = root["effect"];
	util::general::JSONValue shininessString = root["shininess"];
	util::general::JSONValue reflectivityString = root["reflectivity"];
	// TODO: Uv scale.

	Material* mtl = new Material();

	if (diffuseTexString.IsString())
	{
		mtl->SetTexture(Material::TT_DIFFUSE, Texture2D::Load(diffuseTexString.AsString()));
	}
	if (specularTexString.IsString())
	{
		mtl->SetTexture(Material::TT_SPECULAR, Texture2D::Load(specularTexString.AsString()));
	}
	if (normalTexString.IsString())
	{
		mtl->SetTexture(Material::TT_NORMAL, Texture2D::Load(normalTexString.AsString()));
	}
	if (emissiveTexString.IsString())
	{
		mtl->SetTexture(Material::TT_EMISSIVE, Texture2D::Load(emissiveTexString.AsString()));
	}
	if (effectString.IsString())
	{
		std::stringstream ss;
		ss << _MaterialFile << (rand()%1024);

		mtl->SetEffect(renderer::g_EffectManager->CreateEffectFromFile(effectString.AsString(), ss.str()));
	}
	if (shininessString.IsDouble())
	{
		mtl->SetShininess((f32)shininessString.AsDouble());
	}
	if (reflectivityString.IsDouble())
	{
		mtl->SetReflectivity((f32)reflectivityString.AsDouble());
	}

	JSON_CLOSE(doc);

	m_Materials[_MaterialFile] = mtl;
	return mtl;
}

Material* Material::_FindMaterial( const std::string& _MaterialFile )
{
	MaterialsConstIter it = m_Materials.find(_MaterialFile);
	if (it == m_Materials.end())
	{
		return 0;
	}
	return (*it).second;
}

}

}