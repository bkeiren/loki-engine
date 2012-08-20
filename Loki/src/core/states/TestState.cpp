#include <GLEW\\glew.h>
#include "util/util.h"
#include "core/engine.h"
#include "core/states/Teststate.h"
#include "core/renderer/scene/scene.h"
#include "core/renderer/renderer.h"

using namespace loki;
using namespace loki::renderer;

State_Test::State_Test( const char* _Name )	:
	LkIState(_Name)
{

}

void State_Test::Init()
{
// 	m_SphereModel = new SphereModel(5, 10, 10, "resources//shaders//deferredShading.vert", "resources//shaders//deferredShading.frag");
// 	m_SphereModel->LoadTexture("resources//textures//earth.raw");
// 	m_SphereModel->SetPosition(glm::vec3(0, 0, 0));

	// Create a scene.
	//Renderer::SetScene(new Scene());
	//Renderer::GetScene()->AddModel(m_SphereModel);
}

void State_Test::ReInit()
{

}

void State_Test::Update()
{
	/*
	if (g_RenderWindow.GetInput().IsKeyDown(sf::Key::A))
	{
		std::cout << "A is pressed!" << std::endl;
		std::cout << "Framerate: " << 1.0f / g_RenderWindow.GetFrameTime() << " fps" << std::endl;
	}

	if (g_RenderWindow.GetInput().IsKeyDown(sf::Key::B))
	{
		std::cout << "B is pressed!" << std::endl;

		// Dump the log buffer to a file (clearing the file in the process).
		Logger::DumpToFile(Logger::DefaultLogFile, false);
	}

	sf::Vector3f musicpos = g_Music1.GetPosition();
	musicpos.y += 10.0f;
	g_Music1.SetPosition(musicpos);
	*/
}

void State_Test::Render()
{
	/*
	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);

	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, 1280, 720);
	gluPerspective(90.f, 1280.0f/720.0f, 1.f, 500.f);

	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw a cube.
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -200.0f);
		glRotatef(loki::g_Clock.GetElapsedTime() * 50, 1.0f, 0.0f, 0.0f);
		glRotatef(loki::g_Clock.GetElapsedTime() * 30, 0.0f, 1.0f, 0.0f);
		glRotatef(loki::g_Clock.GetElapsedTime() * 90, 0.0f, 0.0f, 1.0f);

		
		glBegin(GL_QUADS);		

		glVertex3f(-50.0f, -50.0f, -50.0f);
		glVertex3f(-50.0f,  50.0f, -50.0f);
		glVertex3f( 50.0f,  50.0f, -50.0f);
		glVertex3f( 50.0f, -50.0f, -50.0f);

		glVertex3f(-50.f, -50.f, 50.f);
		glVertex3f(-50.f,  50.f, 50.f);
		glVertex3f( 50.f,  50.f, 50.f);
		glVertex3f( 50.f, -50.f, 50.f);

		glVertex3f(-50.f, -50.f, -50.f);
		glVertex3f(-50.f,  50.f, -50.f);
		glVertex3f(-50.f,  50.f,  50.f);
		glVertex3f(-50.f, -50.f,  50.f);

		glVertex3f(50.f, -50.f, -50.f);
		glVertex3f(50.f,  50.f, -50.f);
		glVertex3f(50.f,  50.f,  50.f);
		glVertex3f(50.f, -50.f,  50.f);

		glVertex3f(-50.f, -50.f,  50.f);
		glVertex3f(-50.f, -50.f, -50.f);
		glVertex3f( 50.f, -50.f, -50.f);
		glVertex3f( 50.f, -50.f,  50.f);

		glVertex3f(-50.f, 50.f,  50.f);
		glVertex3f(-50.f, 50.f, -50.f);
		glVertex3f( 50.f, 50.f, -50.f);
		glVertex3f( 50.f, 50.f,  50.f);

		glEnd();
	}

	// Render the text.
	g_RenderWindow.Draw(g_Text);

	// Apply a shader effect to whatever is on the screen right now.
	g_ScreenColorizeEffect.SetParameter("color", 1.0f,	g_RenderWindow.GetInput().GetMouseX() / (float)g_RenderWindow.GetWidth(), 
		g_RenderWindow.GetInput().GetMouseY() / (float)g_RenderWindow.GetHeight());
	g_RenderWindow.Draw(g_ScreenColorizeEffect);
	*/
}

/*
void State_Test::HandleEvent( sf::Event _Event )
{

}
*/

void State_Test::CleanUp()
{

}