#pragma once

#ifndef OVERLAY_CHECKBOX_H
#define OVERLAY_CHECKBOX_H

#include "core/ui/elements/button.h"

namespace loki
{

namespace renderer
{
	class LkImage;
}

namespace ui
{

enum ECheckBoxState
{
	CBS_CLEAR = 0,
	CBS_CLEAR_OVER,
	CBS_CLEAR_DOWN,
	CBS_CHECKED,
	CBS_CHECKED_OVER,
	CBS_CHECKED_DOWN
};

#define OCB_CHECKBOX_TOGGLE		loki::ui::OCB_UNSPECIFIED_CALLBACK0

LkOverlayElement* OverlayElementFactory_CheckBox();

class LkOverlayCheckBox	: public LkOverlayButton
{
	friend LkOverlayElement* OverlayElementFactory_CheckBox();
public:
	bool IsChecked() const;

	void Render();
	void _Init();

	void _OnEvent( const LkEvent& _Event );

private:
	LkOverlayCheckBox();
	~LkOverlayCheckBox();

	void _ToggleState();
	
	bool m_IsChecked;
	renderer::LkImage* m_CheckMarkImage;
};

}

}

#endif