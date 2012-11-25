#include "core/gui/Document.h"
#include <Rocket/Core/ElementDocument.h>

namespace loki
{

namespace gui
{

Document::Document()	:
	m_RocketElementDocument(0)
{

}

Document::~Document()
{

}

void Document::Show()
{
	m_RocketElementDocument->Show();
}

void Document::Hide()
{
	m_RocketElementDocument->Hide();
}

void Document::SetHidden( bool _Hidden )
{
	(_Hidden) ? (Hide()) : (Show());
}

}

}