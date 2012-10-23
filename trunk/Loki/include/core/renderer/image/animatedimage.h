#pragma once

#ifndef ANIMATEDIMAGE_H
#define ANIMATEDIMAGE_H

#include "core/renderer/image/image.h"
#include "core/eventsystem/eventlistener/eventlistener.h"

namespace loki
{

namespace renderer
{

//////////////////////////////////////////////////////////////////////////
// This class provides functionality to render an animated image.
// The image's source texture is supposed to be an imagestrip of frames.
//////////////////////////////////////////////////////////////////////////
class LkAnimatedImage	: public LkImage, public LkEventListener
{
public:
	//////////////////////////////////////////////////////////////////////////
	// _Duration is in seconds. Over this duration, _Frames will be shown.
	//////////////////////////////////////////////////////////////////////////
	LkAnimatedImage( f32 _Duration, int32 _Frames, const char* _Texture, const vec3& _Position, const vec2& _Size, bool _PositionIsAbsolute = false, bool _SizeIsAbsolute = false );
	LkAnimatedImage( f32 _Duration, int32 _Frames, const char* _Texture, const vec2& _Position, const vec2& _Size, bool _PositionIsAbsolute = false, bool _SizeIsAbsolute = false );
	~LkAnimatedImage();
	
	void SetAnimationSpeed( f32 _Speed );
	f32 GetAnimationSpeed() const;

protected:
	void _OnEvent( const LkEvent& _Event );

private:
	f32 m_Duration;
	int32 m_Frames;
	int32 m_CurrentFrame;
	f32 m_TimeElapsed;
	f32 m_Speed;
};

}

}

#endif
