#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "core/actor/actor.h"

#include "core/actor/components/moveablecomponent/moveablecomponent.h"

#define PROJECTION_PERSPECTIVE	false
#define PROJECTION_ORTHOGONAL	true

namespace loki
{

namespace renderer
{
	class LkScene;
}

namespace game
{
	class LkLevel;
}

class LkCamera	: public LkActor
{
	friend class game::LkLevel;
public:
	void LookAt( const glm::vec3& _Target );

	//////////////////////////////////////////////////////////////////////////
	// Applies this camera's viewport settings.
	//////////////////////////////////////////////////////////////////////////
	void ApplyViewport();

	//////////////////////////////////////////////////////////////////////////
	// Applies this camera's projection matrix.
	// NOTE: Leaves glMatrixMode in GL_PROJECTION!
	//////////////////////////////////////////////////////////////////////////
	void ApplyProjectionMatrix();

	//////////////////////////////////////////////////////////////////////////
	// Applies the camera's transformation.
	//////////////////////////////////////////////////////////////////////////
	void ApplyViewTransformation();

	const glm::mat4& GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	
	void SetFoVY( float _FoVY );
	float GetFoVY() const;
	
	void SetZFar( float _ZFar );
	float GetZFar() const;
	
	void SetZNear( float _ZNear );
	float GetZNear() const;

	void SetViewport( const glm::int2& _Viewport );
	const glm::int2& GetViewport() const;

	float GetAspectRatio() const;

	//////////////////////////////////////////////////////////////////////////
	// 0 = Perspective, 1 = Orthogonal.
	//////////////////////////////////////////////////////////////////////////
	void SetProjectionType( bool _Projection );

	//////////////////////////////////////////////////////////////////////////
	// Creates a vector in world-space that goes through the pixel indicated
	// by _ViewportCoordinates (In absolute pixels).
	//////////////////////////////////////////////////////////////////////////
	glm::vec3 GetCameraToViewportVector( const glm::int2& _ViewportCoordinates );

	virtual std::string ToString();
protected:
	LkCamera( const char* _Name, game::LkLevel* _Level );
	LkCamera();
	virtual ~LkCamera();

	virtual void _OnEvent( const LkEvent& _Event );
private:

	glm::mat4 m_ProjectionMatrix;
	float m_FoVY;
	float m_ZFar;
	float m_ZNear;
	glm::int2 m_Viewport;
	bool m_ProjectionType;	// 0 == Perspective, 1 == Orthogonal.
	bool m_ProjectionMatrixIsDirty;
};

}

#endif