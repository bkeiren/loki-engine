#pragma once

#ifndef GUI_DOCUMENT_H
#define GUI_DOCUMENT_H

namespace Rocket
{

namespace Core
{

class ElementDocument;

}

}

namespace loki
{

namespace gui
{

class Document
{
	friend class Context;
public:
	void Show();
	void Hide();
	void SetHidden( bool _Hidden );

private:
	Document();
	~Document();

	Rocket::Core::ElementDocument* m_RocketElementDocument;
};

}

}

#endif