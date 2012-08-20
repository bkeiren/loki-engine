#pragma once

#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "core/resourcemanager/resourcemanager.h"
#include "core/renderer/geometry/model/model.h"

namespace loki
{

typedef LkResourceManager<renderer::LkModel*>		LkModelManager;

template<>
renderer::LkModel* LkModelManager::_LoadResource( const char* _Res );

extern LkModelManager* g_ModelManager;

}

#endif