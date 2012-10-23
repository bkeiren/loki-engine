#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include "core/audio/sound/sound.h"
#include <list>

// Forward declarations.
namespace FMOD
{
	class System;
	class Sound;
	class Channel;
}

namespace loki
{

class LkAudio
{
	friend class LkEngine;
public:
	void Update();

	LkSound* CreateSound( char* _File );
	//static void PlaySound( Sound* _Sound );
private:
	LkAudio();
	~LkAudio();

	bool _Init();
	void _Shutdown();

	FMOD::System*		m_System;
	FMOD::Sound*			m_Sound;
	FMOD::Channel*		m_Channel;
	int32					m_Key;
	uint32			m_Version;
	std::list<LkSound*>	m_Sounds;
};

extern LkAudio* g_Audio;

}

#endif