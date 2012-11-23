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
	friend class ::loki::Entity;
public:
	AudioSource();
	~AudioSource();

private:
	void _HandleEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();
};

}

}

REGISTER_COMPONENT(AudioSource)

#endif