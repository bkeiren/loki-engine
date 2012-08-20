#pragma once

#ifndef GUITYPES_H
#define GUITYPES_H

//////////////////////////////////////////////////////////////////////////
// Forward declarations.
//////////////////////////////////////////////////////////////////////////
namespace gui
{

//////////////////////////////////////////////////////////////////////////
// The way the gui namespace handles vectors ensures that clients can
// use whichever vector class they desire, as long as these classes
// contain public data members named 'x', 'y' and 'z'.
// Clients can enable the use of their own classes by 
// #defining 'GUI_VECTOR2_TYPE' and 'GUI_VECTOR3_TYPE'.
// If these are both not defined, the gui namespace Vec2 and Vec3 structs
// will be used. If either one is defined, only that one will be used
// while the other will remain defined as the gui namespace version.
// If both are defined, they are simply both used.
//////////////////////////////////////////////////////////////////////////

// Issue a message if both GUI_VECTOR2_TYPE and GUI_VECTOR3_TYPE are not defined.
// The client may not be aware that the default structs are being used and might
// appreciate being informed of the fact that he/she can override the type.
#if (!(defined GUI_VECTOR2_TYPE) && !(defined GUI_VECTOR3_TYPE))
	#pragma message("GUI_VECTOR2_TYPE and GUI_VECTOR3_TYPE are undefined. Default structs will be used.")
#endif

#if (!(defined GUI_VECTOR2_TYPE))
	struct Vec2 { float x, y; };
	#ifdef GUI_VECTOR3_TYPE
		#pragma message("GUI_VECTOR3_TYPE is defined, but 2-component vector type is not. Is this intentional?")
	#endif
#else
	typedef GUI_VECTOR2_TYPE Vec2;
#endif

#if (!(defined GUI_VECTOR3_TYPE))
	struct Vec3 { float x, y, z; };
#ifdef GUI_VECTOR2_TYPE
	#pragma message("GUI_VECTOR2_TYPE is defined, but 3-component vector type is not. Is this intentional?")
#endif
#else
	typedef GUI_VECTOR3_TYPE Vec3;
#endif
	
class Resizeable;

class Container;
	class Canvas;
	class Window;
	class Tab;

class Widget;
	class TextField;
	class Button;
	class List;
	class TabGroup;
	class RadioButtonGroup;
	class CheckBox;
	class ProgressBar;

class ListItem;
class RadioButton;

}

#endif