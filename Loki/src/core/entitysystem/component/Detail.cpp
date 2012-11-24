#include "core/entitysystem/component/Detail.h"

namespace loki
{

namespace components
{

namespace detail
{

RegistryEntry::RegistryEntry( CreateComponentFunction _Function, const util::general::TypeInfo& _TypeInfo )	:
	m_Function(_Function),
	m_TypeInfo(_TypeInfo)
{

}

}

}

}
