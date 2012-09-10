#include "core/viewsystem/ViewSystem.h"
#include "core/viewsystem/View.h"

namespace loki
{

ViewSystem::ViewSystem()	:
	m_ActiveView(0)
{

}

ViewSystem::~ViewSystem()
{
	m_Views.clear();
}

IView* ViewSystem::FindViewByName( const char* _ViewName ) const
{
	ViewsConstIter it = m_Views.find(std::string(_ViewName));
	if (it != m_Views.end())
	{
		return (*it).second;
	}
	return 0;
}

IView* ViewSystem::CreateView( const char* _ViewName )
{
	std::pair<ViewsIter, bool> res = m_Views.insert(ViewsPair(std::string(_ViewName), 0));
	if (!res.second)
	{
		LOG(VL_NORMAL, "ViewSystem::CreateView: Can't create view '%s', a view with that name already exists", _ViewName);
		return 0;
	}
	IView* view = new View(_ViewName);
	(*res.first).second = view;
}

void ViewSystem::DestroyView( const char* _ViewName )
{
	IView* view = FindViewByName(_ViewName);
	if (view)
	{
		m_Views.erase(std::string(_ViewName));
		delete (View*)view;
		if (m_ActiveView == view)
		{
			m_ActiveView = 0;
		}
	}
}

void ViewSystem::SetActiveView( IView* _View )
{
	assert(_View != 0);
	m_ActiveView = _View;
}

bool ViewSystem::SetActiveView( const char* _ViewName )
{
	IView* view = FindViewByName(_ViewName);
	if (view)
	{
		SetActiveView(view);
		return true;
	}
	return false;
}

IView* ViewSystem::GetActiveView() const
{
	return m_ActiveView;
}

}