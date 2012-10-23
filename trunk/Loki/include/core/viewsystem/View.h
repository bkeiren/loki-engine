#pragma once

#ifndef VIEW_H
#define VIEW_H

#include "core/viewsystem/IView.h"

namespace loki
{

class View	: public IView
{
	friend class ViewSystem;
public:
	const std::string& GetName() const;

	Entity* GetLinkedEntity() const;
	void LinkTo( Entity* _Link );

	const mat4& GetProjectionMatrix();
	mat4 GetViewMatrix();

	void SetFoV( f32 _FoV );
	f32 GetFoV() const;

	void SetZFar( f32 _ZFar );
	f32 GetZFar() const;

	void SetZNear( f32 _ZNear );
	f32 GetZNear() const;

	const int2& GetViewport() const;	

	const vec4& GetOrthoViewport() const;

	f32 GetAspectRatio() const;

	void SetOrthogonalProjection( f32 _Left = 0.0f, f32 _Right = 1.0f, f32 _Top = 0.0f, f32 _Bottom = 1.0f );
	void SetPerspectiveProjection( f32 _FoV, int32 _Width, int32 _Height );
private:
	View( const char* _Name );
	View();
	~View();

	std::string m_Name;
	Entity* m_LinkedEntity;

	mat4 m_ProjectionMatrix;
	int2 m_Viewport;
	vec4 m_OrthoViewport;
	EProjectionType m_ProjectionType;
	f32 m_FoV;
	f32 m_ZFar;
	f32 m_ZNear;
	bool m_ProjectionMatrixIsDirty;
};

}

#endif