#include "core/actor/camera/pathcam/pathcam.h"

#include "core/renderer/renderer.h"

namespace loki
{

LkCamera* CameraFactoryPathCam( const char* _Name, game::LkLevel* _Level )
{
	return new LkPathCam(_Name, _Level);
}

LkPathCam::LkPathCam( const char* _Name, game::LkLevel* _Level )	:
	LkCamera(_Name, _Level)
{
	SubscribeToEvent(EVENT_ONUPDATE);

	m_PathSegment = 0;
	m_PathPosition = 0.0f;
	m_PathControlPoints.push_back(glm::vec3(0.0f, -10.0f, -10.0f));
	m_PathControlPoints.push_back(glm::vec3(-10.0f, -20.0f, 0.0f));
	m_PathControlPoints.push_back(glm::vec3(-10.0f, -10.0f, 10.0f));
	m_PathControlPoints.push_back(glm::vec3(-5.0f, 0.0f, 0.0f));

	m_PathControlPoints.push_back(glm::vec3(0.0f, 0.0f, 5.0f));
	m_PathControlPoints.push_back(glm::vec3(5.0f, 0.0f, 5.0f));
	m_PathControlPoints.push_back(glm::vec3(-5.0f, 0.0f, 0.0f));
}

LkPathCam::LkPathCam()
{
	ILLEGAL_CTOR_ERROR("PathCam");
}

LkPathCam::~LkPathCam()
{

}

void LkPathCam::_OnEvent( const LkEvent& _Event )
{
	LkCamera::_OnEvent(_Event);

	switch (_Event.GetEventType())
	{
	case EVENT_ONUPDATE:
		{
			LkMovableComponent* comp = GetComponent<LkMovableComponent>();

			m_PathPosition += 0.01f;
			if (m_PathPosition >= 1.0f)
			{
				m_PathPosition = 0.0f;
				++m_PathSegment;
			}

			if (m_PathSegment < m_PathControlPoints.size() - 3)
			{
				comp->SetPosition(glm::gtx::spline::catmullRom(m_PathControlPoints[m_PathSegment], 
															   m_PathControlPoints[m_PathSegment + 1],
															   m_PathControlPoints[m_PathSegment + 2],
															   m_PathControlPoints[m_PathSegment + 3],
															   m_PathPosition));
			}
			else
			{
				m_PathSegment = 0;
				m_PathPosition = 0.0f;
			}

			LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

			if (m_PathSegment == 0)
			{
				SetFoVY(m_PathPosition * 25.0f + m_PathPosition * 20.0f);
			}
			else
			{
				SetFoVY(45.0f);
			}

			break;
		}
	}
}

}