#pragma once

#ifndef GUI_H
#define GUI_H

#include <list>
#include <string>
#include "gui_types.h"

#include "resizeable.h"
#include "container.h"
#include "canvas.h"
#include "window.h"
#include "tab.h"
#include "widget.h"
#include "button.h"
#include "textfield.h"
#include "checkbox.h"
#include "list.h"
#include "progressbar.h"
#include "radiobuttongroup.h"
#include "tabgroup.h"
#include "listitem.h"
#include "radiobutton.h"

namespace gui
{

// Creates a blank canvas which can then be accessed to
// create windows and widgets on it.
// This is the first function that should be called in order to successfully
// use the GUI system.
Canvas* CreateCanvas();

}

#endif