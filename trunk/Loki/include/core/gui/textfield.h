#pragma once

#ifndef TEXTFIELD_H
#define TEXTFIELD_H

namespace gui
{

class Container;

class TextField	: public Widget
{
public:

	void SetContent( std::string& _Content );
	const std::string& GetContent();
private:
	friend class Container;	// The Container class should be the only class able to instantiate and delete TextField objects.

	TextField( Container* _Parent );
	TextField();
	virtual ~TextField();

	std::string m_Content;
};

}

#endif