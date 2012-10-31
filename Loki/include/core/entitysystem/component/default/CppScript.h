#pragma once

#ifndef CPPSCRIPT_H
#define CPPSCRIPT_H

#include "core/entitysystem/component/Component.h"

namespace loki
{

namespace components
{

class CppScript	: public Component
{
public:
	// We don't use the DECLARE_COMPONENT_TYPEINFO here because the CppScript class is an abstract class.

	CppScript();
	virtual ~CppScript() = 0;

protected:
	// Called when the script awakes.
	virtual void Awake() {}

	// Called on each update.
	virtual void Update() {}

	// Called when the script is stopped.
	virtual void Stop() {}

private:
	void _OnEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();
};

}

}

#endif