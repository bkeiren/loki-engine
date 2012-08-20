#include "core/resourcemanager/modelmanager.h"
#include "core/resourcemanager/resourcemanager.h"
#include "core/renderer/geometry/model/model.h"

namespace loki
{

LkModelManager* g_ModelManager = NULL;

// _LoadResource Mesh specialization.
template<>
renderer::LkModel* LkModelManager::_LoadResource( const char* _Res )
{
	return new renderer::LkModel(_Res);
}


}