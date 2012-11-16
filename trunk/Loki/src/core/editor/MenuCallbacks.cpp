#include "core/editor/MenuCallbacks.h"

namespace loki
{

void NewSceneCallback()
{
	LOG(VL_NORMAL, "Creating new scene");
}

void NewMaterialCallback()
{
	LOG(VL_NORMAL, "Creating new material");
}

void NewModelCallback()
{
	LOG(VL_NORMAL, "Creating new model");
}

void EmptyEntityCallback()
{
	LOG(VL_NORMAL, "Creating new empty entity");
}

}