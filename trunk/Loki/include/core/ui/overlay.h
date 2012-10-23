#pragma once

#ifndef OVERLAY_H
#define OVERLAY_H

#include <hash_map>
#include <core/ui/overlaymanager.h>
#include "core/ui/overlaystyle.h"

namespace loki
{

namespace ui
{

class LkOverlayElement;
class LkOverlayStyle;

typedef uint32	OverlayElementID;

class LkOverlay
{
	typedef stdext::hash_map<OverlayElementID, LkOverlayElement*>	Elements;
	typedef std::pair<OverlayElementID, LkOverlayElement*>			ElementsPair;

	typedef LkOverlayElement*(*OverlayElementFactory)();
	typedef stdext::hash_map<const char*, OverlayElementFactory>	ElementFactories;
	typedef std::pair<const char*, OverlayElementFactory>			ElementFactoriesPair;

	friend class LkOverlayManager;
public:
	//////////////////////////////////////////////////////////////////////////
	// Registers a new element type factory function. _ElementType matches
	// the _Type argument for CreateElement.
	//////////////////////////////////////////////////////////////////////////
	static bool RegisterElementFactory( const char* _ElementType, OverlayElementFactory _Factory );

	//////////////////////////////////////////////////////////////////////////
	// Returns an overlay element by name if it exists. If the function fails
	// it returns NULL.
	//////////////////////////////////////////////////////////////////////////
	LkOverlayElement* GetElement( const char* _Element ) const;

	//////////////////////////////////////////////////////////////////////////
	// Instantiates a new element with name _Element (if that name is not taken yet),
	// of type _Type (if a type with such a name has been registered through
	// RegisterElementFactory. If the function fails, it returns NULL.
	//////////////////////////////////////////////////////////////////////////
	LkOverlayElement* CreateElement( const char* _Element, const char* _Type );

	const LkOverlayStyle* GetOverlayStyle() const;

	//////////////////////////////////////////////////////////////////////////
	// Compiles a list of the names of all registered element types. These
	// are the names that can be used to create an element using the
	// CreateElement function.
	//////////////////////////////////////////////////////////////////////////
	void GetRegisteredElementTypesList( std::list<std::string>& _Output ) const;

	//////////////////////////////////////////////////////////////////////////
	// Checks whether the passed element type name is a valid type to be used
	// when creating an element.
	//////////////////////////////////////////////////////////////////////////
	bool IsValidElementType( const char* _ElementType ) const;
private:
	LkOverlay( const LkOverlayStyle* _OverlayStyle );
	LkOverlay();
	~LkOverlay();

	LkOverlayElement* _GetElement( OverlayElementID _ElementID ) const;

	void Render();
	
	Elements m_Elements;
	static ElementFactories m_ElementFactories;
	const LkOverlayStyle* m_OverlayStyle;	// Overlay styles are managed by the OverlayManager class.
};

}

}

#endif