#include "core/renderer/debugrenderer.h"
#include <vector>
#include <GLEW\\glew.h>
#include <GL\\glut.h>

#define DBG_DRAW_ENABLED

namespace loki
{

namespace renderer
{

namespace debug
{

namespace
{
	
struct DbgDrawItem 
{

#define _DBGDRAW_TYPE_NONE				-1	// Will not be drawn. Only for internal use.

#define DBGDRAW_TYPE_LINE3D				0
#define DBGDRAW_TYPE_LINE2D				1
#define DBGDRAW_TYPE_SPHERE_WIRE		2
#define DBGDRAW_TYPE_SPHERE_SOLID		3
#define DBGDRAW_TYPE_CUBE_WIRE			4
#define DBGDRAW_TYPE_CUBE_SOLID			5
#define DBGDRAW_TYPE_ICOSAHEDRON_WIRE	6
#define DBGDRAW_TYPE_ICOSAHEDRON_SOLID	7
#define DBGDRAW_TYPE_CONE_WIRE			8
#define DBGDRAW_TYPE_CONE_SOLID			9
#define DBGDRAW_TYPE_CYLINDER_WIRE		10
#define DBGDRAW_TYPE_CYLINDER_SOLID		11
#define DBGDRAW_TYPE_DISK				12
#define DBGDRAW_TYPE_DISK_PARTIAL		13

	int32 m_Type;
	vec3 m_Vec0;
	vec3 m_Vec1;
	f32 m_Val0;
	f32 m_Val1;
	vec3 m_Color;
	bool m_DepthTest;
};

//////////////////////////////////////////////////////////////////////////
// The 2D and 3D debug draw items are separated to avoid having to make
// unnecessary matrix mode switches.
//////////////////////////////////////////////////////////////////////////
#ifdef DBG_DRAW_ENABLED
const uint32 DbgDrawItemsMaxCount = 2048;
#else
const uint32 DbgDrawItemsMaxCount = 2048;
#endif
typedef std::vector<DbgDrawItem>		DbgDrawItems;
typedef DbgDrawItems::iterator			DbgDrawItemsIter;
typedef DbgDrawItems::const_iterator	DbgDrawItemsConstIter;

DbgDrawItems DbgDrawItems3D = DbgDrawItems(DbgDrawItemsMaxCount);
DbgDrawItems DbgDrawItems2D = DbgDrawItems(DbgDrawItemsMaxCount);

uint32 DbgDrawItems3DCounter = 0;
uint32 DbgDrawItems2DCounter = 0;

}

void DrawItems( const mat4& _ProjectionMatrix, const mat4& _ViewMatrix )
{
#ifdef DBG_DRAW_ENABLED
	glDepthMask(false);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(math::value_ptr(_ProjectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(math::value_ptr(_ViewMatrix));
	glPushMatrix();
	//for (DbgDrawItemsIter it = DbgDrawItems3D.begin(); it != DbgDrawItems3D.end(); ++it)
	for (uint32 i = 0; i < DbgDrawItems3DCounter; ++i)
	{
		//DbgDrawItem* item = &(*it);
		DbgDrawItem* item = &DbgDrawItems3D[i];

		if (item->m_DepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
		glColor3f(item->m_Color.r, item->m_Color.g, item->m_Color.b);

		switch (item->m_Type)
		{
		case DBGDRAW_TYPE_LINE3D:
			{
				glBegin(GL_LINE_STRIP);
				glVertex3f(item->m_Vec0.x, item->m_Vec0.y, item->m_Vec0.z);
				glVertex3f(item->m_Vec1.x, item->m_Vec1.y, item->m_Vec1.z);
				glEnd();

				break;
			}
		case DBGDRAW_TYPE_SPHERE_WIRE:
		case DBGDRAW_TYPE_SPHERE_SOLID:
			{
				glPushMatrix();
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(item->m_Vec0.x, item->m_Vec0.y, item->m_Vec0.z);
				(item->m_Type == DBGDRAW_TYPE_SPHERE_WIRE)?
					(glutWireSphere(item->m_Val0, 15, 15)):
					(glutSolidSphere(item->m_Val0, 15, 15));
				glPopMatrix();
				break;
			}
		case DBGDRAW_TYPE_CUBE_WIRE:
		case DBGDRAW_TYPE_CUBE_SOLID:
			{
				glPushMatrix();
				glTranslatef(item->m_Vec0.x, item->m_Vec0.y, item->m_Vec0.z);
				(item->m_Type == DBGDRAW_TYPE_CUBE_WIRE)?
					(glutWireCube(item->m_Val0)):
					(glutSolidCube(item->m_Val0));
				glPopMatrix();
				break;
			}
		case DBGDRAW_TYPE_ICOSAHEDRON_WIRE:
		case DBGDRAW_TYPE_ICOSAHEDRON_SOLID:
			{
				glPushMatrix();
				glScalef(item->m_Val0, item->m_Val0, item->m_Val0);
				glTranslatef(item->m_Vec0.x, item->m_Vec0.y, item->m_Vec0.z);
				(item->m_Type == DBGDRAW_TYPE_ICOSAHEDRON_WIRE)?
					(glutWireIcosahedron()):
					(glutSolidIcosahedron());
				glPopMatrix();
				break;
			}
		case DBGDRAW_TYPE_CONE_WIRE:
		case DBGDRAW_TYPE_CONE_SOLID:
			{
				mat4 m = math::inverse(math::gtc::matrix_transform::lookAt(vec3(0.0f, 0.0f, 0.0f), -item->m_Vec1, UNIT_Y));

				glPushMatrix();
				m[0] = -m[0];
				m[1] = -m[1];
				m[2] = -m[2];
				m[3] = vec4(vec3(m[2]) * -item->m_Val1, 1.0f);
				glTranslatef(item->m_Vec0.x, item->m_Vec0.y, item->m_Vec0.z);
				glMultMatrixf((GLfloat*)&m);
				(item->m_Type == DBGDRAW_TYPE_CONE_WIRE)?
					(glutWireCone(item->m_Val0, item->m_Val1, 15, 1)):
					(glutSolidCone(item->m_Val0, item->m_Val1, 15, 1));
				glPopMatrix();
				break;
			}
		case DBGDRAW_TYPE_CYLINDER_WIRE:
		case DBGDRAW_TYPE_CYLINDER_SOLID:
			{
				(item->m_Type == DBGDRAW_TYPE_CYLINDER_WIRE)?
					(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)):
					(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
				glDisable(GL_CULL_FACE);

				glPushMatrix();
				glTranslatef(item->m_Vec0.x, item->m_Vec0.y, item->m_Vec0.z);
				static GLUquadric* q = gluNewQuadric();
				gluCylinder(q, item->m_Val0, item->m_Val0, item->m_Val1, 15, 1);
				glPopMatrix();

				glEnable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				break;
			}
		case DBGDRAW_TYPE_DISK:
		case DBGDRAW_TYPE_DISK_PARTIAL:
			{
				glDisable(GL_CULL_FACE);

				glPushMatrix();
				glTranslatef(item->m_Vec0.x, item->m_Vec0.y, item->m_Vec0.z);
				static GLUquadric* q = gluNewQuadric();
				(item->m_Type == DBGDRAW_TYPE_DISK)?
					(gluDisk(q, 0, item->m_Val0, 50, 1)):
					(gluPartialDisk(q, 0, item->m_Val0, max((int32)((item->m_Vec1.y / 360) * 50), 5), 1, item->m_Vec1.x, item->m_Vec1.y));
				glPopMatrix();

				glEnable(GL_CULL_FACE);
				break;
			}
		}

		item->m_Type = _DBGDRAW_TYPE_NONE;
	}
	glPopMatrix();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, 1.0f, 1.0f, 0.0f, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//for (DbgDrawItemsIter it = DbgDrawItems2D.begin(); it != DbgDrawItems2D.end(); ++it)
	for (uint32 i = 0; i < DbgDrawItems2DCounter; ++i)
	{
		//DbgDrawItem* item = &(*it);
		DbgDrawItem* item = &DbgDrawItems2D[i];

		if (item->m_DepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
		glColor3f(item->m_Color.r, item->m_Color.g, item->m_Color.b);

		switch (item->m_Type)
		{
		case DBGDRAW_TYPE_LINE2D:
			{
				glBegin(GL_LINE_STRIP);
				glVertex2f(item->m_Vec0.x, item->m_Vec0.y);
				glVertex2f(item->m_Vec1.x, item->m_Vec1.y);
				glEnd();

				break;
			}
		case DBGDRAW_TYPE_DISK:
		case DBGDRAW_TYPE_DISK_PARTIAL:
			{
				glDisable(GL_CULL_FACE);

				glPushMatrix();
				glTranslatef(item->m_Vec0.x, item->m_Vec0.y, item->m_Vec0.z);
				glScalef(1.0f, -1.0f, 1.0f);
				static GLUquadric* q = gluNewQuadric();
				(item->m_Type == DBGDRAW_TYPE_DISK)?
					(gluDisk(q, 0, item->m_Val0, 50, 1)):
					(gluPartialDisk(q, 0, item->m_Val0, max((int32)((item->m_Vec1.y / 360) * 50), 5), 1, item->m_Vec1.x, item->m_Vec1.y));
				glPopMatrix();

				glEnable(GL_CULL_FACE);
				break;
			}
		}

		item->m_Type = _DBGDRAW_TYPE_NONE;
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(true);

	glColor3f(1.0f, 1.0f, 1.0f);	// Here because otherwise the sky isn't drawn for some reason... x)

#endif

	DbgDrawItems3DCounter = 0;
	DbgDrawItems2DCounter = 0;
}


void DrawLine3D(const vec3& _From, const vec3& _To, bool _DepthTest /*= true*/, const vec3& _Color /* = vec3 */)
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems3DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems3D[DbgDrawItems3DCounter];
		item->m_Type = DBGDRAW_TYPE_LINE3D;
		item->m_Vec0 = _From;
		item->m_Vec1 = _To;
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems3DCounter;
	}
#endif
}

void DrawLine2D(const vec2& _From, const vec2& _To, bool _DepthTest /*= true*/, const vec3& _Color /* = vec3 */)
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems2DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems2D[DbgDrawItems2DCounter];
		item->m_Type = DBGDRAW_TYPE_LINE2D;
		item->m_Vec0 = vec3(_From.x, _From.y, 0.0f);
		item->m_Vec1 = vec3(_To.x, _To.y, 0.0f);
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems2DCounter;
	}
#endif
}

void DrawSphere(const vec3& _Pos, f32 _Radius, bool _DepthTest /*= true*/, const vec3& _Color /* = vec3 */, bool _Wire /*= true*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems3DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems3D[DbgDrawItems3DCounter];
		item->m_Type = (_Wire)?(DBGDRAW_TYPE_SPHERE_WIRE):(DBGDRAW_TYPE_SPHERE_SOLID);
		item->m_Vec0 = _Pos;
		item->m_Val0 = _Radius;
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems3DCounter;
	}
#endif
}

void DrawCube(const vec3& _Pos, f32 _Size, bool _DepthTest /*= true*/, const vec3& _Color /* = vec3 */, bool _Wire /*= true*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems3DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems3D[DbgDrawItems3DCounter];
		item->m_Type = (_Wire)?(DBGDRAW_TYPE_CUBE_WIRE):(DBGDRAW_TYPE_CUBE_SOLID);
		item->m_Vec0 = _Pos;
		item->m_Val0 = _Size;
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems3DCounter;
	}
#endif
}

void DrawIcosahedron(const vec3& _Pos, f32 _Size, bool _DepthTest /* = true */, const vec3& _Color /* = vec3 */, bool _Wire /*= true*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems3DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems3D[DbgDrawItems3DCounter];
		item->m_Type = (_Wire)?(DBGDRAW_TYPE_ICOSAHEDRON_WIRE):(DBGDRAW_TYPE_ICOSAHEDRON_SOLID);
		item->m_Vec0 = _Pos;
		item->m_Val0 = _Size;
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems3DCounter;
	}
#endif
}

void DrawCone(const vec3& _Pos, f32 _Base, f32 _Height, vec3& _Direction, bool _DepthTest /* = true */, const vec3& _Color /* = vec3 */, bool _Wire /*= true*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems3DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems3D[DbgDrawItems3DCounter];
		item->m_Type = (_Wire)?(DBGDRAW_TYPE_CONE_WIRE):(DBGDRAW_TYPE_CONE_SOLID);
		item->m_Vec0 = _Pos;
		item->m_Vec1 = math::normalize(_Direction);
		item->m_Val0 = _Base;
		item->m_Val1 = _Height;
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems3DCounter;
	}
#endif
}

void DrawCylinder(const vec3& _Pos, f32 _Radius, f32 _Height, bool _DepthTest /* = true */, const vec3& _Color /* = vec3 */, bool _Wire /*= true*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems3DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems3D[DbgDrawItems3DCounter];
		item->m_Type = (_Wire)?(DBGDRAW_TYPE_CYLINDER_WIRE):(DBGDRAW_TYPE_CYLINDER_SOLID);
		item->m_Vec0 = _Pos;
		item->m_Val0 = _Radius;
		item->m_Val1 = _Height;
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems3DCounter;
	}
#endif
}

void DrawDisk3D( const vec3& _Pos, f32 _Radius, bool _DepthTest /*= true*/, const vec3& _Color /*= vec3(1.0f, 1.0f, 1.0f)*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems3DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems3D[DbgDrawItems3DCounter];
		item->m_Type = DBGDRAW_TYPE_DISK;
		item->m_Vec0 = _Pos;
		item->m_Val0 = _Radius;
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems3DCounter;
	}
#endif
}

void DrawDisk2D( const vec3& _Pos, f32 _Radius, const vec3& _Color /*= vec3(1.0f, 1.0f, 1.0f)*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems2DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems2D[DbgDrawItems2DCounter];
		item->m_Type = DBGDRAW_TYPE_DISK;
		item->m_Vec0 = _Pos;
		item->m_Val0 = _Radius;
		item->m_Color = _Color;
		item->m_DepthTest = false;
		++DbgDrawItems2DCounter;
	}
#endif
}

void DrawPartialDisk3D( const vec3& _Pos, f32 _Radius, f32 _StartAngle, f32 _Angle, bool _DepthTest /*= true*/, const vec3& _Color /*= vec3(1.0f, 1.0f, 1.0f)*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems3DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems3D[DbgDrawItems3DCounter];
		item->m_Type = DBGDRAW_TYPE_DISK_PARTIAL;
		item->m_Vec0 = _Pos;
		item->m_Val0 = _Radius;
		item->m_Vec1 = vec3(_StartAngle, _Angle, 0.0f);
		item->m_Color = _Color;
		item->m_DepthTest = _DepthTest;
		++DbgDrawItems3DCounter;
	}
#endif
}

void DrawPartialDisk2D( const vec3& _Pos, f32 _Radius, f32 _StartAngle, f32 _Angle, const vec3& _Color /*= vec3(1.0f, 1.0f, 1.0f)*/ )
{
#ifdef DBG_DRAW_ENABLED
	if (DbgDrawItems2DCounter < DbgDrawItemsMaxCount)
	{
		DbgDrawItem* item = &DbgDrawItems2D[DbgDrawItems2DCounter];
		item->m_Type = DBGDRAW_TYPE_DISK_PARTIAL;
		item->m_Vec0 = _Pos;
		item->m_Val0 = _Radius;
		item->m_Vec1 = vec3(_StartAngle, _Angle, 0.0f);
		item->m_Color = _Color;
		item->m_DepthTest = false;
		++DbgDrawItems2DCounter;
	}
#endif
}

void DrawAxes( const vec3& _Pos, const mat3& _Axes, f32 _Scale /* = 1.0f */, bool _DepthTest /* = true */, bool _Wire /* = true*/ )
{
	f32 cone_base = 0.085f;
	f32 cone_height = 0.25f;
	vec3 forward = _Axes[0];
	vec3 up = _Axes[1];
	vec3 side = _Axes[2];
	
	//////////////////////////////////////////////////////////////////////////
	// X-axis
	DrawLine3D(_Pos, _Pos + (forward * _Scale), _DepthTest, FORWARD);
	DrawCone(_Pos + (forward * _Scale), cone_base * _Scale, cone_height * _Scale, -forward, _DepthTest, FORWARD, _Wire);

	//////////////////////////////////////////////////////////////////////////
	// Y-axis
	DrawLine3D(_Pos, _Pos + (up * _Scale), _DepthTest, UP);
	DrawCone(_Pos + (up * _Scale), cone_base * _Scale, cone_height * _Scale, -up, _DepthTest, UP, _Wire);

	//////////////////////////////////////////////////////////////////////////
	// Z-axis
	DrawLine3D(_Pos, _Pos + (side * _Scale), _DepthTest, SIDE);
	DrawCone(_Pos + (side * _Scale), cone_base * _Scale, cone_height * _Scale, -side, _DepthTest, SIDE, _Wire);
}

void DrawAxes( const vec3& _Pos, const quat& _LocalForwardOrientation, f32 _Scale /* = 1.0f*/, bool _DepthTest /* = true*/, bool _Wire /* = true*/ )
{
	DrawAxes(_Pos, math::gtc::quaternion::mat3_cast(_LocalForwardOrientation), _Scale, _DepthTest, _Wire);
}

}

}

}