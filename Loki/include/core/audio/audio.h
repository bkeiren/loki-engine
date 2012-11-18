#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include <list>

// Forward declarations.
namespace FMOD
{
	class System;
	class Channel;
}

namespace loki
{

namespace components
{
	class AudioSource;
	class AudioListener;
}

namespace audio
{

class Audio
{
	friend class LokiEngine;

	CONTAINER_MACRO_LIST(components::AudioSource*, Sources)
	CONTAINER_MACRO_LIST(components::AudioListener*, Listeners)
public:
	

private:
	Audio();
	~Audio();

	void _Update();

	bool _Init();
	void _Shutdown();

	FMOD::System*		m_System;
	FMOD::Channel*		m_Channel;
	int32				m_Key;
	uint32				m_Version;

	Sources m_Sources;
	Listeners m_Listeners;
};

extern Audio* g_Audio;

}

}

#endif