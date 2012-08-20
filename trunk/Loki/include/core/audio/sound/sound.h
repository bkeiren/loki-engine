#pragma once

#ifndef SOUND_H
#define SOUND_H

#include "FMOD\\fmod.hpp"
#include <string>

namespace FMOD
{
	class System;
	class Channel;
	class LkSound;
}

namespace loki
{

class LkSound
{
public:
	void Play();
	void Stop();
private:
	friend class LkAudio;

	LkSound( FMOD::System* _System, FMOD::Channel* _Channel, const char* _File );
	virtual ~LkSound();
	LkSound();

	FMOD::Sound* m_Sound;
	FMOD::System* m_System;
	FMOD::Channel* m_Channel;
	FMOD_CREATESOUNDEXINFO m_SoundExInfo;
	std::string m_File;
};

}

#endif