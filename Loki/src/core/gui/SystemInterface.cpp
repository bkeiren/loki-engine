#include <Rocket/Core/systeminterface.h>
#include "core//gui//SystemInterface.h"
#include "core/time/Time.h"

namespace loki
{

namespace gui
{

SystemInterface::SystemInterface()
{

}

SystemInterface::~SystemInterface()
{

}

float SystemInterface::GetElapsedTime()
{
	return g_Time->GetGlobalTime();
}

bool SystemInterface::LogMessage( Rocket::Core::Log::Type _Type, const Rocket::Core::String& _Message )
{
	VerbosityLevel Verbosity[6] = { VL_ALWAYS,	// Rocket::Core::Log::Type::LT_ALWAYS
									VL_ERROR,	// Rocket::Core::Log::Type::LT_ERROR
									VL_ERROR,	// Rocket::Core::Log::Type::LT_ASSERT
									VL_WARN,	// Rocket::Core::Log::Type::LT_WARNING
									VL_NORMAL,	// Rocket::Core::Log::Type::LT_INFO
									VL_NORMAL	// Rocket::Core::Log::Type::LT_DEBUG
									};
	LOG(Verbosity[(int)_Type], "LibRocket: %s", _Message.CString());

	return (_Type != Rocket::Core::Log::Type::LT_ASSERT);
}

}

}