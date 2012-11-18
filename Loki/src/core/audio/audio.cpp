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

namespace audio
{

Audio* g_Audio = NULL;

// FMOD::System* Audio::m_System = NULL;
// FMOD::Sound* Audio::m_Sound = NULL;
// FMOD::Channel* Audio::m_Channel = NULL;
// int32 Audio::m_Key = 0;
// uint32 Audio::m_Version = 0;
// std::list<Sound*> Audio::m_Sounds;

Audio::Audio()	:
	m_System(NULL),
	m_Channel(NULL),
	m_Key(0),
	m_Version(0)
{
	_Init();
}

Audio::~Audio()
{
	_Shutdown();
}

bool Audio::_Init()
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

void Audio::_Shutdown()
{
	FMOD_RESULT result = FMOD_OK;
	result = m_System->release();
	ErrorCheck(result);

	LOG(VL_ALWAYS, "Audio::Shutdown: Done");
}

void Audio::_Update()
{
	m_System->update();
}

}

}