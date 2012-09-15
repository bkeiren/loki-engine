#pragma once

#ifndef IVIEW_H
#define IVIEW_H

namespace loki
{

class IEntity;

enum EProjectionType
{
	PT_PERSPECTIVE = 0,
	PT_ORTHO
};

class IView
{
public:
	virtual const std::string& GetName() const = 0;

	virtual IEntity* GetLinkedEntity() const = 0;
	virtual void LinkTo( IEntity* _Link ) = 0;

	virtual const mat4& GetProjectionMatrix() = 0;
	virtual mat4 GetViewMatrix() = 0;

	virtual void SetFoV( float _FoV ) = 0;
	virtual float GetFoV() const = 0;

	virtual void SetZFar( float _ZFar ) = 0;
	virtual float GetZFar() const = 0;

	virtual void SetZNear( float _ZNear ) = 0;
	virtual float GetZNear() const = 0;

	virtual const int2& GetViewport() const = 0;	

	virtual const vec4& GetOrthoViewport() const = 0;

	virtual float GetAspectRatio() const = 0;

	virtual void SetOrthogonalProjection( float _Left = 0.0f, float _Right = 1.0f, float _Top = 0.0f, float _Bottom = 1.0f ) = 0;
	virtual void SetPerspectiveProjection( float _FoV, int _Width, int _Height ) = 0;
protected:
	IView();
	virtual ~IView();
};

}

#endif