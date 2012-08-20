#pragma once

#ifndef OVERLAY_BUTTON_H
#define OVERLAY_BUTTON_H

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

enum EButtonState
{
	BS_UP = 0,	// Still up (not released 'anymore', not being pressed down).
	BS_OVER,	// Mouse is over.
	BS_PRESSED,	// Just pressed.
	BS_DOWN,	// Still being pressed down.
	BS_RELEASED	// Just released.
};

LkOverlayElement* OverlayElementFactory_Button();

class LkOverlayButton	: public LkOverlayElement, public renderer::LkImage
{
	friend LkOverlayElement* OverlayElementFactory_Button();
public:
	bool IsPressed() const;
	bool IsReleased() const;
	bool IsDown() const;
	bool IsMouseOver() const;

	//static void SetDefaultSize( const glm::vec2& _DefaultSize );
protected:
	LkOverlayButton();
	virtual ~LkOverlayButton();

	virtual void Render();
	virtual void _Init();

	virtual void _OnEvent( const LkEvent& _Event );

private:
	bool _MouseIsWithin() const;

	EButtonState m_ButtonState;
	EButtonState m_PreviousButtonState;

	bool m_MouseIsOver;
	bool m_PreviousMouseIsOver;
};

}

}

#endif