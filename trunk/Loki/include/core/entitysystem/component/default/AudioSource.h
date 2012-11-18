#pragma once

#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "core/entitysystem/component/Component.h"

namespace loki
{

namespace components
{

class AudioSource	: public Component
{
public:
	DECLARE_COMPONENT(AudioSource)

	AudioSource();
	~AudioSource();

private:
	void _HandleEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();
};

}

}

#endif