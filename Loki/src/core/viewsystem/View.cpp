#include "core/viewsystem/View.h"

namespace loki
{

View::View( const char* _Name )	:
	m_Name(std::string(_Name))
{

}

View::View()
{
	ILLEGAL_CTOR_ERROR("View");
}

const std::string& View::GetName() const
{
	return m_Name;
}

}