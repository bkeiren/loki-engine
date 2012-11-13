#include "core/entitysystem/component/default/Light.h"
#include <GLEW\\glew.h>
#include "core/graphics/Texture2D.h"
#include "core/graphics/DisplayList.h"

namespace loki
{

namespace components
{

namespace
{


void _IcoSphereSubdivisionHelper( f32 _Radius, int32 _Subdivision, int32 _MaxSubdivisions, const vec3& _V0, const vec3& _V1, const vec3& _V2 )
{
	//////////////////////////////////////////////////////////////////////////
	//       V0
	//      /  \
	//     /    \
	//    V01___V02
	//   / \    / \
	//  /   \  /   \
	// V1____V12____V2
	//////////////////////////////////////////////////////////////////////////

	// Calculate 3 new vertices that lie half-way on each edge.
	// The vertices are not exactly half-way since they are scaled so that they are of the proper length so
	// we forming an icosphere and not simply a subdivided icosahedron.
	vec3 V01 = math::normalize(_V0 + _V1);
	vec3 V02 = math::normalize(_V0 + _V2);
	vec3 V12 = math::normalize(_V1 + _V2);

	// Subdivide each of the 4 new triangles again if required.
	if (_Subdivision < _MaxSubdivisions)
	{
		_IcoSphereSubdivisionHelper(_Radius, _Subdivision + 1, _MaxSubdivisions, _V0, V01, V02);
		_IcoSphereSubdivisionHelper(_Radius, _Subdivision + 1, _MaxSubdivisions, V01, _V1, V12);
		_IcoSphereSubdivisionHelper(_Radius, _Subdivision + 1, _MaxSubdivisions, V01, V12, V02);
		_IcoSphereSubdivisionHelper(_Radius, _Subdivision + 1, _MaxSubdivisions, V02, V12, _V2);
	}
	else
	{
		// Draw the 4 triangles since we don't need to subdivide more.

#define TRIDRAWHELPER(v0, v1, v2)	{	vec3 V0 = v0 * _Radius;	\
										vec3 V1 = v1 * _Radius;	\
										vec3 V2 = v2 * _Radius;	\
										glVertex3f( V0.x, V0.y, V0.z );	\
										glVertex3f( V1.x, V1.y, V1.z );	\
										glVertex3f( V2.x, V2.y, V2.z );	}

		TRIDRAWHELPER(_V0, V01, V02)
		TRIDRAWHELPER(V01, _V1, V12)
		TRIDRAWHELPER(V01, V12, V02)
		TRIDRAWHELPER(V02, V12, _V2)

#undef TRIDRAWHELPER
	}
}

}

Light::Lights Light::m_Lights;
Light::Lights Light::m_LightsByType[];
graphics::Texture2D* Light::m_PointAttenuationTexture = 0;
graphics::Texture2D* Light::m_SpotAttenuationTexture = 0;

Light::Light()	:
	m_LightType(LIGHT_POINT)
	,m_Range(10.0f)
	,m_Geometry(0)
	,m_GeometryIsDirty(true)
	,m_SpotAngle(25.0f)
	,m_CookieSize(1.0f)
	,m_AreaSize(vec2(1.0f, 1.0f))
	,m_Color(ColorRGB(1.0f, 1.0f, 1.0f))
	,m_Intensity(1.0f)
	,m_ShadowType(SHADOWS_NONE)
	,m_Cookie(0)
{
	SetSpotAngle(30.0f);

	if (!m_PointAttenuationTexture)
	{
		m_PointAttenuationTexture = graphics::Texture2D::Load("resources//textures//PointLightAttenuation.bmp");
		if (!m_PointAttenuationTexture)
		{
			LOG(VL_ERROR, "Light::Light: Failed to load pointlight attenuation texture");
		}
		else
		{
			m_PointAttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_S, graphics::CLAMP_TO_EDGE);
			m_PointAttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_T, graphics::CLAMP_TO_EDGE);
		}
	}
	if (!m_SpotAttenuationTexture)
	{
		m_SpotAttenuationTexture = graphics::Texture2D::Load("resources//textures//SpotLightAttenuation.bmp");
		if (!m_SpotAttenuationTexture)
		{
			LOG(VL_ERROR, "Light::Light: Failed to load spotlight attenuation texture");
		}
		else
		{
			m_SpotAttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_S, graphics::CLAMP_TO_EDGE);
			m_SpotAttenuationTexture->SetTextureParameter(graphics::TEXTURE_WRAP_T, graphics::CLAMP_TO_EDGE);
		}
	}

	m_Geometry = graphics::DisplayList::Create(1);
	if (!m_Geometry)
	{
		LOG(VL_ERROR, "Light::Light: Failed to create display list for light geometry");
	}
}

Light::~Light()
{
	delete m_Geometry;
}

Light::Lights& Light::GetAllLights()
{
	return m_Lights;
}

Light::Lights& Light::GetAllLightsByType( ELightType _Type )
{
	return m_LightsByType[_Type];
}

graphics::Texture2D* Light::GetPointAttenuationTexture()
{
	return m_PointAttenuationTexture;
}

graphics::Texture2D* Light::GetSpotAttenuationTexture()
{
	return m_SpotAttenuationTexture;
}

Light::ELightType Light::GetLightType() const
{
	return m_LightType;
}

void Light::SetLightType( ELightType _Type )
{
	if (_Type == m_LightType)
	{
		return;
	}

	m_LightsByType[m_LightType].remove(this);
	m_LightType = _Type;
	m_LightsByType[m_LightType].push_back(this);

	m_GeometryIsDirty = true;
}

f32 Light::GetRange() const
{
	return m_Range;
}

void Light::SetRange( f32 _Range )
{
	m_Range = _Range;

	m_GeometryIsDirty = true;
}

const ColorRGB& Light::GetColor() const
{
	return m_Color;
}

void Light::SetColor( const ColorRGB& _Color )
{
	m_Color = _Color;
}

f32 Light::GetIntensity() const
{
	return m_Intensity;
}

void Light::SetIntensity( f32 _Intensity )
{
	m_Intensity = math::clamp(_Intensity, 0.0f, 8.0f);
}

Light::EShadowType Light::GetShadowType() const
{
	return m_ShadowType;
}

void Light::SetShadowType( EShadowType _ShadowType )
{
	m_ShadowType = _ShadowType;
}

f32 Light::GetSpotAngle() const
{
	return m_SpotAngle;
}

void Light::SetSpotAngle( f32 _Angle )
{
	m_SpotAngle = math::clamp(_Angle, 1.0f, 179.0f);
	m_SpotBaseTangent = math::tan(math::radians(m_SpotAngle * 0.5f));
	
	m_GeometryIsDirty = true;
}

f32 Light::GetSpotBaseRadius() const
{
	return GetRange() * m_SpotBaseTangent;
}

graphics::DisplayList* Light::GetGeometry()
{
	// Update light geometry if required.
	if (m_GeometryIsDirty)
	{
		_GenerateGeometry();
		m_GeometryIsDirty = false;
	}
	return m_Geometry;
}

void Light::_HandleEvent( const LkEvent& _Event )
{
	
}

void Light::_Init()
{
	m_Lights.push_back(this);
}

void Light::_Terminate()
{
	m_Lights.remove(this);
	m_LightsByType[GetLightType()].remove(this);
}

void Light::_GenerateGeometry()
{
	m_Geometry->BeginList();
		
	switch (m_LightType)
	{
	case LIGHT_POINT:
		{
			_DrawPointLightIcoSphere(GetRange(), 1);
			break;
		}
	case LIGHT_SPOT:
		{
			_DrawSpotLightCone(GetSpotBaseRadius(), GetRange(), 20);
			break;
		}
	default:
		{
			static const char* LightTypesStrings[4] = {"LIGHT_POINT", "LIGHT_SPOT", "LIGHT_DIRECTIONAL", "LIGHT_AREA"};
			LOG(VL_WARN, "Light::_GenerateGeometry: No geometry to generate for a light of type %s", LightTypesStrings[m_LightType]);
			break;
		}
	}

	m_Geometry->EndList();
}

void Light::_DrawSpotLightCone( f32 _Base, f32 _Height, int32 _Slices )
{
	static const int MaxSlices = 127;
	static float X[MaxSlices + 1];
	static float Y[MaxSlices + 1];
	if (_Slices > MaxSlices)
	{
		_Slices = MaxSlices;
	}
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);	// Top of the cone.
	float a = math::radians(360.0f / _Slices);
	for (int32 i = 0; i <= _Slices; ++i)
	{
		X[i] = cos(a * i) * _Base;
		Y[i] = sin(a * i) * _Base;

		glVertex3f(X[i], Y[i], _Height);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, _Height);
	for (int32 i = _Slices; i >= 0; --i)	// Reversed order.
	{
		glVertex3f(X[i], Y[i], _Height);
	}
	glEnd();
}

void Light::_DrawPointLightIcoSphere( f32 _Radius, int32 _Subdivisions /*= 1*/ )
{
	static const float t = (float)((1.0 + math::sqrt(5.0)) / 2.0);	// Golden ratio calculation.
	static vec3 BaseVertices[12] = {	vec3(-1.0f,  t,  0.0f),
										vec3( 1.0f,  t,  0.0f),
										vec3(-1.0f, -t,  0.0f),
										vec3( 1.0f, -t,  0.0f),

										vec3( 0.0f, -1.0f,  t),
										vec3( 0.0f,  1.0f,  t),
										vec3( 0.0f, -1.0f, -t),
										vec3( 0.0f,  1.0f, -t),

										vec3( t,  0.0f, -1.0f),
										vec3( t,  0.0f,  1.0f),
										vec3(-t,  0.0f, -1.0f),
										vec3(-t,  0.0f,  1.0f)	};
	static int3 BaseFaceIndices[20] = {	// 5 faces around point 0
										int3(0, 11, 5),
										int3(0, 5, 1),
										int3(0, 1, 7),
										int3(0, 7, 10),
										int3(0, 10, 11),
										// 5 adjacent faces
										int3(1, 5, 9),
										int3(5, 11, 4),
										int3(11, 10, 2),
										int3(10, 7, 6),
										int3(7, 1, 8),
										// 5 faces around point 3
										int3(3, 9, 4),
										int3(3, 4, 2),
										int3(3, 2, 6),
										int3(3, 6, 8),
										int3(3, 8, 9),
										// 5 adjacent faces
										int3(4, 9, 5),
										int3(2, 4, 11),
										int3(6, 2, 10),
										int3(8, 6, 7),
										int3(9, 8, 1) };

	glBegin(GL_TRIANGLES);
	for (int32 face = 0; face < 20; ++face)
	{
		int3 Indices = BaseFaceIndices[face];

		// Instead outputting the base face, the face is subdivided into 4 triangles for each subdivision level.

		vec3 v0 = math::normalize(BaseVertices[Indices.x]);
		vec3 v1 = math::normalize(BaseVertices[Indices.y]);
		vec3 v2 = math::normalize(BaseVertices[Indices.z]);
		_IcoSphereSubdivisionHelper(_Radius, 0, _Subdivisions, v0, v1, v2);
	}
	glEnd();
}

}

}