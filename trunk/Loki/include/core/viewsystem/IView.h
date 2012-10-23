#pragma once

#ifndef IVIEW_H
#define IVIEW_H

namespace loki
{

class Entity;

enum EProjectionType
{
	PT_PERSPECTIVE = 0,
	PT_ORTHO
};

class IView
{
public:
	virtual const std::string& GetName() const = 0;

	virtual Entity* GetLinkedEntity() const = 0;
	virtual void LinkTo( Entity* _Link ) = 0;

	virtual const mat4& GetProjectionMatrix() = 0;
	virtual mat4 GetViewMatrix() = 0;

	virtual void SetFoV( f32 _FoV ) = 0;
	virtual f32 GetFoV() const = 0;

	virtual void SetZFar( f32 _ZFar ) = 0;
	virtual f32 GetZFar() const = 0;

	virtual void SetZNear( f32 _ZNear ) = 0;
	virtual f32 GetZNear() const = 0;

	virtual const int2& GetViewport() const = 0;	

	virtual const vec4& GetOrthoViewport() const = 0;

	virtual f32 GetAspectRatio() const = 0;

	virtual void SetOrthogonalProjection( f32 _Left = 0.0f, f32 _Right = 1.0f, f32 _Top = 0.0f, f32 _Bottom = 1.0f ) = 0;
	virtual void SetPerspectiveProjection( f32 _FoV, int32 _Width, int32 _Height ) = 0;
protected:
	IView();
	virtual ~IView();
};

}

#endif