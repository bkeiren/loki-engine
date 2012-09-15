#include "core/ui/elements/progressbar.h"
#include "core/renderer/image/image.h"

namespace loki
{

namespace ui
{

LkOverlayElement* OverlayElementFactory_ProgressBar()
{
	return new LkOverlayProgressBar();
}

LkOverlayProgressBar::LkOverlayProgressBar()	:
	renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f)),
	m_Progress(0.0f)
{
	SubscribeToEvent(EVENT_POSTUPDATE);
	
	SetProgress(0.0f);
}

LkOverlayProgressBar::~LkOverlayProgressBar()
{
	delete m_BarStartEmpty;
	m_BarStartEmpty = NULL;

	delete m_BarCenterEmpty;
	m_BarCenterEmpty = NULL;

	delete m_BarEndEmpty;
	m_BarEndEmpty = NULL;

	delete m_BarStartFilled;
	m_BarStartFilled = NULL;

	delete m_BarCenterFilled;
	m_BarCenterFilled = NULL;

	delete m_BarEndFilled;
	m_BarEndFilled = NULL;
}

void LkOverlayProgressBar::SetProgress( float _Progress )
{
	m_Progress = math::clamp(_Progress, 0.0f, 1.0f);

	vec2 v = LkImage::GetRelativeSize();
	v.x *= m_Progress;

	m_BarCenterFilled->SetRelativeSize(v);
}

float LkOverlayProgressBar::GetProgress() const
{
	return m_Progress;
}

void LkOverlayProgressBar::Render()
{
	m_BarStartEmpty->Render();
	m_BarCenterEmpty->Render();
	m_BarEndEmpty->Render();
	
	if (m_Progress > 0.0f)
	{
		m_BarStartFilled->Render();
	}

	m_BarCenterFilled->Render();

	if (m_Progress >= 1.0f)
	{
		m_BarEndFilled->Render();
	}
}

void LkOverlayProgressBar::_Init()
{
	m_BarStartEmpty = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_BarCenterEmpty = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_BarEndEmpty = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_BarStartFilled = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_BarCenterFilled = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));
	m_BarEndFilled = new renderer::LkImage(0, vec2(0.0f, 0.0f), vec2(1.0f, 1.0f));

	const LkOverlayStyle* style = GetParentOverlay()->GetOverlayStyle();
	std::string output;
	std::string output2;
	
	if (!style->GetData(GetOverlayElementType(), "ImageStartEmpty", output))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load ImageStartEmpty path");
	}
	else
	{
		m_BarStartEmpty->AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageCenterEmpty", output))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load ImageCenterEmpty path");
	}
	else
	{
		m_BarCenterEmpty->AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageEndEmpty", output))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load ImageEndEmpty path");
	}
	else
	{
		m_BarEndEmpty->AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageStartFilled", output))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load ImageStartFilled path");
	}
	else
	{
		m_BarStartFilled->AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageCenterFilled", output))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load ImageCenterFilled path");
	}
	else
	{
		m_BarCenterFilled->AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "ImageEndFilled", output))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load ImageEndFilled path");
	}
	else
	{
		m_BarEndFilled->AddTexture(output.c_str());
	}

	if (!style->GetData(GetOverlayElementType(), "DefaultSizeX", output) || !style->GetData(GetOverlayElementType(), "DefaultSizeY", output2))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load DefaultSizeX or DefaultSizeY value");
	}
	else
	{
		float x = 0.0f;
		float y = 0.0f;

		x = (float)atof(output.c_str());
		y = (float)atof(output2.c_str());

		LkImage::SetRelativeSize(vec2(x, y));
	}

	if (!style->GetData(GetOverlayElementType(), "BarStartSizeX", output) || !style->GetData(GetOverlayElementType(), "BarStartSizeY", output2))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load BarStartSizeX or BarStartSizeY value");
	}
	else
	{
		float x = 0.0f;
		float y = 0.0f;

		x = (float)atof(output.c_str());
		y = (float)atof(output2.c_str());

		m_BarStartEmpty->SetRelativeSize(vec2(x, y));
		m_BarStartFilled->SetRelativeSize(vec2(x, y));
	}

	if (!style->GetData(GetOverlayElementType(), "BarEndSizeX", output) || !style->GetData(GetOverlayElementType(), "BarEndSizeY", output2))
	{
		LOG(VL_ERROR, "OverlayProgressBar::OverlayProgressBar: Unable to load BarEndSizeX or BarEndSizeY value");
	}
	else
	{
		float x = 0.0f;
		float y = 0.0f;

		x = (float)atof(output.c_str());
		y = (float)atof(output2.c_str());

		m_BarEndEmpty->SetRelativeSize(vec2(x, y));
		m_BarEndFilled->SetRelativeSize(vec2(x, y));
	}
}

void LkOverlayProgressBar::_OnEvent( const LkEvent& _Event )
{
	switch (_Event.GetEventType())
	{
	case EVENT_POSTUPDATE:
		{
			vec3 barstartpos = LkImage::GetRelativePosition() - vec3(m_BarStartEmpty->GetRelativeSize().x, 0.0f, 0.0f);
			vec3 barcenterpos = LkImage::GetRelativePosition();
			vec3 barendpos = LkImage::GetRelativePosition() + vec3(LkImage::GetRelativeSize().x, 0.0f, 0.0f);

			m_BarCenterEmpty->SetRelativeSize(LkImage::GetRelativeSize());

			m_BarCenterEmpty->SetRelativePosition(barcenterpos);
			m_BarCenterFilled->SetRelativePosition(barcenterpos);			
			
			m_BarStartEmpty->SetRelativePosition(barstartpos);
			m_BarStartFilled->SetRelativePosition(barstartpos);
			
			m_BarEndEmpty->SetRelativePosition(barendpos);
			m_BarEndFilled->SetRelativePosition(barendpos);

			break;
		}
	}
}

}

}