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
	m_Viewport(int2(64, 64)),
	m_ProjectionType(PROJ_PERSPECTIVE),
	m_ProjectionMatrixIsDirty(true)
{
	AddComponent<LkMovableComponent>();

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

void LkCamera::LookAt( const vec3& _Target )
{
	LkMovableComponent* movcomp = GetComponent<LkMovableComponent>();

	mat4x4 mat = math::gtc::matrix_transform::lookAt(-movcomp->GetPosition(), -_Target,	GlobalY);
	movcomp->SetTransformation(mat);
	movcomp->SetOrientation(math::gtc::quaternion::quat_cast(mat));
}

void LkCamera::ApplyViewport()
{
	glViewport(0, 0, m_Viewport.x, m_Viewport.y);
}

void LkCamera::ApplyProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);	// Deprecated.

	(m_ProjectionType == PROJ_PERSPECTIVE)?
		(m_ProjectionMatrix = math::gtc::matrix_transform::perspective(m_FoVY, GetAspectRatio(), m_ZNear, m_ZFar)):
		(m_ProjectionMatrix = math::gtc::matrix_transform::ortho(0.0f, 1.0f, 0.0f, 1.0f, m_ZNear, m_ZFar));
	glLoadMatrixf((GLfloat*)&m_ProjectionMatrix);
}

void LkCamera::ApplyViewTransformation()
{
	glMatrixMode(GL_MODELVIEW);	// Deprecated.

	LkMovableComponent* comp = GetComponent<LkMovableComponent>();
	mat4x4 mat = math::inverse(comp->GetTransformation());
	glLoadMatrixf((f32*)&mat);	// Load the camera matrix.
}

const mat4& LkCamera::GetProjectionMatrix()
{
	if (m_ProjectionMatrixIsDirty)
	{
		(m_ProjectionType == PROJ_PERSPECTIVE)?
			(m_ProjectionMatrix = math::gtc::matrix_transform::perspective(m_FoVY, GetAspectRatio(), m_ZNear, m_ZFar)):
			(m_ProjectionMatrix = math::gtc::matrix_transform::ortho(0.0f, 1.0f, 0.0f, 1.0f, m_ZNear, m_ZFar));
		m_ProjectionMatrixIsDirty = false;
	}
	return m_ProjectionMatrix;
}

mat4 LkCamera::GetViewMatrix()
{
	LkMovableComponent* comp = GetComponent<LkMovableComponent>();
	mat4 mat = math::inverse(comp->GetTransformation());
	return mat;
}

void LkCamera::SetFoVY( f32 _FoVY )
{
	m_FoVY = _FoVY;
	m_ProjectionMatrixIsDirty = true;
}

f32 LkCamera::GetFoVY() const
{
	return m_FoVY;
}

void LkCamera::SetZFar( f32 _ZFar )
{
	m_ZFar = _ZFar;
	m_ProjectionMatrixIsDirty = true;
}

f32 LkCamera::GetZFar() const
{
	return m_ZFar;
}

void LkCamera::SetZNear( f32 _ZNear )
{
	m_ZNear = _ZNear;
	m_ProjectionMatrixIsDirty = true;
}

f32 LkCamera::GetZNear() const
{
	return m_ZNear;
}

void LkCamera::SetViewport( const int2& _Viewport )
{
	m_Viewport = _Viewport;

	if (m_Viewport.y == 0)
	{
		m_Viewport.y = 1;	// Don't want to divide by zero when we calculate the aspect ratio.
	}

	m_ProjectionMatrixIsDirty = true;
}

const int2& LkCamera::GetViewport() const
{
	return m_Viewport;
}

f32 LkCamera::GetAspectRatio() const
{
	return ((f32)m_Viewport.x / (f32)m_Viewport.y);
}

void LkCamera::SetProjectionType( bool _Projection )
{
	m_ProjectionType = _Projection;
	m_ProjectionMatrixIsDirty = true;
}

vec3 LkCamera::GetCameraToViewportVector( const int2& _ViewportCoordinates )
{
	int2 c = int2(_ViewportCoordinates.x, _ViewportCoordinates.y);

	mat4 viewmat = GetViewMatrix();
	mat4 projmat = GetProjectionMatrix();
	vec4 viewport = vec4(0.0f, 0.0f, m_Viewport.x, m_Viewport.y);
	vec3 a = math::gtc::matrix_transform::unProject(vec3((f32)c.x, (f32)c.y, 0.0f), 
														viewmat, 
														projmat, 
														viewport);
	vec3 b = math::gtc::matrix_transform::unProject(vec3((f32)c.x, (f32)c.y, 1.0f),
														viewmat,
														projmat,
														viewport);
	return math::normalize(b - a);
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