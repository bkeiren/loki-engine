#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <list>

namespace loki
{

class LkActor;
class LkLight;
class LkCamera;

namespace renderer
{

class LkModel;

class LkScene
{
public:
	LkScene();
	virtual ~LkScene();

	void AddActor( LkActor* _Actor );
	void AddLight( LkLight* _Light );
	void AddCamera( LkCamera* _Camera );
	void AddModel( LkModel* _Model );

	LkCamera* const GetCurrentCamera();

	const std::list<LkModel*>* GetModels();
private:
	std::list<LkActor*> m_Actors;
	std::list<LkLight*> m_Lights;
	std::list<LkCamera*> m_Cameras;
	std::list<LkModel*> m_Models;

	LkCamera* m_CurrentCamera;
};

}

}

#endif