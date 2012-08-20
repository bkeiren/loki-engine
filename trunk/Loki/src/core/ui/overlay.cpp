#include "core/ui/overlay.h"
#include "core/ui/overlayelement.h"
#include "util/hash/hash.h"

#include "core/ui/elements/button.h"
#include "core/ui/elements/checkbox.h"
#include "core/ui/elements/slider.h"
#include "core/ui/elements/progressbar.h"

namespace loki
{

namespace ui
{

LkOverlay::ElementFactories LkOverlay::m_ElementFactories;

LkOverlay::LkOverlay( const LkOverlayStyle* _OverlayStyle )	:
	m_OverlayStyle(_OverlayStyle)
{
	static bool DefaultFactoriesInitialized = false;
	if (!DefaultFactoriesInitialized)
	{
		RegisterElementFactory("button", &OverlayElementFactory_Button);
		RegisterElementFactory("checkbox", &OverlayElementFactory_CheckBox);
		RegisterElementFactory("slider", &OverlayElementFactory_Slider);
		RegisterElementFactory("progressbar", &OverlayElementFactory_ProgressBar);
	}
}

LkOverlay::LkOverlay()
{
	ILLEGAL_CTOR_ERROR("Overlay");
}

LkOverlay::~LkOverlay()
{

}

bool LkOverlay::RegisterElementFactory( const char* _ElementType, OverlayElementFactory _Factory )
{
	assert(_ElementType != NULL);
	assert(_Factory != NULL);

	ElementFactories::iterator it = m_ElementFactories.find(_ElementType);
	if (it != m_ElementFactories.end())
	{
		LOG(VL_ERROR, "Overlay::RegisterElementFactory: A factory for overlay element type '%s' already exists", _ElementType);
		return false;
	}
	m_ElementFactories.insert(ElementFactoriesPair(_ElementType, _Factory));
	return true;
}

LkOverlayElement* LkOverlay::GetElement( const char* _Element ) const
{
	assert(_Element != NULL);

	OverlayElementID hash = HASH(_Element);
	return _GetElement(hash);
}

LkOverlayElement* LkOverlay::CreateElement( const char* _Element, const char* _Type )
{
	assert(_Element != NULL);
	assert(_Type != NULL);

	OverlayElementID hash = HASH(_Element);

	Elements::iterator it = m_Elements.find(hash);
	if (it != m_Elements.end())
	{
		LOG(VL_ERROR, "Overlay::CreateElement: An overlay element named '%s' already exists", _Element);
		return NULL;
	}

	ElementFactories::iterator it2 = m_ElementFactories.find(_Type);
	if (it2 == m_ElementFactories.end())
	{
		LOG(VL_ERROR, "Overlay::CreateElement: No factory has been registered for overlay element type '%s'", _Type);
		return NULL;
	}
	LkOverlayElement* element = (*it2).second();
	element->_SetOverlayElementData(_Type, _Element, this);
	element->_Init();
	m_Elements.insert(ElementsPair(hash, element));
	return element;
}

const LkOverlayStyle* LkOverlay::GetOverlayStyle() const
{
	return m_OverlayStyle;
}

void LkOverlay::GetRegisteredElementTypesList( std::list<std::string>& _Output ) const
{
	_Output.clear();

	for (ElementFactories::const_iterator it = m_ElementFactories.begin(); it != m_ElementFactories.end(); ++it)
	{
		_Output.push_back((*it).first);
	}
}

bool LkOverlay::IsValidElementType( const char* _ElementType ) const
{
	ElementFactories::iterator it2 = m_ElementFactories.find(_ElementType);
	if (it2 == m_ElementFactories.end())
	{
		return false;
	}
	return true;
}

LkOverlayElement* LkOverlay::_GetElement( OverlayElementID _ElementID ) const
{
	Elements::const_iterator it = m_Elements.find(_ElementID);
	if (it != m_Elements.end())
	{
		return (*it).second;
	}
	return NULL;
}

void LkOverlay::Render()
{
	for (Elements::iterator it = m_Elements.begin(); it != m_Elements.end(); ++it)
	{
		(*it).second->Render();
	}
}

}

}