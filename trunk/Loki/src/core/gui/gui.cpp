#include "core/gui/GUI.h"
#include <Rocket/Core/Core.h>
#include "core/gui/SystemInterface.h"
#include "core/gui/RenderInterface.h"
#include "core/renderer/renderer.h"

namespace loki
{

namespace gui
{

GUI* g_GUI = 0;

GUI::GUI()	:
	m_SystemInterface(0)
	,m_RenderInterface(0)
	,m_MainContext(0)
{
	m_SystemInterface = new SystemInterface();
	m_RenderInterface = new RenderInterface();

	_AttachInterfaces();

	Rocket::Core::Initialise();

	m_MainContext = new Context("MainGUIContext", int2(renderer::g_Renderer->GetRenderWidth(), renderer::g_Renderer->GetRenderHeight()));

	LOG(VL_ALWAYS, "GUI: GUI system initialized.");
}

GUI::~GUI()
{
	delete m_MainContext;
	m_MainContext = 0;

	_DestroyContexts();

	Rocket::Core::Shutdown();

	delete m_RenderInterface;
	delete m_SystemInterface;

	LOG(VL_ALWAYS, "GUI: GUI system terminated.");
}

Context* GUI::CreateContext( const std::string& _Name, const int2& _Dimensions )
{
	Context* context = _FindContext(_Name);
	if (context)
	{
		LOG(VL_ERROR, "GUI::CreateContext: A context named '%s' already exists.", _Name.c_str());
		return 0;
	}
	context = new Context(_Name, _Dimensions);
	m_Contexts[_Name] = context;
	return context;
}

void GUI::DestroyContext( const std::string& _Name )
{
	ContextsIter it = _FindContextIterator(_Name);
	Context* context = it->second;
	m_Contexts.erase(it);
	delete context;
}

Context* GUI::GetMainContext()
{
	return m_MainContext;
}

void GUI::_AttachInterfaces()
{
	Rocket::Core::SetSystemInterface((Rocket::Core::SystemInterface*)m_SystemInterface);
	Rocket::Core::SetRenderInterface((Rocket::Core::RenderInterface*)m_RenderInterface);
}

GUI::ContextsIter GUI::_FindContextIterator( const std::string& _Name )
{
	ContextsIter it = m_Contexts.find(_Name);
	return it;
}

Context* GUI::_FindContext( const std::string& _Name )
{
	ContextsIter it = _FindContextIterator(_Name);
	if (it == m_Contexts.end())
	{
		return 0;
	}
	return it->second;
}

void GUI::_DestroyContexts()
{
	for (ContextsIter it = m_Contexts.begin(); it != m_Contexts.end(); ++it)
	{
		delete it->second;
	}
	m_Contexts.clear();
}

void GUI::_UpdateContexts()
{
	m_MainContext->m_RocketContext->Update();
	for (ContextsIter it = m_Contexts.begin(); it != m_Contexts.end(); ++it)
	{
		it->second->m_RocketContext->Update();
	}
}

void GUI::_RenderContexts()
{
	m_MainContext->m_RocketContext->Render();
	for (ContextsIter it = m_Contexts.begin(); it != m_Contexts.end(); ++it)
	{
		it->second->m_RocketContext->Render();
	}
}

bool GUI::_ProcessKeyDown( char _Key )
{
	//m_MainContext->m_RocketContext->ProcessKeyDown(Rocket::Core::Input::KeyIdentifier)
	return true;
}

bool GUI::_ProcessKeyUp( char _Key )
{
	return true;
}

}

}