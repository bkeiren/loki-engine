#pragma once

#ifndef SYSTEM_INTERFACE_H
#define SYSTEM_INTERFACE_H

// We don't include the header file for Rocket::Core::SystemInterface here because
// it is included anyway in the GUI files were we use this class.
// Besides, you shouldn't have to include this file manually anyway.

namespace loki
{

namespace gui
{

//////////////////////////////////////////////////////////////////////////
// LibRocket requires an implementation of the Rocket::Core::SystemInterface
// class.
//////////////////////////////////////////////////////////////////////////
class SystemInterface	: public Rocket::Core::SystemInterface
{
	friend class GUI;
public:
	float GetElapsedTime();

	bool LogMessage( Rocket::Core::Log::Type _Type, const Rocket::Core::String& _Message );
private:
	SystemInterface();
	~SystemInterface();
};

}

}

#endif