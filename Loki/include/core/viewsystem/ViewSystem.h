#pragma once

#ifndef VIEWSYSTEM_H
#define VIEWSYSTEM_H

#include "core/viewsystem/IViewSystem.h"

namespace loki
{

class ViewSystem	: public IViewSystem
{
	CONTAINER_MACRO_HASH_MAP(std::string, IView*, Views)
public:
	ViewSystem();
	~ViewSystem();

	IView* FindViewByName( const char* _ViewName ) const;

	IView* CreateView( const char* _ViewName );

	void DestroyView( const char* _ViewName );

	void SetActiveView( IView* _View );

	bool SetActiveView( const char* _ViewName );

	IView* GetActiveView() const;
private:
	Views m_Views;
	IView* m_ActiveView;
};

}

#endif