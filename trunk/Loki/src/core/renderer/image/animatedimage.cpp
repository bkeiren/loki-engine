#include "core/renderer/image/animatedimage.h"
#include "core/engine.h"

namespace loki
{

namespace renderer
{

LkAnimatedImage::LkAnimatedImage( float _Duration, int _Frames, const char* _Texture, const vec2& _Position, const vec2& _Size, bool _PositionIsAbsolute /* = false */, bool _SizeIsAbsolute /* = false */ )	:
	LkImage(_Texture, _Position, _Size, _PositionIsAbsolute, _SizeIsAbsolute),
	m_Duration(_Duration),
	m_Frames(_Frames),
	m_CurrentFrame(0),
	m_TimeElapsed(0.0f),
	m_Speed(1.0f)
{
	SubscribeToEvent(EVENT_ONUPDATE);
}

LkAnimatedImage::LkAnimatedImage( float _Duration, int _Frames, const char* _Texture, const vec3& _Position, const vec2& _Size, bool _PositionIsAbsolute /* = false */, bool _SizeIsAbsolute /* = false */ )	:
	LkImage(_Texture, _Position, _Size, _PositionIsAbsolute, _SizeIsAbsolute),
	m_Duration(_Duration),
	m_Frames(_Frames),
	m_CurrentFrame(0),
	m_TimeElapsed(0.0f),
	m_Speed(1.0f)
{
	SubscribeToEvent(EVENT_ONUPDATE);
}

LkAnimatedImage::~LkAnimatedImage()
{

}

void LkAnimatedImage::SetAnimationSpeed( float _Speed )
{
	m_Speed = _Speed;
}

float LkAnimatedImage::GetAnimationSpeed() const
{
	return m_Speed;
}

void LkAnimatedImage::_OnEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_ONUPDATE:
		{
			m_TimeElapsed += loki::g_Engine->GetFrameTime() * m_Speed;

			// Wrap around if necessary.
			if (m_TimeElapsed < 0.0f)
			{
				m_TimeElapsed = m_Duration - (int(-m_TimeElapsed / m_Duration) * m_Duration);
			}
			else if (m_TimeElapsed >= m_Duration)
			{
				m_TimeElapsed -= int(m_TimeElapsed / m_Duration) * m_Duration;
			}

			// Calculate the current frame.
			m_CurrentFrame = int((m_TimeElapsed / m_Duration) * m_Frames);

			// Set the LkImage class' UV values for rendering.
			float t = (1.0f / m_Frames);
			float t2 = t * m_CurrentFrame;
			m_UVTopLeft = vec2(t2, 0.0f);
			m_UVBottomRight = vec2(t2 + t, 1.0f);

			break;
		}
	}
}

}

}
