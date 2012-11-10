#include "core/entitysystem/component/default/CameraComponent.h"
#include "core/renderer/renderer.h"
#include "core/entitysystem/Entity.h"

namespace loki
{

namespace components
{

#define CLIP_NEAR		x
#define CLIP_FAR		y

#define VIEWPORT_LEFT		x
#define VIEWPORT_TOP		y
#define VIEWPORT_RIGHT		z
#define VIEWPORT_BOTTOM		w

CameraComponent* CameraComponent::m_ActiveCamera = 0;

CameraComponent::CameraComponent()	:
	m_ClippingPlanes(vec2(0.1f, 1000.0f)),
	m_ViewPort(vec4(0.0f, 0.0f, 1.0f, 1.0f)),
	m_ProjectionType(PROJECTION_PERSPECTIVE),
	m_FieldOfView(60.0f),
	m_ProjectionMatrix(IDENTITYMAT4),
	m_ProjectionMatrixIsDirty(true)
{
	_ComputeProjectionMatrix();

	if (!m_ActiveCamera)
	{
		m_ActiveCamera = this;
	}
}

CameraComponent::~CameraComponent()
{

}

f32 CameraComponent::GetNearPlane() const
{
	return m_ClippingPlanes.CLIP_NEAR;
}

f32 CameraComponent::GetFarPlane() const
{
	return m_ClippingPlanes.CLIP_FAR;
}

const vec2& CameraComponent::GetClippingPlanes() const
{
	return m_ClippingPlanes;
}

void CameraComponent::SetNearPlane( f32 _Distance )
{
	m_ClippingPlanes.CLIP_NEAR = _Distance;
	m_ProjectionMatrixIsDirty = true;
}

void CameraComponent::SetFarPlane( f32 _Distance )
{
	m_ClippingPlanes.CLIP_FAR = _Distance;
	m_ProjectionMatrixIsDirty = true;
}

void CameraComponent::SetClippingPlanes( const vec2& _ClippingPlanes )
{
	m_ClippingPlanes = _ClippingPlanes;
	m_ProjectionMatrixIsDirty = true;
}

const vec4& CameraComponent::GetViewPort() const
{
	return m_ViewPort;
}

void CameraComponent::SetViewPort( const vec4& _ViewPort )
{
	m_ViewPort = _ViewPort;
	m_ProjectionMatrixIsDirty = true;
}

CameraComponent::EProjectionType CameraComponent::GetProjectionType() const
{
	return m_ProjectionType;
}

void CameraComponent::SetProjectionType( EProjectionType _ProjectionType )
{
	m_ProjectionType = _ProjectionType;
	m_ProjectionMatrixIsDirty = true;
}

f32 CameraComponent::GetFieldOfView() const
{
	return m_FieldOfView;
}

void CameraComponent::SetFieldOfView( f32 _FieldOfView )
{
	m_FieldOfView = _FieldOfView;
	m_ProjectionMatrixIsDirty = true;
}

void CameraComponent::LookAt( const vec3& _Target )
{
	GetTransform().LookAt(_Target);
}

const mat4& CameraComponent::GetProjectionMatrix()
{
	if (m_ProjectionMatrixIsDirty)
	{
		_ComputeProjectionMatrix();
	}
	return m_ProjectionMatrix;
}

mat4 CameraComponent::GetViewMatrix()
{
	mat4 m = GetEntity()->GetTransform().GetMatrix();
	m[0] = -m[0];	// This is required in order to make the camera actually point in the same way as
	m[2] = -m[2];	// all other entities are oriented (Meaning, Z equals forward). This isn't strictly required
					// but it helps in making controlling the camera a lot more intuitive.
	m = math::inverse(m);
	return m;
}

void CameraComponent::Activate()
{
	m_ActiveCamera = this;
}

bool CameraComponent::IsActiveCamera() const
{
	return (GetActiveCamera() == this);
}

CameraComponent* CameraComponent::GetActiveCamera()
{
	return m_ActiveCamera;
}

void CameraComponent::_HandleEvent( const LkEvent& _Event )
{

}

void CameraComponent::_Init()
{

}

void CameraComponent::_Terminate()
{

}

void CameraComponent::_ComputeProjectionMatrix()
{
	f32 width = renderer::g_Renderer->GetRenderWidth() * m_ViewPort.VIEWPORT_RIGHT;
	f32 height = renderer::g_Renderer->GetRenderHeight() * m_ViewPort.VIEWPORT_BOTTOM;

	(m_ProjectionType == PROJECTION_PERSPECTIVE)?
		(m_ProjectionMatrix = math::gtc::matrix_transform::perspective(m_FieldOfView, width / height, GetNearPlane(), GetFarPlane())):
		(m_ProjectionMatrix = math::gtc::matrix_transform::ortho(m_ViewPort.VIEWPORT_LEFT, m_ViewPort.VIEWPORT_RIGHT, m_ViewPort.VIEWPORT_BOTTOM, m_ViewPort.VIEWPORT_TOP, GetNearPlane(), GetFarPlane()));
	m_ProjectionMatrixIsDirty = false;
}

#undef VIEWPORT_BOTTOM
#undef VIEWPORT_RIGHT
#undef VIEWPORT_TOP
#undef VIEWPORT_LEFT

#undef CLIP_FAR
#undef CLIP_NEAR

}

}