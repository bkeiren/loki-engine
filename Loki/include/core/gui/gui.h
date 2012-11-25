#pragma once

#ifndef GUI_H
#define GUI_H

#include "core/gui/Context.h"
#include "core/gui/Document.h"

namespace loki
{

namespace gui
{

class SystemInterface;
class RenderInterface;
class Context;

class GUI
{
	friend class LokiEngine;

	CONTAINER_MACRO_HASH_MAP(std::string, Context*, Contexts)
public:
	Context* CreateContext( const std::string& _Name, const int2& _Dimensions );
	void DestroyContext( const std::string& _Name );

	Context* GetMainContext();
private:
	GUI();
	~GUI();

	void _AttachInterfaces();

	ContextsIter _FindContextIterator( const std::string& _Name );
	Context* _FindContext( const std::string& _Name );

	void _DestroyContexts();

	void _UpdateContexts();
	void _RenderContexts();

	SystemInterface* m_SystemInterface;
	RenderInterface* m_RenderInterface;

	Context* m_MainContext;
	Contexts m_Contexts;
};

extern GUI* g_GUI;

}

}

#endif