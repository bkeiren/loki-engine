#include "core/game/level/level.h"

#include "core/actor/light/point/pointlight.h"
#include "core/actor/light/spot/spotlight.h"
#include "core/actor/light/directional/directionallight.h"
#include "core/actor/pawn/pawn.h"

#include "core/actor/camera/camera.h"
#include "core/actor/camera/freecam/freecam.h"
#include "core/actor/camera/trackballcam/trackballcam.h"
#include "core/actor/camera/pathcam/pathcam.h"

#include "core/game/level/skybox/skybox.h"

#define INSERT_ACTOR(a)	m_Actors.insert(ActorsPair(a->GetID(), a));
#define ERASE_ACTOR(a)	m_Actors.erase(a->GetID());

#define CHECK_IF_EXISTS(funcname)	{if (ActorExists(_Name)){LOG(VL_ERROR, "Level::"funcname": An actor with name '%s' already exists", _Name);return NULL;}}

namespace loki
{

namespace game
{

LkLevel::CameraFactories LkLevel::m_CameraFactories;

LkLevel::LkLevel()	:
	m_CurrentCamera(NULL),
	m_Skybox(new LkSkyBox("resources//textures//mountain_.bmp"))
{
	// Register built-in camera types.
	RegisterCameraFactory(CAM_FREE, &CameraFactoryFreeCam);
	RegisterCameraFactory(CAM_TRACKBALL, &CameraFactoryTrackBallCam);
	RegisterCameraFactory(CAM_PATH, &CameraFactoryPathCam);

	LkCamera* default_cam = SpawnCamera("DefaultCamera", CAM_FREE);
	if (!default_cam)
	{
		LOG(VL_ERROR, "Level::Level: Unable to instantiate default camera");
	}
	SetCurrentCamera(default_cam);
}

LkLevel::~LkLevel()
{
	delete m_Skybox;
}

LkActor* LkLevel::ActorExists( const char* _Name )
{
	unsigned int hash = LkActor::GetHashForName(_Name);
	
	Actors::iterator it = m_Actors.find(hash);
	if (it == m_Actors.end())
	{
		return NULL;
	}

	return (*it).second;
}

LkPointLight* LkLevel::SpawnPointLight( const char* _Name )
{
	if (ActorExists(_Name))
	{
		LOG(VL_ERROR, "Level::SpawnPointLight: An actor with name '%s' already exists", _Name);
		return NULL;
	}

	LkPointLight* light = new LkPointLight(_Name, this);
	m_PointLights.insert(PointLightsPair(light->GetID(), light));
	INSERT_ACTOR(light)

	return light;
}

LkPointLight* LkLevel::GetPointLight( const char* _Name )
{
	unsigned int hash = LkActor::GetHashForName(_Name);
	PointLights::iterator it = m_PointLights.find(hash);
	if (it == m_PointLights.end())
	{
		return NULL;
	}
	return (*it).second;
}

const LkLevel::PointLights* LkLevel::GetPointLights()
{
	return &m_PointLights;
}

void LkLevel::DespawnPointLight( LkPointLight* _Light )
{
	m_PointLights.erase(_Light->GetID());
	ERASE_ACTOR(_Light)
}

LkSpotLight* LkLevel::SpawnSpotLight( const char* _Name )
{
	if (ActorExists(_Name))
	{
		LOG(VL_ERROR, "Level::SpawnSpotLight: An actor with name '%s' already exists", _Name);
		return NULL;
	}

	LkSpotLight* light = new LkSpotLight(_Name, this);
	m_SpotLights.insert(SpotLightsPair(light->GetID(), light));
	INSERT_ACTOR(light)

	return light;
}

LkSpotLight* LkLevel::GetSpotLight( const char* _Name )
{
	unsigned int hash = LkActor::GetHashForName(_Name);
	SpotLights::iterator it = m_SpotLights.find(hash);
	if (it == m_SpotLights.end())
	{
		return NULL;
	}
	return (*it).second;
}

const LkLevel::SpotLights* LkLevel::GetSpotLights()
{
	return &m_SpotLights;
}

void LkLevel::DespawnSpotLight( LkSpotLight* _Light )
{
	m_SpotLights.erase(_Light->GetID());
	ERASE_ACTOR(_Light)
}

LkDirectionalLight* LkLevel::SpawnDirectionalLight( const char* _Name )
{
	if (ActorExists(_Name))
	{
		LOG(VL_ERROR, "Level::SpawnDirectionalLight: An actor with name '%s' already exists", _Name);
		return NULL;
	}

	LkDirectionalLight* light = new LkDirectionalLight(_Name, this);
	m_DirectionalLights.insert(DirectionalLightsPair(light->GetID(), light));
	INSERT_ACTOR(light)

	return light;
}

LkDirectionalLight* LkLevel::GetDirectionalLight( const char* _Name )
{
	unsigned int hash = LkActor::GetHashForName(_Name);
	DirectionalLights::iterator it = m_DirectionalLights.find(hash);
	if (it == m_DirectionalLights.end())
	{
		return NULL;
	}
	return (*it).second;
}

const LkLevel::DirectionalLights* LkLevel::GetDirectionalLights()
{
	return &m_DirectionalLights;
}

void LkLevel::DespawnDirectionalLight( LkDirectionalLight* _Light )
{
	m_DirectionalLights.erase(_Light->GetID());
	ERASE_ACTOR(_Light)
}

LkPawn* LkLevel::SpawnPawn( const char* _Name )
{
	CHECK_IF_EXISTS("SpawnPawn");

	LkPawn* pawn = new LkPawn(_Name, this);
	m_Pawns.insert(PawnsPair(pawn->GetID(), pawn));
	INSERT_ACTOR(pawn)

	return pawn;
}

LkPawn* LkLevel::GetPawn( const char* _Name )
{
	unsigned int hash = LkActor::GetHashForName(_Name);
	Pawns::iterator it = m_Pawns.find(hash);
	if (it == m_Pawns.end())
	{
		return NULL;
	}
	return (*it).second;
}

const LkLevel::Pawns* LkLevel::GetPawns()
{
	return &m_Pawns;
}

void LkLevel::DespawnPawn( LkPawn* _Pawn )
{
	m_Pawns.erase(_Pawn->GetID());
	ERASE_ACTOR(_Pawn)

	delete _Pawn;
}

LkCamera* LkLevel::SpawnCamera( const char* _Name, const char* _Type )
{
	if (ActorExists(_Name))
	{
		LOG(VL_ERROR, "Level::SpawnCamera: An actor with name '%s' already exists", _Name);
		return NULL;
	}

	CameraFactories::iterator it = m_CameraFactories.find(_Type);
	if (it == m_CameraFactories.end())
	{
		LOG(VL_ERROR, "Level::SpawnCamera: Unable to find camera factory for type %s", _Type);
		return NULL;
	}
	LkCamera* cam = ((*it).second)(_Name, this);
	m_Cameras.insert(CamerasPair(cam->GetID(), cam));
	INSERT_ACTOR(cam)

	return cam;
}

LkCamera* LkLevel::GetCamera( const char* _Name )
{
	unsigned int hash = LkActor::GetHashForName(_Name);
	Cameras::iterator it = m_Cameras.find(hash);
	if (it == m_Cameras.end())
	{
		return NULL;
	}
	return (*it).second;
}

LkCamera* LkLevel::GetCurrentCamera()
{
	return m_CurrentCamera;
}

void LkLevel::SetCurrentCamera( LkCamera* _Camera )
{
	assert(_Camera != NULL);
	m_CurrentCamera = _Camera;
}

void LkLevel::DespawnCamera( LkCamera* _Camera )
{
	m_Cameras.erase(_Camera->GetID());
	ERASE_ACTOR(_Camera)
}

void LkLevel::RegisterCameraFactory( const char* _Type, CameraFactory _Factory )
{
	if (m_CameraFactories.insert(CameraFactoriesPair(_Type, _Factory)).second == false)
	{
		LOG(VL_WARN, "Level::RegisterCameraFactory: Factory for camera type '%s' has already been registered", _Type);
	}
}

const LkSkyBox* LkLevel::GetSkyBox() const
{
	return m_Skybox;
}

}

}