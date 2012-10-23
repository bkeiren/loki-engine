#include "core/ui/elements/slider.h"
#include "core/renderer/image/image.h"
#include "core/input/input.h"

namespace loki
{

namespace ui
{

LkOverlayElement* OverlayElementFactory_Slider()
{
	return new LkOverlaySlider();
}

LkOverlaySlider::LkOverlaySlider()	:
	m_Orientation(SO_HORIZONTAL),
	m_Position(0.5f),
	m_SliderBarStart(0),
	m_SliderBarCenter(0),
	m_SliderBarEnd(0),
	m_SliderButton(0),
	m_NumSteps(0)
{
	LkOverlayElement::SubscribeToEvent(EVENT_POSTUPDATE);
}

LkOverlaySlider::~LkOverlaySlider()
{
	delete m_SliderBarStart;
	m_SliderBarStart = NULL;

	delete m_SliderBarCenter;
	m_SliderBarCenter = NULL;

	delete m_SliderBarEnd;
	m_SliderBarEnd = NULL;

	delete m_SliderButton;
	m_SliderButton = NULL;
}

f32 LkOverlaySlider::GetSliderValue() const
{
	return m_Position;
}

void LkOverlaySlider::SetSliderValue( f32 _Value )
{
	_SetPosition(_Value);
}

uint32 LkOverlaySlider::GetNumSteps() const
{
	return m_NumSteps;
}

void LkOverlaySlider::SetNumSteps( uint32 _NumSteps )
{
	m_NumSteps = _NumSteps;
	
	_ResolveStep();
}

void LkOverlaySlider::Render()
{
	m_SliderBarCenter->Render();
	m_SliderBarStart->Render();
	m_SliderBarEnd->Render();

	m_SliderButton->Render();

	// NOTE: LkOverlayButton::Render() is not called because we don't need (or want) to render the underlying LkOverlayButton class.
	// This class only provides a way of detecting mouse pressed on the full length of the slider and to provide a good way
	// to size/position the entire slider.
	// Also note that if the underlying button were to be rendered, there would not actually be any image to render
	// because none would have been loaded.
}

void LkOverlaySlider::_Init()
{
	m_SliderBarStart = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_SliderBarCenter = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_SliderBarEnd = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_SliderButton = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));

	const LkOverlayStyle* style = GetParentOverlay()->GetOverlayStyle();
	std::string output;
	std::string output2;
	
	if (m_Orientation == SO_HORIZONTAL)
	{
		m_SliderButton->SetAnchorPoint(renderer::AP_TOPMIDDLE);

		if (!style->GetData(GetOverlayElementType(), "ButtonImageHor", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load ButtonImageHor path");
		}
		else
		{
			m_SliderButton->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "ButtonImageOverHor", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load ButtonImageOverHor path");
		}
		else
		{
			m_SliderButton->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "ButtonImageDownHor", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load ButtonImageDownHor path");
		}
		else
		{
			m_SliderButton->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "BarImageStartHor", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarImageStartHor path");
		}
		else
		{
			m_SliderBarStart->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "BarImageCenterHor", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarImageCenterHor path");
		}
		else
		{
			m_SliderBarCenter->AddTexture(output.c_str());
		}
		
		if (!style->GetData(GetOverlayElementType(), "BarImageEndHor", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarImageEndHor path");
		}
		else
		{
			m_SliderBarEnd->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "DefaultSizeXHor", output) || !style->GetData(GetOverlayElementType(), "DefaultSizeYHor", output2))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load DefaultSizeXHor or DefaultSizeYHor value");
		}
		else
		{
			f32 x = 0.0f;
			f32 y = 0.0f;

			x = (f32)atof(output.c_str());
			y = (f32)atof(output2.c_str());

			LkImage::SetRelativeSize(vec2(x, y));
			m_SliderBarCenter->SetRelativeSize(vec2(x, y));
		}

		if (!style->GetData(GetOverlayElementType(), "BarStartSizeXHor", output) || !style->GetData(GetOverlayElementType(), "BarStartSizeYHor", output2))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarStartSizeXHor or BarStartSizeYHor value");
		}
		else
		{
			f32 x = 0.0f;
			f32 y = 0.0f;

			x = (f32)atof(output.c_str());
			y = (f32)atof(output2.c_str());

			m_SliderBarStart->SetRelativeSize(vec2(x, y));
		}

		if (!style->GetData(GetOverlayElementType(), "BarEndSizeXHor", output) || !style->GetData(GetOverlayElementType(), "BarEndSizeYHor", output2))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarEndSizeXHor or BarEndSizeYHor value");
		}
		else
		{
			f32 x = 0.0f;
			f32 y = 0.0f;

			x = (f32)atof(output.c_str());
			y = (f32)atof(output2.c_str());

			m_SliderBarEnd->SetRelativeSize(vec2(x, y));
		}

		if (!style->GetData(GetOverlayElementType(), "ButtonSizeXHor", output) || !style->GetData(GetOverlayElementType(), "ButtonSizeYHor", output2))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load ButtonSizeXHor or ButtonSizeYHor value");
		}
		else
		{
			f32 x = 0.0f;
			f32 y = 0.0f;

			x = (f32)atof(output.c_str());
			y = (f32)atof(output2.c_str());

			m_SliderButton->SetRelativeSize(vec2(x, y));
		}
	}
	else if (m_Orientation == SO_VERTICAL)
	{
		m_SliderButton->SetAnchorPoint(renderer::AP_MIDDLELEFT);

		if (!style->GetData(GetOverlayElementType(), "ButtonImageVer", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load ButtonImageVer path");
		}
		else
		{
			m_SliderButton->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "ButtonImageOverVer", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load ButtonImageOverVer path");
		}
		else
		{
			m_SliderButton->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "ButtonImageDownVer", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load ButtonImageDownVer path");
		}
		else
		{
			m_SliderButton->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "BarImageStartVer", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarImageStartVer path");
		}
		else
		{
			m_SliderBarStart->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "BarImageCenterVer", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarImageCenterVer path");
		}
		else
		{
			m_SliderBarCenter->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "BarImageEndVer", output))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarImageEndVer path");
		}
		else
		{
			m_SliderBarEnd->AddTexture(output.c_str());
		}

		if (!style->GetData(GetOverlayElementType(), "DefaultSizeXVer", output) || !style->GetData(GetOverlayElementType(), "DefaultSizeYVer", output2))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load DefaultSizeXVer or DefaultSizeYVer value");
		}
		else
		{
			f32 x = 0.0f;
			f32 y = 0.0f;

			x = (f32)atof(output.c_str());
			y = (f32)atof(output2.c_str());

			LkImage::SetRelativeSize(vec2(x, y));
			m_SliderBarCenter->SetRelativeSize(vec2(x, y));
		}

		if (!style->GetData(GetOverlayElementType(), "BarStartSizeXVer", output) || !style->GetData(GetOverlayElementType(), "BarStartSizeYVer", output2))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarStartSizeXVer or BarStartSizeYVer value");
		}
		else
		{
			f32 x = 0.0f;
			f32 y = 0.0f;

			x = (f32)atof(output.c_str());
			y = (f32)atof(output2.c_str());

			m_SliderBarStart->SetRelativeSize(vec2(x, y));
		}

		if (!style->GetData(GetOverlayElementType(), "BarEndSizeXVer", output) || !style->GetData(GetOverlayElementType(), "BarEndSizeYVer", output2))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load BarEndSizeXVer or BarEndSizeYVer value");
		}
		else
		{
			f32 x = 0.0f;
			f32 y = 0.0f;

			x = (f32)atof(output.c_str());
			y = (f32)atof(output2.c_str());

			m_SliderBarEnd->SetRelativeSize(vec2(x, y));
		}

		if (!style->GetData(GetOverlayElementType(), "ButtonSizeXVer", output) || !style->GetData(GetOverlayElementType(), "ButtonSizeYVer", output2))
		{
			LOG(VL_ERROR, "OverlaySlider::OverlaySlider: Unable to load ButtonSizeXVer or ButtonSizeYVer value");
		}
		else
		{
			f32 x = 0.0f;
			f32 y = 0.0f;

			x = (f32)atof(output.c_str());
			y = (f32)atof(output2.c_str());

			m_SliderButton->SetRelativeSize(vec2(x, y));
		}
	}
}

void LkOverlaySlider::_OnEvent( const LkEvent& _Event )
{
	LkOverlayButton::_OnEvent(_Event);

	switch (_Event.GetEventType())
	{
	case EVENT_MOUSEMOVE:
	case EVENT_MB_LEFT_PRESSED:
		{
			if (LkOverlayButton::IsDown() || LkOverlayButton::IsPressed())
			{
				f32 pos = 0.0f;

				switch (m_Orientation)
				{
				case SO_HORIZONTAL:
					{
						pos = (g_Input->GetMouseX() - LkImage::GetAbsolutePosition().x) / LkImage::GetAbsoluteSize().x;

						break;
					}
				case SO_VERTICAL:
					{
						pos = (g_Input->GetMouseY() - LkImage::GetAbsolutePosition().y) / LkImage::GetAbsoluteSize().y;

						break;
					}
				}

				_SetPosition(pos);
			}

			break;
		}
	case EVENT_MB_LEFT_RELEASED:
		{
			if (LkOverlayButton::IsReleased())
			{
				_CallCallback(OCB_SLIDER_VALUE_RELEASE);
			}
			break;
		}
	case EVENT_POSTUPDATE:
		{
			vec3 SliderCenterPos = LkImage::GetRelativePosition();
			m_SliderBarCenter->SetRelativePosition(SliderCenterPos);

			vec2 SliderBarStartPos;
			vec2 SliderBarEndPos;
			vec2 SliderButtonPos;

			switch (m_Orientation)
			{
			case SO_HORIZONTAL:
				{
					f32 offset = m_Position * LkImage::GetRelativeSize().x * 0.5f;

					SliderBarStartPos = vec2(SliderCenterPos.x - m_SliderBarStart->GetRelativeSize().x * 0.5f, SliderCenterPos.y);
					SliderBarEndPos = vec2(SliderCenterPos.x + LkImage::GetRelativeSize().x * 0.5f, SliderCenterPos.y);
					SliderButtonPos = vec2(SliderCenterPos.x + offset, SliderCenterPos.y);

					break;
				}
			case SO_VERTICAL:
				{
					f32 offset = m_Position * LkImage::GetRelativeSize().y * 0.5f;

					SliderBarStartPos = vec2(SliderCenterPos.x, SliderCenterPos.y - m_SliderBarStart->GetRelativeSize().y * 0.5f);
					SliderBarEndPos = vec2(SliderCenterPos.x, SliderCenterPos.y + LkImage::GetRelativeSize().y * 0.5f);
					SliderButtonPos = vec2(SliderCenterPos.x, SliderCenterPos.y + offset);

					break;
				}
			}
			
			m_SliderBarStart->SetRelativePosition(SliderBarStartPos);
			m_SliderBarEnd->SetRelativePosition(SliderBarEndPos);
			m_SliderButton->SetRelativePosition(SliderButtonPos);
			m_SliderBarCenter->SetRelativeSize(LkImage::GetRelativeSize());

			if (LkOverlayButton::IsDown())
			{
				m_SliderButton->SetTextureIndex(2);	
			}
			else if (LkOverlayButton::IsMouseOver())
			{
				m_SliderButton->SetTextureIndex(1);
			}
			else
			{
				m_SliderButton->SetTextureIndex(0);
			}

			break;	
		}
	}
}

void LkOverlaySlider::_SetPosition( f32 _Position )
{
	m_Position = _Position;
	m_Position = math::clamp(m_Position, 0.0f, 1.0f);

	// Clamp to nearest step if required.
	_ResolveStep();

	_CallCallback(OCB_SLIDER_VALUE_MOVE);
}

void LkOverlaySlider::_ResolveStep()
{
	if (m_NumSteps > 0)
	{
		f32 stepsize = 1.0f / (m_NumSteps - 1);

		m_Position = stepsize * int32((m_Position / stepsize) + 0.5f);
	}
}

}

}