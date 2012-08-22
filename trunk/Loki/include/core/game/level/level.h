#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#define USE_HASH_MAP	// If defined, uses hash_map's to store actors instead of simple maps.
						// Hash maps are in most cases O(1) complexity (Because they cleverly use hash tables)
						// while regular maps are O(log N) complexity in general.

#ifdef USE_HASH_MAP
#include <hash_map>
#define MAP_TYPE	stdext::hash_map
#else
#include <map>
#define MAP_TYPE	std::map
#endif
#include "core/actor/actor.h"

#define CAM_FREE		"FreeCam"
#define CAM_TRACKBALL	"TrackBallCam"
#define CAM_PATH		"PathCam"

namespace loki
{

class LkActor;
class LkPawn;
class LkLight;
class LkPointLight;
class LkSpotLight;
class LkDirectionalLight;
class LkCamera;

namespace game
{

class LkSkyBox;

class LkLevel
{
public:
	typedef MAP_TYPE<ActorID, LkActor*>				Actors;
	typedef std::pair<ActorID, LkActor*>				ActorsPair;

	typedef MAP_TYPE<ActorID, LkPawn*>				Pawns;
	typedef std::pair<ActorID, LkPawn*>				PawnsPair;

	typedef MAP_TYPE<ActorID, LkLight*>				Lights;
	typedef std::pair<ActorID, LkLight*>				LightsPair;

	typedef MAP_TYPE<ActorID, LkPointLight*>			PointLights;
	typedef std::pair<ActorID, LkPointLight*>			PointLightsPair;

	typedef MAP_TYPE<ActorID, LkDirectionalLight*>	DirectionalLights;
	typedef std::pair<ActorID, LkDirectionalLight*>	DirectionalLightsPair;

	typedef MAP_TYPE<ActorID, LkSpotLight*>			SpotLights;
	typedef std::pair<ActorID, LkSpotLight*>			SpotLightsPair;

	typedef MAP_TYPE<ActorID, LkCamera*>				Cameras;
	typedef std::pair<ActorID, LkCamera*>				CamerasPair;

	typedef LkCamera*(*CameraFactory)(const char*, LkLevel*);	// Camera factory function prototype.
	typedef MAP_TYPE<const char*, CameraFactory>	CameraFactories;
	typedef std::pair<const char*, CameraFactory>	CameraFactoriesPair;

	class GameRules
	{
	public:
	private:
	};

	LkLevel();
	~LkLevel();

	//////////////////////////////////////////////////////////////////////////
	// Returns a pointer to the actor if an actor exists, NULL otherwise.
	//////////////////////////////////////////////////////////////////////////
	LkActor* ActorExists( const char* _Name );
	
	LkPointLight* SpawnPointLight( const char* _Name );
	LkPointLight* GetPointLight( const char* _Name );
	const PointLights* GetPointLights();
	void DespawnPointLight( LkPointLight* _Light );

	LkSpotLight* SpawnSpotLight( const char* _Name );
	LkSpotLight* GetSpotLight( const char* _Name );
	const SpotLights* GetSpotLights();
	void DespawnSpotLight( LkSpotLight* _Light );

	LkDirectionalLight* SpawnDirectionalLight( const char* _Name );
	LkDirectionalLight* GetDirectionalLight( const char* _Name );
	const DirectionalLights* GetDirectionalLights();
	void DespawnDirectionalLight( LkDirectionalLight* _Light );

	LkPawn* SpawnPawn( const char* _Name );
	LkPawn* GetPawn( const char* _Name );
	const Pawns* GetPawns();
	void DespawnPawn( LkPawn* _Pawn );

	LkCamera* SpawnCamera( const char* _Name, const char* _Type );
	LkCamera* GetCamera( const char* _Name );
	LkCamera* GetCurrentCamera();
	void SetCurrentCamera( LkCamera* _Camera );
	void DespawnCamera( LkCamera* _Camera );

	//////////////////////////////////////////////////////////////////////////
	// Registers a factory function for a given type (Indicated by _Type).
	// Registered types can be used to spawn cameras by passing the type
	// to SpawnCamera().
	//////////////////////////////////////////////////////////////////////////
	static void RegisterCameraFactory( const char* _Type, CameraFactory _Factory );

	const LkSkyBox* GetSkyBox() const;
private:
	GameRules* m_GameRules;

	Actors m_Actors;	// All actors.

	Pawns m_Pawns;	
	PointLights m_PointLights;
	SpotLights m_SpotLights;
	DirectionalLights m_DirectionalLights;
	Cameras m_Cameras;
	LkCamera* m_CurrentCamera;

	static CameraFactories m_CameraFactories;

	LkSkyBox* m_Skybox;
};

}

}

#endif