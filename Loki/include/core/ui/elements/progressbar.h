#pragma once

#ifndef OVERLAY_PROGRESSBAR_H
#define OVERLAY_PROGRESSBAR_H

#include "core/ui/overlayelement.h"
#include "core/renderer/image/image.h"

namespace loki
{

namespace renderer
{
	class LkImage;
}

namespace ui
{

LkOverlayElement* OverlayElementFactory_ProgressBar();

class LkOverlayProgressBar	: public LkOverlayElement, public renderer::LkImage
{
	friend LkOverlayElement* OverlayElementFactory_ProgressBar();
public:
	//////////////////////////////////////////////////////////////////////////
	// Sets the progress bar's progress. Value will be clamped to the range 
	// [0 .. 1].
	//////////////////////////////////////////////////////////////////////////
	void SetProgress( f32 _Progress );
	
	f32 GetProgress() const;

protected:
	virtual void Render();

	virtual void _Init();

	virtual void _OnEvent( const LkEvent& _Event );

private:
	LkOverlayProgressBar();
	~LkOverlayProgressBar();

	renderer::LkImage* m_BarStartEmpty;
	renderer::LkImage* m_BarCenterEmpty;
	renderer::LkImage* m_BarEndEmpty;
	renderer::LkImage* m_BarStartFilled;
	renderer::LkImage* m_BarCenterFilled;
	renderer::LkImage* m_BarEndFilled;

	f32 m_Progress;
};

}

}

#endif