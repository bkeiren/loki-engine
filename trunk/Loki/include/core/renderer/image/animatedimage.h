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
	LkAnimatedImage( float _Duration, int _Frames, const char* _Texture, const glm::vec3& _Position, const glm::vec2& _Size, bool _PositionIsAbsolute = false, bool _SizeIsAbsolute = false );
	LkAnimatedImage( float _Duration, int _Frames, const char* _Texture, const glm::vec2& _Position, const glm::vec2& _Size, bool _PositionIsAbsolute = false, bool _SizeIsAbsolute = false );
	~LkAnimatedImage();
	
	void SetAnimationSpeed( float _Speed );
	float GetAnimationSpeed() const;

protected:
	void _OnEvent( const LkEvent& _Event );

private:
	float m_Duration;
	int m_Frames;
	int m_CurrentFrame;
	float m_TimeElapsed;
	float m_Speed;
};

}

}

#endif
