#include "core/resourcemanager/resource/resource.h"

namespace loki
{

LkResource::LkResource( const char* _Name )	:
	m_Name(std::string(_Name)),
	m_RefCount(0)
{

}

LkResource::LkResource()
{
	LOG(VL_ERROR, "Resource::Resource: Class was instantiated using default c-tor. Did you inherit from this class without explicitly specifying in your own class which c-tor to call?");
	ILLEGAL_CTOR_ERROR("Resource");
}

LkResource::~LkResource()
{

}

}