#include "core/audio/audio.h"
#include "FMOD\\fmod.h"
#include "FMOD\\fmod.hpp"
#include "FMOD\\fmod_errors.h"
#include "FMOD\\fmod_codec.h"
#include "FMOD\\fmod_dsp.h"
#include "FMOD\\fmod_memoryinfo.h"
#include "FMOD\\fmod_output.h"
#include "core/audio/audio_error.h"

namespace loki
{

LkAudio* g_Audio = NULL;

// FMOD::System* Audio::m_System = NULL;
// FMOD::Sound* Audio::m_Sound = NULL;
// FMOD::Channel* Audio::m_Channel = NULL;
// int Audio::m_Key = 0;
// unsigned int Audio::m_Version = 0;
// std::list<Sound*> Audio::m_Sounds;

LkAudio::LkAudio()	:
	m_System(NULL),
	m_Sound(NULL),
	m_Channel(NULL),
	m_Key(0),
	m_Version(0)
{
	_Init();
}

LkAudio::~LkAudio()
{
	_Shutdown();
}

bool LkAudio::_Init()
{
	// http://www.fmod.org/wiki/index.php5?title=Midi_example_in_C

	FMOD_RESULT result = FMOD_OK;

    result = FMOD::System_Create(&m_System);
    ErrorCheck(result);

    result = m_System->getVersion(&m_Version);
    ErrorCheck(result);

    if (m_Version < FMOD_VERSION)
    {
		LOG(VL_ERROR, "FMOD: Version %08x is being used. Version %08x is required (Is fmodex.dll outdated?).", m_Version, FMOD_VERSION);
        INIT_FAIL("Audio");
    }

    result = m_System->init(32, FMOD_INIT_NORMAL, NULL);
    ErrorCheck(result);

	LOG(VL_ALWAYS, "Audio::Init: Audio initialized (FMOD v%i.%i.%i)", (FMOD_VERSION >> 4) & 0x0000ffff,
																	  (FMOD_VERSION >> 2) & 0x000000ff,
																	  (FMOD_VERSION & 0x000000ff));
	return true;
}

void LkAudio::_Shutdown()
{
	FMOD_RESULT result = FMOD_OK;
	result = m_Sound->release();
	ErrorCheck(result);
	result = m_System->release();
	ErrorCheck(result);

	LOG(VL_ALWAYS, "Audio::Shutdown: Done");
}

void LkAudio::Update()
{
	m_System->update();
}

LkSound* LkAudio::CreateSound( char* _File )
{
	LkSound* sound = new LkSound(m_System, m_Channel, _File);
	m_Sounds.push_back(sound);
	return sound;
}

// void Audio::PlaySound( Sound* _Sound )
// {
// 	
// }

}