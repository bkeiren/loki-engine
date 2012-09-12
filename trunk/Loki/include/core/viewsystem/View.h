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

	IEntity* GetLinkedEntity() const;
	void LinkTo( IEntity* _Link );

	const glm::mat4& GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	void SetFoV( float _FoV );
	float GetFoV() const;

	void SetZFar( float _ZFar );
	float GetZFar() const;

	void SetZNear( float _ZNear );
	float GetZNear() const;

	const glm::int2& GetViewport() const;	

	const glm::vec4& GetOrthoViewport() const;

	float GetAspectRatio() const;

	void SetOrthogonalProjection( float _Left = 0.0f, float _Right = 1.0f, float _Top = 0.0f, float _Bottom = 1.0f );
	void SetPerspectiveProjection( float _FoV, int _Width, int _Height );
private:
	View( const char* _Name );
	View();
	~View();

	std::string m_Name;
	IEntity* m_LinkedEntity;

	glm::mat4 m_ProjectionMatrix;
	glm::int2 m_Viewport;
	glm::vec4 m_OrthoViewport;
	EProjectionType m_ProjectionType;
	float m_FoV;
	float m_ZFar;
	float m_ZNear;
	bool m_ProjectionMatrixIsDirty;
};

}

#endif