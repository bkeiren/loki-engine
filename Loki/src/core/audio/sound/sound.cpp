#include "core/audio/sound/sound.h"
#include "FMOD\\fmod.hpp"
#include "FMOD\\fmod_errors.h"
#include "core/audio/audio_error.h"

namespace loki
{

LkSound::LkSound( FMOD::System* _System, FMOD::Channel* _Channel, const char* _File )	:
	m_Sound(NULL),
	m_System(NULL),
	m_Channel(NULL),
	m_File(_File)
{
    memset(&m_SoundExInfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    m_SoundExInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
 
    FMOD_RESULT result = _System->createSound(_File, FMOD_HARDWARE | FMOD_CREATESTREAM, &m_SoundExInfo, &m_Sound);
    ErrorCheck(result);
}

LkSound::~LkSound()
{
	
}

LkSound::LkSound()
{

}

void LkSound::Play()
{
	m_System->playSound(FMOD_CHANNEL_FREE, m_Sound, false, &m_Channel);
}

void LkSound::Stop()
{
	
}

}