#include "core/ui/overlayelement.h"

namespace loki
{

namespace ui
{

LkOverlayElement::LkOverlayElement()	:
	m_OverlayElementType("Unknown - This indicates an error"),
	m_OverlayElementName("Unknown - This indicates an error"),
	m_ParentOverlay(0),
	m_Callbacks(Callbacks(OCB_COUNT, 0))
{
	
}

LkOverlayElement::~LkOverlayElement()
{

}

const std::string& LkOverlayElement::GetOverlayElementType() const
{
	return m_OverlayElementType;
}

const std::string& LkOverlayElement::GetOverlayElementName() const
{
	return m_OverlayElementName;
}

void LkOverlayElement::RegisterCallback( EOverlayCallback _CallbackType, OverlayElementCallback _Callback, bool _OverwritePreExisting /*= false*/ )
{
	if ((m_Callbacks[_CallbackType] && _OverwritePreExisting) || !(m_Callbacks[_CallbackType]))
	{
		m_Callbacks[_CallbackType] = _Callback;
	}
	else
	{
		LOG(VL_WARN, "OverlayElement::RegisterCallback: A callback for callback type %i has already been registered and may not be overwritten", _CallbackType);
	}
}

void LkOverlayElement::RemoveCallback( EOverlayCallback _CallbackType )
{
	m_Callbacks[_CallbackType] = 0;
}

const LkOverlay* LkOverlayElement::GetParentOverlay() const
{
	return m_ParentOverlay;
}

void LkOverlayElement::Render()
{

}

void LkOverlayElement::_Init()
{

}

void LkOverlayElement::_OnEvent( const LkEvent& _Event )
{

}

void LkOverlayElement::_CallCallback( EOverlayCallback _CallbackType )
{
	OverlayElementCallback callback = m_Callbacks[_CallbackType];
	if (callback)
	{
		callback(this);
	}
}

void LkOverlayElement::_SetOverlayElementData( const std::string& _Type, const std::string& _Name, const LkOverlay* _ParentOverlay )
{
	m_OverlayElementType = _Type;
	m_OverlayElementName = _Name;
	m_ParentOverlay = _ParentOverlay;
}

}

}