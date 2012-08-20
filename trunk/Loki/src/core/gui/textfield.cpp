#include <string>
#include "widget.h"
#include "textfield.h"

namespace gui
{

TextField::TextField( Container* _Parent )	:
	Widget(_Parent)
{
	m_Content = "SampleText";
}

TextField::TextField()
{

}

TextField::~TextField()
{

}

void TextField::SetContent( std::string& _Content )
{
	m_Content = _Content;
}

const std::string& TextField::GetContent()
{
	return m_Content;
}

}