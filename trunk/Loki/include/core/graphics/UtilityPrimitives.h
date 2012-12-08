#pragma once

#ifndef UTILITY_PRIMITIVES_H
#define UTILITY_PRIMITIVES_H

namespace loki
{

namespace graphics
{

// Points towards negative-z.
void DrawCone( f32 _Base, f32 _Height, int32 _Slices );

// Actually subdivides at least once, even when _SubDivision is 0.
void DrawIcoSphere( f32 _Radius, int32 _Subdivisions = 1 );

void DrawQuad( const math::vec2& _Dimensions );

}

}

#endif