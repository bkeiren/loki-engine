#include "game.h"
#include "core/game/localization/localization.h"
#include "core/renderer/renderer.h"
#include "core/script/squirrel/squirrel.h"
#include "core/graphics/effect/EffectManager.h"

#include "core/ui/overlay.h"
#include "core/ui/elements/button.h"
#include "core/ui/elements/checkbox.h"
#include "core/ui/elements/slider.h"

#include "core/engine.h"

#include "core/physics/physics.h"

#include "core/entitysystem/EntitySystem.h"

#include "util/json/json.h"

#include "core/entitysystem/component/default/PhysicsComponent.h"
#include "core/entitysystem/component/default/MeshRenderer.h"
#include "core/graphics/Mesh.h"
#include "core/entitysystem/component/default/CameraComponent.h"

#include "core/entitysystem/component/default/scripts/FreeCam.h"
#include "core/entitysystem/component/default/scripts/OrbitCam.h"
#include "core/entitysystem/component/default/Light.h"

#include "core/game/Sky.h"
#include "core/graphics/TextureCube.h"

#include "scripts/SimpleController.h"
#include "scripts/SimpleRotationController.h"

#include "util/dragdrophandler/DragDropHandler.h"
#include "core/window/Window.h"

#include "core/audio/audio.h"

#include "core/rect/Rect.h"

#include "core/gui/GUI.h"

#include "core/graphics/Octree.h"
#include "core/graphics/OctreeSpecializations.h"

using namespace loki;

MyGame::MyGame()
{
	
}

MyGame::~MyGame()
{
	
}

void MyGame::PreInit()
{

}

bool MyGame::Init()
{
// 	{
// 		Entity* entity0 = g_EntitySystem->SpawnEntity("TestEntity2374896");
// 
// 		Octree<components::MeshRenderer*>* octree = new Octree<components::MeshRenderer*>(2048.0f, 10);	
// 		octree->Insert(entity0->InstantiateComponent<loki::components::MeshRenderer>());
// 		octree->Insert(entity0->InstantiateComponent<loki::components::MeshRenderer>());
// 		octree->Insert(entity0->InstantiateComponent<loki::components::MeshRenderer>());
// 		octree->Insert(entity0->InstantiateComponent<loki::components::MeshRenderer>());
// 		octree->Insert(entity0->InstantiateComponent<loki::components::MeshRenderer>());
// 		int dbg = 0;
// 	}

	// Load a localization table.
	if (!game::g_Localization->LoadLocalizationTable("resources//localization//strings.loc"))
	{
		LOG(VL_ERROR, "Failed to load localization table");
	}
	//std::string str = game::g_Localization->GetLocalizedString("TestString");
	//game::g_Localization->SetLocale(game::LOCALE_NL);
	//std::string str2 = game::g_Localization->GetLocalizedString("p1wins");

// 	LkParticleSystemDescriptor descr;
// 	LkParticleSourceDescriptor& srcdescr = descr.AddSource();
// 	
// 	srcdescr.m_Lifetime = 2.0f;
// 	srcdescr.m_Quota = 256;
// 	srcdescr.m_SpawnRate = 128;
// 	srcdescr.m_Callback = &testcb;
// 
// 	LkParticleSystem* ps = m_Level->SpawnParticleSystem(descr);

	loki::components::MeshRenderer* rc = 0;
	loki::components::PhysicsComponent* pc = 0;
	loki::components::Light* lc = 0;

	Entity* entity0 = g_EntitySystem->SpawnEntity("TestEntity");
	lc = entity0->InstantiateComponent<loki::components::Light>();
	lc->SetRange(100.0f);
	lc->SetSpotAngle(50.0f);
	lc->SetLightType(components::Light::LIGHT_SPOT);
	lc->SetCookie(graphics::Texture2D::Load("resources//textures//stainedglass.bmp", false));
	pc = entity0->InstantiateComponent<loki::components::PhysicsComponent>();
	rc = entity0->InstantiateComponent<loki::components::MeshRenderer>();

	rc->SetMesh("resources//models//sphere_small.obj");
	rc->SetMaterial("resources//lma//test.lma", 0);
	//rc->SetModel(graphics::Model::Load("resources//lmo//test.lmo"));

	//entity0->GetTransform().Translate(vec3(10.0f, 0.0f, 10.0f));
	entity0->GetTransform().position += vec3(10.0f, 0.0f, 10.0f);	// Ooooh, nice property.
	SimpleController* simplecntrl = entity0->InstantiateComponent<SimpleController>();

	physics::RigidBodyInfo info;
	info.m_Shape = physics::CS_MESH_CONVEXTRIANGLEMESH;
	info.m_MeshData.m_Mesh = const_cast<graphics::Mesh*>(rc->GetMesh());
	info.m_Restitution = 0.1f;
	info.m_Mass = 0.001f;
	pc->CreateBodyFromInfo(info);

	Entity* entity1 = g_EntitySystem->SpawnEntity("BoxEntity");
	entity1->InstantiateComponent<loki::components::MeshRenderer>();
	rc = entity1->GetComponent<loki::components::MeshRenderer>();
	//rc->SetModel(graphics::Model::Load("resources//lmo//cube.lmo"));
	rc->SetMesh("resources//models//cube.dae");
	rc->SetMaterial("resources//lma//cube.lma");

	Entity* camEntity = g_EntitySystem->SpawnEntity("Main Camera");
	camEntity->InstantiateComponent<loki::components::CameraComponent>()->Activate();
	camEntity->InstantiateComponent<loki::components::scripts::FreeCam>()->Enable();
	camEntity->InstantiateComponent<loki::components::scripts::OrbitCam>()->Disable();
	lc = camEntity->InstantiateComponent<loki::components::Light>();
	lc->SetLightType(loki::components::Light::LIGHT_SPOT);
	lc->SetRange(30.0f);
	lc->SetSpotAngle(50.0f);


	SimpleRotationController* rcntrl = 0;

	Entity* entity12 = g_EntitySystem->SpawnEntity("Light0");
	lc = entity12->InstantiateComponent<loki::components::Light>();
	rcntrl = entity12->InstantiateComponent<SimpleRotationController>();
	rcntrl->SetRotationVector(UP);
	rcntrl->SetVector(vec3(4.0f, 0.0f, 0.0f));
	lc->SetRange(40.0f);
	//lc->SetColor(ColorRGB(1.0f, 0.2f, 0.0f));
	lc->SetCookie(graphics::TextureCube::Load("resources//textures//cubemap3.bmp"));
	entity12->GetTransform().Translate(vec3(-10.0f, -5.0f, 0.0f));

// 	Entity* entity13 = g_EntitySystem->SpawnEntity("Light1");
// 	lc = entity13->InstantiateComponent<loki::components::Light>();
// 	rcntrl = entity13->InstantiateComponent<SimpleRotationController>();
// 	rcntrl->SetRotationVector(FORWARD);
// 	rcntrl->SetVector(vec3(-8.0f, 0.0f, 0.0f));
// 	lc->SetRange(40.0f);
// 	lc->SetColor(ColorRGB(0.0f, 0.2f, 1.0f));
// 	entity13->GetTransform().Translate(vec3(-2.0f, -8.0f, 3.0f));
// 
// 	Entity* entity14 = g_EntitySystem->SpawnEntity("Light2");
// 	lc = entity14->InstantiateComponent<loki::components::Light>();
// 	rcntrl = entity14->InstantiateComponent<SimpleRotationController>();
// 	rcntrl->SetRotationVector(SIDE);
// 	rcntrl->SetVector(vec3(0.0f, 5.0f, 0.0f));
// 	lc->SetRange(40.0f);
// 	lc->SetColor(ColorRGB(0.4f, 0.4f, 0.4f));
// 	entity14->GetTransform().Translate(vec3(-10.0f, -15.0f, 10.0f));

	
	game::Sky::SetCubeMap(graphics::TextureCube::Load("resources//textures//cubemap2.bmp"));

	for (int i = 0; i < 10; ++i)
	{
		std::stringstream str;
		str << "TorusEntity";
		str << i;
		
		Entity* TorusEntity = g_EntitySystem->SpawnEntity(str.str().c_str());
		rc = TorusEntity->InstantiateComponent<loki::components::MeshRenderer>();
		//rc->SetModel(graphics::Model::Load("resources//lmo//torus.lmo"));
		rc->SetMesh("resources//models//torus.dae");
		rc->SetMaterial("resources//lma//torus.lma");
		
		pc = TorusEntity->InstantiateComponent<loki::components::PhysicsComponent>();
		info.m_Shape = physics::CS_MESH_CONVEXHULL;
		info.m_MeshData.m_Mesh = const_cast<graphics::Mesh*>(rc->GetMesh());
		info.m_Restitution = 0.1f;
		info.m_Mass = 0.001f;
		pc->CreateBodyFromInfo(info);
		
		
		TorusEntity->GetTransform().Translate(vec3(0.0f, i * 10.0f, 0.0f));
	}

// 	loki::util::general::JSONDocument* doc = loki::util::general::JSONDocument::Open("resources//test.json");
// 	if (doc)
// 	{
// 		loki::util::general::JSONValue& root = doc->GetRoot();
// 
// 		std::string test = root["jeoma"].AsString();
// 
// 		int a = root["class"]["member_one"].AsInt();
// 		int b = root["class"]["member_two"].AsInt();
// 		
// 		root["class"]["member_one"] = 16;
// 
// 		std::string output;
// 		doc->WriteToString(output);
// 
// 		doc->Close();
// 	}

	loki::Rect r(10.0f, 12.0f, 200.0f, 8.0f);
	r.position = vec2(20.0f, 0.0f);
	r.dimensions = vec2(100.0f, 100.0f);

	
// 	gui::Context* context = gui::g_GUI->GetMainContext();
// 	gui::Document* doc = context->LoadDocument("resources//gui//demo.rml");

	return true;
}

void MyGame::PostInit()
{

}

void MyGame::PostInitFail()
{

}

void MyGame::PreUpdate()
{

}

void MyGame::Update()
{
// 	if (KEY_RELEASED(KEY_H))
// 	{
// 		g_Squirrel->RunScript("resources//scripts//squirrelscript2.nut");
// 	}
// 
// 	if (KEY_RELEASED(KEY_P))
// 	{
// 		loki::renderer::g_Renderer->ToggleWireframe();
// 	}

// 	if (KEY_RELEASED(KEY_R))
// 	{
// 		m_Level->GetPawn("StanfordDragon")->GetComponent<LkMovableComponent>()->SetPosition(vec3(0.0f, 0.0f, 0.0f));
// 	}
	
	if (KEY_RELEASED(KEY_G))
	{
		physics::RigidBodyInfo info;
		info.m_Shape = physics::CS_SPHERE;
		info.m_Mass = 10.0f;
		info.m_SphereData.m_Radius = 0.5f;
		info.m_InitialTransform = components::CameraComponent::GetActiveCamera()->GetEntity()->GetTransform().GetMatrix();
		physics::LkRigidBody* b = physics::g_Physics->AddRigidBody(info);
		vec3 force = vec3(info.m_InitialTransform[2]) * 200.0f;
		b->ApplyCentralImpulse(force);
	}

	// Toggle between freecam and orbit cam.
	if (KEY_RELEASED('C'))
	{
		Entity* cam = g_EntitySystem->FindEntityByName("Main Camera");
		if (cam)
		{
			loki::components::scripts::FreeCam* freecam = cam->GetComponent<loki::components::scripts::FreeCam>();
			loki::components::scripts::OrbitCam* orbitcam = cam->GetComponent<loki::components::scripts::OrbitCam>();

			if (freecam && orbitcam)
			{
				if (freecam->IsEnabled())
				{
					orbitcam->Enable();
					freecam->Disable();
				}
				else
				{
					freecam->Enable();
					orbitcam->Disable();
				}
			}
		}
	}

	while (util::system::DragDropHandler::GetNumFiles() > 0)
	{
		util::system::DragDropHandler::DroppedFileInfo Info;
		util::system::DragDropHandler::QueryNewest(Info, true);

		LOG(VL_NORMAL, "Dropped file: %s (@ [%i, %i])", Info.m_File.c_str(), Info.m_CursorPosition.x, Info.m_CursorPosition.y);
	}
	
// 	{
// 		LkPointLight* p0 = m_Level->GetPointLight("PointLight3");
// 		LkMovableComponent* comp0 = p0->GetComponent<LkMovableComponent>();
// 		LkPointLight* p1 = m_Level->GetPointLight("PointLight2");
// 		LkMovableComponent* comp1 = p1->GetComponent<LkMovableComponent>();
// 		static float f = 0.0f;
// 		f += 0.01f;
// 		comp0->SetPosition( vec3(cos(f) * 10, cos(f) * 10, sin(f) * 10) );
// 		comp1->SetPosition( vec3(cos(-f) * 10, sin(f) * 10, sin(-f) * 10) );
// 	}

	/*
	static vec3 p0 = vec3(0.0f, 0.0f, 0.0f);
	static vec3 p1 = vec3(5.0f, 5.0f, 0.0f);
	static vec3 p2 = vec3(10.0f, 0.0f, 0.0f);
	static vec3 p3 = vec3(15.0f, -5.0f, 0.0f);
	
	float d = 0.01f;
	for (float f = 0.0f; f < 1.0f; f += d)
	{
		vec3 pos0 = gtx::spline::catmullRom(p0, p1, p2, p3, f);
		vec3 pos1 = gtx::spline::catmullRom(p0, p1, p2, p3, f + d);

		renderer::debug::DrawLine3D(pos0, pos1, false, vec3(0.0f, 1.0f, 0.0f));
	}
	*/

	//loki::LkMovableComponent* movcomp = m_Level->GetPawn("StanfordDragon")->GetComponent<loki::LkMovableComponent>();
	//renderer::debug::DrawAxes(movcomp->GetPosition(), movcomp->GetOrientation(), 1.0f, true);

	if (g_Input->IsReleased(KEY_ESCAPE))
	{
		g_Engine->RequestExit();
	}
}

void MyGame::PostUpdate()
{
	
}

void MyGame::PreShutdown()
{

}

void MyGame::Shutdown()
{

}

void MyGame::PostShutdown()
{

}

void MyGame::PreLevelLoad()
{

}

void MyGame::PostLevelLoad()
{

}