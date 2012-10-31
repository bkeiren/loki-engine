#include "core\renderer\scene\scene.h"

using namespace loki;
using namespace loki::renderer;

LkScene::LkScene()
{

}

LkScene::~LkScene()
{

}

void LkScene::AddActor( LkActor* _Entity )
{
	m_Actors.push_back(_Entity);
}

void LkScene::AddLight( LkLight* _Light )
{
	m_Lights.push_back(_Light);
}

void LkScene::AddCamera( LkCamera* _Camera )
{
	m_Cameras.push_back(_Camera);
}

void LkScene::AddModel( LkModel* _Model )
{
	m_Models.push_back(_Model);
}

LkCamera* const LkScene::GetCurrentCamera()
{
	return m_CurrentCamera;
}

const std::list<LkModel*>* LkScene::GetModels()
{
	return &m_Models;
}