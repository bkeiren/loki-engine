#pragma once

#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "core/entitysystem/component/Component.h"

namespace loki
{

namespace components
{

class CameraComponent	: public Component
{
public:
	DECLARE_COMPONENT_TYPEINFO(CameraComponent)	// Required!

	CameraComponent();
	~CameraComponent();

	enum EProjectionType
	{
		PROJECTION_PERSPECTIVE = 0,
		PROJECTION_ORTHOGRAPHIC
	};

	f32 GetNearPlane() const;
	f32 GetFarPlane() const;
	const vec2& GetClippingPlanes() const;
	void SetNearPlane( f32 _Distance );
	void SetFarPlane( f32 _Distance );
	void SetClippingPlanes( const vec2& _ClippingPlanes );	// x: left, y: top, z: right, w: bottom.

	const vec4& GetViewPort() const;
	void SetViewPort( const vec4& _ViewPort );

	EProjectionType GetProjectionType() const;
	void SetProjectionType( EProjectionType _ProjectionType );

	f32 GetFieldOfView() const;
	void SetFieldOfView( f32 _FieldOfView );

	void LookAt( const vec3& _Target );

	const mat4& GetProjectionMatrix();

	//////////////////////////////////////////////////////////////////////////
	// Simply returns the inverse of the entity's transform matrix.
	//////////////////////////////////////////////////////////////////////////
	mat4 GetViewMatrix();

	//////////////////////////////////////////////////////////////////////////
	// Activates this camera as the currently active camera.
	//////////////////////////////////////////////////////////////////////////
	void Activate();

	static CameraComponent* GetActiveCamera();
private:
	void _OnEvent( const LkEvent& _Event );
	void _Init();
	void _Terminate();

	void _ComputeProjectionMatrix();

	vec2 m_ClippingPlanes;
	vec4 m_ViewPort;
	EProjectionType m_ProjectionType;
	f32 m_FieldOfView;
	mat4 m_ProjectionMatrix;
	bool m_ProjectionMatrixIsDirty;

	static CameraComponent* m_ActiveCamera;
};

}

}

#endif