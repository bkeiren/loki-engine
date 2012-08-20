#include "gui.h"
#include <vector>

namespace gui
{

namespace	// Anonymous namespace.
{
	const unsigned int m_MaxCanvasCount = 5;	// Any number that suffices, really.
	std::vector<Canvas*> m_Canvasses;
}

Canvas* CreateCanvas()
{
	if (m_Canvasses.size() >= m_MaxCanvasCount)
	{
		// Output warning indicating no canvas was created because the maximum number
		// of canvasses has been reached.
		return 0;
	}

	Canvas* canvas = new Canvas();
	m_Canvasses.push_back(canvas);
	return canvas;
}

}