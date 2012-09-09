#pragma once

#ifndef IVIEW_H
#define IVIEW_H

namespace loki
{

class IView
{
public:
	virtual const std::string& GetName() const = 0;

protected:
	IView();
	virtual ~IView();
};

}

#endif