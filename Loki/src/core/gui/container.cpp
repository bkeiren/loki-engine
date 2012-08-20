#include "container.h"
#include "widget.h"
#include "textfield.h"
#include "button.h"
#include "list.h"
#include "tabgroup.h"
#include "radiobuttongroup.h"
#include "checkbox.h"
#include "progressbar.h"

namespace gui
{

Container::Container()
{

}

Container::~Container()
{

}

TextField* Container::AddTextField()
{
	TextField* widget = new TextField(this);
	m_Elements.push_back(widget);
	return widget;
}

Button* Container::AddButton()
{
	Button* widget = new Button(this);
	m_Elements.push_back(widget);
	return widget;
}

List* Container::AddList()
{
	List* widget = new List(this);
	m_Elements.push_back(widget);
	return widget;
}

TabGroup* Container::AddTabGroup()
{
	TabGroup* widget = new TabGroup(this);
	m_Elements.push_back(widget);
	return widget;
}

RadioButtonGroup* Container::AddRadioButtonGroup()
{
	RadioButtonGroup* widget = new RadioButtonGroup(this);
	m_Elements.push_back(widget);
	return widget;
}

CheckBox* Container::AddCheckBox()
{
	CheckBox* widget = new CheckBox(this);
	m_Elements.push_back(widget);
	return widget;
}

ProgressBar* Container::AddProgressBar()
{
	ProgressBar* widget = new ProgressBar(this);
	m_Elements.push_back(widget);
	return widget;
}

}