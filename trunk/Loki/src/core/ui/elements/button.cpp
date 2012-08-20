#include "core/ui/elements/button.h"

namespace loki
{

namespace ui
{

#define ADVANCEQUEUEVARIABLES(current, prev, state)	{prev = current; current = state;}

LkOverlayElement* OverlayElementFactory_Button()
{
	return new LkOverlayButton();
}

LkOverlayButton::LkOverlayButton()	:
	renderer::LkImage(0, glm::vec2(0.0f, 0.0f), glm::vec2(0.2f, 0.1f)),
	m_ButtonState(BS_UP),
	m_PreviousButtonState(m_ButtonState),
	m_MouseIsOver(false),
	m_PreviousMouseIsOver(false)
{
	LkOverlayElement::SubscribeToEvent(EVENT_MOUSEMOVE);
	LkOverlayElement::SubscribeToEvent(EVENT_MB_LEFT_PRESSED);
	LkOverlayElement::SubscribeToEvent(EVENT_MB_LEFT_RELEASED);
	LkOverlayElement::SubscribeToEvent(EVENT_PREUPDATE);
}

LkOverlayButton::~LkOverlayButton()
{
	
}

bool LkOverlayButton::IsPressed() const
{
	return (m_ButtonState == BS_PRESSED);
}

bool LkOverlayButton::IsReleased() const
{
	return (m_ButtonState == BS_RELEASED);
}

bool LkOverlayButton::IsDown() const
{
	return (m_ButtonState == BS_DOWN);
}

bool LkOverlayButton::IsMouseOver() const
{
	return (m_ButtonState == BS_DOWN || m_ButtonState == BS_OVER);
}

void LkOverlayButton::Render()
{
	switch (m_ButtonState)
	{
	case BS_UP:
		{
			LkImage::SetTextureIndex(0);
			break;
		}
	case BS_OVER:
		{
			LkImage::SetTextureIndex(1);
			break;
		}
	case BS_DOWN:
		{
			LkImage::SetTextureIndex(2);
			break;
		}
	}
	LkImage::Render();
}

void LkOverlayButton::_Init()
{
	const LkOverlayStyle* style = GetParentOverlay()->GetOverlayStyle();
	std::string output;
	std::string output2;
	if (!style->GetData(GetOverlayElementType(), "Image", output))
	{
		LOG(VL_ERROR, "OverlayButton::OverlayButton: Unable to load Image path");
	}
	else
	{
		LkImage::AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageOver", output))
	{
		LOG(VL_ERROR, "OverlayButton::OverlayButton: Unable to load ImageOver path");
	}
	else
	{
		LkImage::AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageDown", output))
	{
		LOG(VL_ERROR, "OverlayButton::OverlayButton: Unable to load ImageDown path");
	}
	else
	{
		LkImage::AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "DefaultSizeX", output) || !style->GetData(GetOverlayElementType(), "DefaultSizeY", output2))
	{
		LOG(VL_ERROR, "OverlayButton::OverlayButton: Unable to load DefaultSizeX or DefaultSizeY value");
	}
	else
	{
		float x = 0.0f;
		float y = 0.0f;

		x = (float)atof(output.c_str());
		y = (float)atof(output2.c_str());

		LkImage::SetRelativeSize(glm::vec2(x, y));
	}
}

void LkOverlayButton::_OnEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_MOUSEMOVE:
		{
			if (_MouseIsWithin())
			{
				ADVANCEQUEUEVARIABLES(m_MouseIsOver, m_PreviousMouseIsOver, true);

				if (m_MouseIsOver && !m_PreviousMouseIsOver)
				{
					ADVANCEQUEUEVARIABLES(m_ButtonState, m_PreviousButtonState, BS_OVER);

					_CallCallback(OCB_MOUSE_ENTER);
				}
			}
			else
			{
				ADVANCEQUEUEVARIABLES(m_MouseIsOver, m_PreviousMouseIsOver, false);

				if (m_PreviousMouseIsOver && !m_MouseIsOver)
				{
					ADVANCEQUEUEVARIABLES(m_ButtonState, m_PreviousButtonState, BS_UP);

					_CallCallback(OCB_MOUSE_LEAVE);
				}
			}

			break;
		}
	case EVENT_MB_LEFT_PRESSED:
		{
			if (m_ButtonState == BS_OVER)
			{
				ADVANCEQUEUEVARIABLES(m_ButtonState, m_PreviousButtonState, BS_PRESSED);

				_CallCallback(OCB_MOUSE_LEFT_PRESSED);
			}
			break;
		}
	case EVENT_MB_LEFT_RELEASED:
		{
			if (m_ButtonState == BS_DOWN)
			{
				ADVANCEQUEUEVARIABLES(m_ButtonState, m_PreviousButtonState, BS_RELEASED);

				_CallCallback(OCB_MOUSE_LEFT_RELEASED);
			}
			break;
		}
	case EVENT_PREUPDATE:
		{
			if (m_ButtonState == BS_PRESSED)
			{
				ADVANCEQUEUEVARIABLES(m_ButtonState, m_PreviousButtonState, BS_DOWN);
			}
			else if (m_ButtonState == BS_RELEASED)
			{
				ADVANCEQUEUEVARIABLES(m_ButtonState, m_PreviousButtonState, BS_UP);

				if (_MouseIsWithin())
				{
					// DO NOT USE ADVANCEQUEUEVARIABLES MACRO HERE.
					m_ButtonState = BS_OVER;
				}
			}
			break;	
		}
	}
}

bool LkOverlayButton::_MouseIsWithin() const
{
	glm::vec3 abspos = LkImage::GetAbsolutePosition();
	glm::vec2 abssize = LkImage::GetAbsoluteSize();

	glm::int2 mousepos = g_Input->GetMousePosition() /*- glm::int2(LkImage::GetAbsoluteAnchorOffset())*/;

	return (mousepos.x >= abspos.x && mousepos.x <= abspos.x + abssize.x &&
			mousepos.y >= abspos.y && mousepos.y <= abspos.y + abssize.y);
}

#undef SETBUTTONSTATE

}

}