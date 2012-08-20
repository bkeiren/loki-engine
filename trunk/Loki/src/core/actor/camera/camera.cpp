#include <sstream>
#include <GLEW\\glew.h>
#include "core/actor/camera/camera.h"
#include "core/renderer/renderer.h"

using namespace loki;

LkCamera::LkCamera( const char* _Name, game::LkLevel* _Level )	:
	LkActor(_Name, _Level),
	m_FoVY(45.0f),
	m_ZFar(500.0f),
	m_ZNear(0.1f),
	m_Viewport(glm::int2(64, 64)),
	m_ProjectionType(PROJECTION_PERSPECTIVE),
	m_ProjectionMatrixIsDirty(true)
{
	AddComponent<LkMoveableComponent>();

	// By default, the viewport settings of a camera are copied from the renderer.
	m_Viewport.x = renderer::g_Renderer->GetRenderWidth();
	m_Viewport.y = renderer::g_Renderer->GetRenderHeight();
}

LkCamera::LkCamera()
{
	ILLEGAL_CTOR_ERROR("Camera");
}

LkCamera::~LkCamera()
{
	
}

void LkCamera::LookAt( const glm::vec3& _Target )
{
	LkMoveableComponent* movcomp = GetComponent<LkMoveableComponent>();

	glm::mat4x4 mat = glm::gtc::matrix_transform::lookAt(-movcomp->GetPosition(), -_Target, math::GlobalY);
	movcomp->SetTransformation(mat);
	movcomp->SetOrientation(glm::gtc::quaternion::quat_cast(mat));
}

void LkCamera::ApplyViewport()
{
	glViewport(0, 0, m_Viewport.x, m_Viewport.y);
}

void LkCamera::ApplyProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);	// Deprecated.

	(m_ProjectionType == PROJECTION_PERSPECTIVE)?
		(m_ProjectionMatrix = glm::gtc::matrix_transform::perspective(m_FoVY, GetAspectRatio(), m_ZNear, m_ZFar)):
		(m_ProjectionMatrix = glm::gtc::matrix_transform::ortho(0.0f, 1.0f, 0.0f, 1.0f, m_ZNear, m_ZFar));
	glLoadMatrixf((GLfloat*)&m_ProjectionMatrix);
}

void LkCamera::ApplyViewTransformation()
{
	glMatrixMode(GL_MODELVIEW);	// Deprecated.

	LkMoveableComponent* comp = GetComponent<LkMoveableComponent>();
	glm::mat4x4 mat = glm::inverse(comp->GetTransformation());
	glLoadMatrixf((float*)&mat);	// Load the camera matrix.
}

const glm::mat4& LkCamera::GetProjectionMatrix()
{
	if (m_ProjectionMatrixIsDirty)
	{
		(m_ProjectionType == PROJECTION_PERSPECTIVE)?
			(m_ProjectionMatrix = glm::gtc::matrix_transform::perspective(m_FoVY, GetAspectRatio(), m_ZNear, m_ZFar)):
			(m_ProjectionMatrix = glm::gtc::matrix_transform::ortho(0.0f, 1.0f, 0.0f, 1.0f, m_ZNear, m_ZFar));
		m_ProjectionMatrixIsDirty = false;
	}
	return m_ProjectionMatrix;
}

glm::mat4 LkCamera::GetViewMatrix()
{
	LkMoveableComponent* comp = GetComponent<LkMoveableComponent>();
	glm::mat4 mat = glm::inverse(comp->GetTransformation());
	return mat;
}

void LkCamera::SetFoVY( float _FoVY )
{
	m_FoVY = _FoVY;
	m_ProjectionMatrixIsDirty = true;
}

float LkCamera::GetFoVY() const
{
	return m_FoVY;
}

void LkCamera::SetZFar( float _ZFar )
{
	m_ZFar = _ZFar;
	m_ProjectionMatrixIsDirty = true;
}

float LkCamera::GetZFar() const
{
	return m_ZFar;
}

void LkCamera::SetZNear( float _ZNear )
{
	m_ZNear = _ZNear;
	m_ProjectionMatrixIsDirty = true;
}

float LkCamera::GetZNear() const
{
	return m_ZNear;
}

void LkCamera::SetViewport( const glm::int2& _Viewport )
{
	m_Viewport = _Viewport;

	if (m_Viewport.y == 0)
	{
		m_Viewport.y = 1;	// Don't want to divide by zero when we calculate the aspect ratio.
	}

	m_ProjectionMatrixIsDirty = true;
}

const glm::int2& LkCamera::GetViewport() const
{
	return m_Viewport;
}

float LkCamera::GetAspectRatio() const
{
	return ((float)m_Viewport.x / (float)m_Viewport.y);
}

void LkCamera::SetProjectionType( bool _Projection )
{
	m_ProjectionType = _Projection;
	m_ProjectionMatrixIsDirty = true;
}

glm::vec3 LkCamera::GetCameraToViewportVector( const glm::int2& _ViewportCoordinates )
{
	glm::int2 c = glm::int2(_ViewportCoordinates.x, _ViewportCoordinates.y);

	glm::mat4 viewmat = GetViewMatrix();
	glm::mat4 projmat = GetProjectionMatrix();
	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, m_Viewport.x, m_Viewport.y);
	glm::vec3 a = glm::gtc::matrix_transform::unProject(glm::vec3((float)c.x, (float)c.y, 0.0f), 
														viewmat, 
														projmat, 
														viewport);
	glm::vec3 b = glm::gtc::matrix_transform::unProject(glm::vec3((float)c.x, (float)c.y, 1.0f),
														viewmat,
														projmat,
														viewport);
	return glm::normalize(b - a);
}

std::string LkCamera::ToString()
{
	std::stringstream str;
	str << LkActor::ToString() <<		\
		   "\nPerspective:\t[ " << m_ProjectionMatrix[0].x << " " << m_ProjectionMatrix[0].y << " " << m_ProjectionMatrix[0].z << " " << m_ProjectionMatrix[0].w << " ]" <<	\
		   "\n\t\t[ " << m_ProjectionMatrix[1].x << " " << m_ProjectionMatrix[1].y << " " << m_ProjectionMatrix[1].z << " " << m_ProjectionMatrix[1].w << " ]" <<	\
		   "\n\t\t[ " << m_ProjectionMatrix[2].x << " " << m_ProjectionMatrix[2].y << " " << m_ProjectionMatrix[2].z << " " << m_ProjectionMatrix[2].w << " ]" <<	\
		   "\n\t\t[ " << m_ProjectionMatrix[3].x << " " << m_ProjectionMatrix[3].y << " " << m_ProjectionMatrix[3].z << " " << m_ProjectionMatrix[3].w << " ]";

	return str.str();
}

void LkCamera::_OnEvent( const LkEvent& _Event )
{
	
}