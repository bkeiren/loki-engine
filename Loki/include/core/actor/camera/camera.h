#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "core/actor/actor.h"

#include "core/actor/components/movablecomponent/movablecomponent.h"

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
	void LookAt( const vec3& _Target );

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

	const mat4& GetProjectionMatrix();
	mat4 GetViewMatrix();
	
	void SetFoVY( f32 _FoVY );
	f32 GetFoVY() const;
	
	void SetZFar( f32 _ZFar );
	f32 GetZFar() const;
	
	void SetZNear( f32 _ZNear );
	f32 GetZNear() const;

	void SetViewport( const int2& _Viewport );
	const int2& GetViewport() const;

	f32 GetAspectRatio() const;

	//////////////////////////////////////////////////////////////////////////
	// 0 = Perspective, 1 = Orthogonal.
	//////////////////////////////////////////////////////////////////////////
	void SetProjectionType( bool _Projection );

	//////////////////////////////////////////////////////////////////////////
	// Creates a vector in world-space that goes through the pixel indicated
	// by _ViewportCoordinates (In absolute pixels).
	//////////////////////////////////////////////////////////////////////////
	vec3 GetCameraToViewportVector( const int2& _ViewportCoordinates );

	virtual std::string ToString();
protected:
	LkCamera( const char* _Name, game::LkLevel* _Level );
	LkCamera();
	virtual ~LkCamera();

	virtual void _OnEvent( const LkEvent& _Event );
private:

	mat4 m_ProjectionMatrix;
	f32 m_FoVY;
	f32 m_ZFar;
	f32 m_ZNear;
	int2 m_Viewport;
	bool m_ProjectionType;	// 0 == Perspective, 1 == Orthogonal.
	bool m_ProjectionMatrixIsDirty;
};

}

#endif