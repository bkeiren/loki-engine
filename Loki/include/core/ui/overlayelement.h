#pragma once

#ifndef OVERLAYELEMENT_H
#define OVERLAYELEMENT_H

#include "core/ui/overlay.h"
#include "core/eventsystem/eventlistener/eventlistener.h"

namespace loki
{

namespace ui
{

enum EOverlayCallback
{
	OCB_MOUSE_ENTER = 0,
	OCB_MOUSE_LEAVE,
	OCB_MOUSE_LEFT_PRESSED,
	OCB_MOUSE_LEFT_RELEASED,
	OCB_MOUSE_RIGHT_PRESSED,
	OCB_MOUSE_RIGHT_RELEASED,


	// These following callbacks events can be used by child classes if specifically named events are required by them.
	// A simple define can be used to provide preprocessor names that make it easier to use.
	// Example:
	// #define OCB_CHECKBOX_TOGGLE	OCB_UNSPECIFIED_CALLBACK0
	// Now RegisterCallback() can be called with as it's callback type argument the preprocessor define OCB_CHECKBOX_TOGGLE, 
	// as long as the class itself uses this same value to call _CallCallback() when it's toggled.
	OCB_UNSPECIFIED_CALLBACK0,
	OCB_UNSPECIFIED_CALLBACK1,
	OCB_UNSPECIFIED_CALLBACK2,
	OCB_UNSPECIFIED_CALLBACK3,
	OCB_UNSPECIFIED_CALLBACK4,
	OCB_UNSPECIFIED_CALLBACK5,
	OCB_UNSPECIFIED_CALLBACK6,

	OCB_COUNT
};

//////////////////////////////////////////////////////////////////////////
// Overlay element base class.
//////////////////////////////////////////////////////////////////////////
class LkOverlayElement	: public LkEventListener
{
	friend class LkOverlay;
public:
	typedef void (*OverlayElementCallback)( const LkOverlayElement* );

	const std::string& GetOverlayElementType() const;
	const std::string& GetOverlayElementName() const;

	//////////////////////////////////////////////////////////////////////////
	// Registers a callback function for a certain callback type.
	// NOTE: Not all child classes implement callback mechanisms for every
	// type of callback.
	//////////////////////////////////////////////////////////////////////////
	void RegisterCallback( EOverlayCallback _CallbackType, OverlayElementCallback _Callback, bool _OverwritePreExisting = false );
	void RemoveCallback( EOverlayCallback _CallbackType );
protected:
	LkOverlayElement();
	virtual ~LkOverlayElement() = 0;

	const LkOverlay* GetParentOverlay() const;

	virtual void Render() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Should be used to initialize the instance instead of the constructor.
	// During a derived class' constructor phase, certain data is not yet set.
	// That data IS set when _Init is called.
	//////////////////////////////////////////////////////////////////////////
	virtual void _Init();

	void _OnEvent( const LkEvent& _Event );

	void _CallCallback( EOverlayCallback _CallbackType );

private:
	typedef std::vector<OverlayElementCallback>	Callbacks;
	typedef Callbacks::iterator					CallbacksIter;
	typedef Callbacks::const_iterator			CallbacksConstIter;

	//////////////////////////////////////////////////////////////////////////
	// Only to be called within LkOverlay::CreateElement.
	//////////////////////////////////////////////////////////////////////////
	void _SetOverlayElementData( const std::string& _Type, const std::string& _Name, const LkOverlay* _ParentOverlay );

	Callbacks m_Callbacks;

	//////////////////////////////////////////////////////////////////////////
	// Data assigned by the parent overlay.
	//////////////////////////////////////////////////////////////////////////
	std::string m_OverlayElementType;
	std::string m_OverlayElementName;
	const LkOverlay* m_ParentOverlay;
};

}

}

#endif