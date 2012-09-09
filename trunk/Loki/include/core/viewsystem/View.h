#pragma once

#ifndef VIEW_H
#define VIEW_H

#include "core/viewsystem/IView.h"

namespace loki
{

class View	: public IView
{
	friend class ViewSystem;
public:
	const std::string& GetName() const;

private:
	View( const char* _Name );
	View();
	~View();

	std::string m_Name;
};

}

#endif