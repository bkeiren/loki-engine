#include "core/viewsystem/View.h"
#include "core/entitysystem/IEntity.h"

namespace loki
{

View::View( const char* _Name )	:
	m_Name(std::string(_Name)),
	m_LinkedEntity(0),
	m_ProjectionType(PT_PERSPECTIVE),
	m_FoV(45.0f),
	m_ZFar(500.0f),
	m_ZNear(0.1f),
	m_ProjectionMatrixIsDirty(false)
{
	SetPerspectiveProjection(45.0f, 64, 64);
}

View::View()
{
	ILLEGAL_CTOR_ERROR("View");
}

const std::string& View::GetName() const
{
	return m_Name;
}

IEntity* View::GetLinkedEntity() const
{
	return m_LinkedEntity;
}

void View::LinkTo( IEntity* _Link )
{
	m_LinkedEntity = _Link;
}

const mat4& View::GetProjectionMatrix()
{
	if (m_ProjectionMatrixIsDirty)
	{
		(m_ProjectionType == PT_PERSPECTIVE)?
			(m_ProjectionMatrix = math::gtc::matrix_transform::perspectiveFov(m_FoV, (float)m_Viewport.x, (float)m_Viewport.y, m_ZNear, m_ZFar)):
			(m_ProjectionMatrix = math::gtc::matrix_transform::ortho(m_OrthoViewport.x, m_OrthoViewport.y, m_OrthoViewport.z, m_OrthoViewport.w, m_ZNear, m_ZFar));
		m_ProjectionMatrixIsDirty = false;
	}
	return m_ProjectionMatrix;
}

mat4 View::GetViewMatrix()
{
	if (m_LinkedEntity)
	{
		Transform& t = m_LinkedEntity->GetTransform();
		return math::inverse(t.GetMatrix());
	}
	return mat4(1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

void View::SetFoV( float _FoV )
{
	m_FoV = _FoV;
	m_ProjectionMatrixIsDirty = true;
}

float View::GetFoV() const
{
	return m_FoV;
}

void View::SetZFar( float _ZFar )
{
	m_ZFar = _ZFar;
	m_ProjectionMatrixIsDirty = true;
}

float View::GetZFar() const
{
	return m_ZFar;
}

void View::SetZNear( float _ZNear )
{
	m_ZNear = _ZNear;
	m_ProjectionMatrixIsDirty = true;
}

float View::GetZNear() const
{
	return m_ZNear;
}

const int2& View::GetViewport() const
{
	return m_Viewport;
}

const vec4& View::GetOrthoViewport() const
{
	return m_OrthoViewport;
}

float View::GetAspectRatio() const
{
	return ((float)m_Viewport.x / (float)m_Viewport.y);
}

void View::SetOrthogonalProjection( float _Left /*= 0.0f*/, float _Right /*= 1.0f*/, float _Top /*= 0.0f*/, float _Bottom /*= 1.0f*/ )
{
	m_OrthoViewport.x = _Left;
	m_OrthoViewport.y = _Right;
	m_OrthoViewport.z = _Top;
	m_OrthoViewport.w = _Bottom;

	m_ProjectionType = PT_ORTHO;

	m_ProjectionMatrixIsDirty = true;
}

void View::SetPerspectiveProjection( float _FoV, int _Width, int _Height )
{
	SetFoV(_FoV);
	m_Viewport.x = _Width;
	m_Viewport.y = _Height;
	
	m_ProjectionType = PT_PERSPECTIVE;

	m_ProjectionMatrixIsDirty = true;
}

}