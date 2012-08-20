#pragma once

#ifndef COLOR_H
#define COLOR_H

//#include "math/glm/core/type_vec3.hpp"
//#include "math/glm/core/type_vec4.hpp"

namespace loki
{

typedef glm::vec3 ColorRGB;
typedef glm::vec4 ColorRGBA;
typedef ColorRGB Color;	// Color defaults to color without alpha.

}

#endif	// COLOR_H