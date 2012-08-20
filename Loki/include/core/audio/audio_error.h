#pragma once

#ifndef AUDIO_ERROR_H
#define AUDIO_ERROR_H

namespace loki
{

inline void ErrorCheck( FMOD_RESULT _Result )
{
	if (_Result != FMOD_OK)
	{
		LOG(VL_ERROR, "FMOD: (%d) %s", _Result, FMOD_ErrorString(_Result));
	}
}

}

#endif