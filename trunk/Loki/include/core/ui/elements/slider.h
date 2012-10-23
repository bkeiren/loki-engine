#pragma once

#ifndef OVERLAY_SLIDER_H
#define OVERLAY_SLIDER_H

#include "core/ui/elements/button.h"

namespace loki
{

namespace renderer
{
	class LkImage;
}

namespace ui
{

enum ESliderOrientation
{
	SO_HORIZONTAL = 0,
	SO_VERTICAL
};

#define OCB_SLIDER_VALUE_MOVE					loki::ui::OCB_UNSPECIFIED_CALLBACK0	// While sliding.
#define OCB_SLIDER_VALUE_RELEASE				loki::ui::OCB_UNSPECIFIED_CALLBACK1	// After sliding.

LkOverlayElement* OverlayElementFactory_Slider();

class LkOverlaySlider	: public LkOverlayButton
{
	friend LkOverlayElement* OverlayElementFactory_Slider();
public:
	//////////////////////////////////////////////////////////////////////////
	// Returns the current slider position in the range [0 .. 1].
	//////////////////////////////////////////////////////////////////////////
	f32 GetSliderValue() const;

	//////////////////////////////////////////////////////////////////////////
	// Sets the current slider value. The value will be clamped to the range
	// [0 .. 1] and if required, stepping will be resolved.
	//////////////////////////////////////////////////////////////////////////
	void SetSliderValue( f32 _Value );

	//////////////////////////////////////////////////////////////////////////
	// Returns the number of steps in the slider range.
	//////////////////////////////////////////////////////////////////////////
	uint32 GetNumSteps() const;

	//////////////////////////////////////////////////////////////////////////
	// Sets the number of steps that the slider uses.
	//////////////////////////////////////////////////////////////////////////
	void SetNumSteps( uint32 _NumSteps );

protected:
	virtual void Render();
	virtual void _Init();

	virtual void _OnEvent( const LkEvent& _Event );

private:
	LkOverlaySlider();
	~LkOverlaySlider();

	void _SetPosition( f32 _Position );

	void _ResolveStep();

	renderer::LkImage* m_SliderBarStart;
	renderer::LkImage* m_SliderBarEnd;
	renderer::LkImage* m_SliderBarCenter;
	renderer::LkImage* m_SliderButton;

	ESliderOrientation m_Orientation;

	// Position of the slider's button. 0.0 indicates it is at the start of the slider bar and 1.0 indicates it is at the end.
	f32 m_Position;

	uint32 m_NumSteps;
};

}

}

#endif