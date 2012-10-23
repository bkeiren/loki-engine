#include "core/ui/elements/checkbox.h"
#include "core/renderer/image/image.h"

namespace loki
{

namespace ui
{

LkOverlayElement* OverlayElementFactory_CheckBox()
{
	return new LkOverlayCheckBox();
}

LkOverlayCheckBox::LkOverlayCheckBox()	:
	m_IsChecked(false),
	m_CheckMarkImage(NULL)
{
	LkOverlayElement::SubscribeToEvent(EVENT_PREUPDATE);
}

LkOverlayCheckBox::~LkOverlayCheckBox()
{
	delete m_CheckMarkImage;
	m_CheckMarkImage = NULL;
}

bool LkOverlayCheckBox::IsChecked() const
{
	return m_IsChecked;
}

void LkOverlayCheckBox::Render()
{
	LkOverlayButton::Render();

	if (m_IsChecked)
	{
		m_CheckMarkImage->SetTextureIndex(0);
	}
	else
	{
		m_CheckMarkImage->SetTextureIndex(1);
	}
	m_CheckMarkImage->Render();
}

void LkOverlayCheckBox::_Init()
{
	m_CheckMarkImage = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));

	const LkOverlayStyle* style = GetParentOverlay()->GetOverlayStyle();
	std::string output;
	std::string output2;
	if (!style->GetData(GetOverlayElementType(), "Image", output))
	{
		LOG(VL_ERROR, "OverlayCheckBox::OverlayCheckBox: Unable to load Image path");
	}
	else
	{
		LkImage::AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageOver", output))
	{
		LOG(VL_ERROR, "OverlayCheckBox::OverlayCheckBox: Unable to load ImageOver path");
	}
	else
	{
		LkImage::AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageDown", output))
	{
		LOG(VL_ERROR, "OverlayCheckBox::OverlayCheckBox: Unable to load ImageDown path");
	}
	else
	{
		LkImage::AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageCheckmarkChecked", output))
	{
		LOG(VL_ERROR, "OverlayCheckBox::OverlayCheckBox: Unable to load ImageCheckmarkChecked path");
	}
	else
	{
		m_CheckMarkImage->AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageCheckmarkUnchecked", output))
	{
		LOG(VL_ERROR, "OverlayCheckBox::OverlayCheckBox: Unable to load ImageCheckmarkUnchecked path");
	}
	else
	{
		m_CheckMarkImage->AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "DefaultSizeX", output) || !style->GetData(GetOverlayElementType(), "DefaultSizeY", output2))
	{
		LOG(VL_ERROR, "OverlayButton::OverlayButton: Unable to load DefaultSizeX or DefaultSizeY value");
	}
	else
	{
		f32 x = 0.0f;
		f32 y = 0.0f;

		x = (f32)atof(output.c_str());
		y = (f32)atof(output2.c_str());

		LkImage::SetRelativeSize(vec2(x, y));
	}
}

void LkOverlayCheckBox::_OnEvent( const LkEvent& _Event )
{
	LkOverlayButton::_OnEvent(_Event);

	switch (_Event.GetEventType())
	{
	case EVENT_MB_LEFT_RELEASED:
		{
			if (LkOverlayButton::IsReleased())
			{
				_ToggleState();
			}

			break;
		}
	case EVENT_PREUPDATE:
		{
			// Copy size and position each frame.
			m_CheckMarkImage->SetRelativePosition(LkImage::GetRelativePosition());
			m_CheckMarkImage->SetRelativeSize(LkImage::GetRelativeSize());

			break;
		}
	}
}

void LkOverlayCheckBox::_ToggleState()
{
	m_IsChecked = !m_IsChecked;
	
	_CallCallback(OCB_CHECKBOX_TOGGLE);
}

}

}