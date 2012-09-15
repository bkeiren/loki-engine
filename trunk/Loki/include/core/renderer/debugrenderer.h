#pragma once

#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

namespace loki
{

namespace renderer
{

namespace debug
{

void DrawItems( const mat4& _ProjectionMatrix, const mat4& _ViewMatrix );

//////////////////////////////////////////////////////////////////////////
// Draw a line in 3D.
//////////////////////////////////////////////////////////////////////////
void DrawLine3D( const vec3& _From, const vec3& _To, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f) );

//////////////////////////////////////////////////////////////////////////
// Draw a line in 2D (Screen).
//////////////////////////////////////////////////////////////////////////
void DrawLine2D( const vec2& _From, const vec2& _To, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f) );

//////////////////////////////////////////////////////////////////////////
// Draw a sphere.
//////////////////////////////////////////////////////////////////////////
void DrawSphere( const vec3& _Pos, float _Radius, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f), bool _Wire = true );

//////////////////////////////////////////////////////////////////////////
// Draw a cube.
//////////////////////////////////////////////////////////////////////////
void DrawCube( const vec3& _Pos, float _Size, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f), bool _Wire = true );

//////////////////////////////////////////////////////////////////////////
// Draw an icosahedron.
//////////////////////////////////////////////////////////////////////////
void DrawIcosahedron( const vec3& _Pos, float _Size, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f), bool _Wire = true );

//////////////////////////////////////////////////////////////////////////
// Draw a cone.
//////////////////////////////////////////////////////////////////////////
void DrawCone( const vec3& _Pos, float _Base, float _Height, vec3& _Direction, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f), bool _Wire = true );

//////////////////////////////////////////////////////////////////////////
// Draw a cylinder.
//////////////////////////////////////////////////////////////////////////
void DrawCylinder( const vec3& _Pos, float _Radius, float _Height, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f), bool _Wire = true );

//////////////////////////////////////////////////////////////////////////
// Draw a disk.
//////////////////////////////////////////////////////////////////////////
void DrawDisk3D( const vec3& _Pos, float _Radius, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f) );
void DrawDisk2D( const vec3& _Pos, float _Radius, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f) );

//////////////////////////////////////////////////////////////////////////
// Draw a partial disk.
//////////////////////////////////////////////////////////////////////////
void DrawPartialDisk3D( const vec3& _Pos, float _Radius, float _StartAngle, float _Angle, bool _DepthTest = true, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f) );
void DrawPartialDisk2D( const vec3& _Pos, float _Radius, float _StartAngle, float _Angle, const vec3& _Color = vec3(1.0f, 1.0f, 1.0f) );

//////////////////////////////////////////////////////////////////////////
// Draw the X, Y and Z axes of a coordinate system using arrows.
// The colours of the axes are fixed: X = red, Y = green, Z = blue.
//////////////////////////////////////////////////////////////////////////
void DrawAxes( const vec3& _Pos, const mat3& _Axes, float _Scale = 1.0f, bool _DepthTest = true, bool _Wire = true );
void DrawAxes( const vec3& _Pos, const quat& _LocalForwardOrientation, float _Scale = 1.0f, bool _DepthTest = true, bool _Wire = true );

}

}

}

#endif