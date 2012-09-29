#include "game.h"
#include "core/actor/camera/camera.h"
#include "core/game/level/level.h"
#include "core/actor/light/point/pointlight.h"
#include "core/actor/light/spot/spotlight.h"
#include "core/actor/light/directional/directionallight.h"
#include "core/actor/handle/handle.h"
#include "core/game/localization/localization.h"
#include "core/renderer/renderer.h"
#include "core/renderer/geometry/model/model.h"
#include "core/actor/components/rendercomponent/rendercomponent.h"
#include "core/actor/components/physicscomponent/physicscomponent.h"
#include "core/actor/pawn/pawn.h"
#include "mycontroller.h"
#include "core/renderer/material/material.h"
#include "core/script/squirrel/squirrel.h"
#include "core/renderer/effect/effectmanager.h"

#include "core/ui/overlay.h"
#include "core/ui/elements/button.h"
#include "core/ui/elements/checkbox.h"
#include "core/ui/elements/slider.h"

#include "core/engine.h"

#include "core/physics/physics.h"

#include "core/entitysystem/IEntitySystem.h"

#include "util/json/json.h"

using namespace loki;

physics::LkRigidBody* body = NULL;

void testcb( LkParticle* _Particle )
{
	//LOG(VL_NORMAL, "LOL");
}

MyGame::MyGame()
{

}

MyGame::~MyGame()
{

}

void MyGame::PreInit()
{

}

void testcallback1( const loki::ui::LkOverlayElement* _button )
{
	//LOG(VL_NORMAL, "MOVING");
	loki::g_Engine->GetGame()->GetLevel()->GetCurrentCamera()->SetFoVY( (((loki::ui::LkOverlaySlider*)_button)->GetSliderValue() * 130) + 30 );
}

void testcallback2( const loki::ui::LkOverlayElement* _button )
{
	LOG(VL_NORMAL, "RELEASED");
}

bool MyGame::Init()
{
	// Load a localization table.
	if (!game::g_Localization->LoadLocalizationTable("resources//localization//strings.loc"))
	{
		LOG(VL_ERROR, "Failed to load localization table");
	}
	//std::string str = game::g_Localization->GetLocalizedString("TestString");
	//game::g_Localization->SetLocale(game::LOCALE_NL);
	//std::string str2 = game::g_Localization->GetLocalizedString("p1wins");


	m_Level = new loki::game::LkLevel();
	m_Level->SetCurrentCamera(m_Level->SpawnCamera("Cam0", CAM_FREE));

	loki::LkCamera* cam = m_Level->GetCurrentCamera();
	loki::LkMovableComponent* cam_movcomp = cam->GetComponent<LkMovableComponent>();

	//cam_movcomp->SetPosition(vec3(0.0f, 0.0f, -40.0f));
	cam_movcomp->RotateY(180.0f);

	{
		// TODO: Remove this.

		LkPointLight* pointlight1 = m_Level->SpawnPointLight("PointLight1");
		pointlight1->SetRadius(20.0f);
		//pointlight1->SetPosition(vec3(-3.5f, -3.0f, -12.0f));
		LkMovableComponent* light_movcomp = pointlight1->GetComponent<LkMovableComponent>();
		light_movcomp->SetPosition(vec3(0.0f, 0.0f, 0.0f));
		pointlight1->SetColor(Color(0.2f, 0.2f, 1.0f));
		pointlight1->Disable();

		LkHandle<LkPointLight> handle = LkHandle<LkPointLight>(pointlight1);

		pointlight1 = m_Level->SpawnPointLight("PointLight2");
		light_movcomp = pointlight1->GetComponent<LkMovableComponent>();
		pointlight1->SetRadius(50.0f);
		light_movcomp->SetPosition(vec3(0.0f, 0.0f, 10.0f));
		pointlight1->SetColor(Color(1.0f, 0.2f, 0.2f));
		//pointlight1->Disable();
		

		pointlight1 = m_Level->SpawnPointLight("MassivePointLight");
		light_movcomp = pointlight1->GetComponent<LkMovableComponent>();
		pointlight1->SetRadius(60.0f);
		light_movcomp->SetPosition(vec3(0.0f, 0.0f, 0.0f));
		pointlight1->SetColor(Color(1.0f, 1.0f, 1.0f));
		//pointlight1->Disable();

		pointlight1 = m_Level->SpawnPointLight("PointLight3");
		light_movcomp = pointlight1->GetComponent<LkMovableComponent>();
		pointlight1->SetRadius(50.0f);
		light_movcomp->SetPosition(vec3(5.0f, 0.0f, 0.0f));
		pointlight1->SetColor(Color(0.0f, 1.0f, 0.2f));
		//pointlight1->Disable();
	}

	// Load an effect.
	loki::renderer::g_EffectManager->CreateEffectFromFile("resources//shaders//testshader.cgfx", "TestEffect");
	{
		LkPawn* pawn = NULL;
		LkMovableComponent* movcomp = NULL;
		LkRenderComponent* rendercomp = NULL;
		loki::renderer::LkMaterial* mtl = NULL;

		{
			pawn = m_Level->SpawnPawn("StanfordDragon");
			rendercomp = pawn->GetComponent<LkRenderComponent>();
			rendercomp->SetModel(new loki::renderer::LkModel("resources//models//torus.dae"));
			mtl = new loki::renderer::LkMaterial("TestEffect", "resources//textures//texture2.bmp", "resources//textures//texture2_normal.bmp", "resources//textures//texture2_spec.bmp", "resources//textures//texture2_emissive.bmp");
			mtl->SetShininess(100.0f);
			rendercomp->GetModel()->SetMaterial(mtl, 0);
			rendercomp->GetModel()->SetUVScale(vec2(3.0f, 3.0f));
			LkPhysicsComponent* phycomp = pawn->GetComponent<LkPhysicsComponent>();
			physics::RigidBodyInfo info;
			info.m_Shape = physics::CS_MESH_CONVEXTRIANGLEMESH;
			info.m_MeshData.m_Mesh = const_cast<renderer::LkMesh*>(rendercomp->GetModel()->GetMesh());
			info.m_Restitution = 0.75f;
			info.m_Mass = 100.0f;
			phycomp->CreateBodyFromInfo(info);
		}
		for (int i = 0; i < 10; ++i)
		{
			{
				std::string name = "StanfordDragon";
				char buff[8];
				_itoa_s(i, buff, 2);
				name += buff;
				pawn = m_Level->SpawnPawn(name.c_str());
				LkMovableComponent* movcomp = pawn->GetComponent<LkMovableComponent>();
				movcomp->SetPosition(vec3(i * 0.01f, 5 + i * 2, 0.0f));
				rendercomp = pawn->GetComponent<LkRenderComponent>();
				rendercomp->SetModel(new loki::renderer::LkModel("resources//models//torus.dae"));
				mtl = new loki::renderer::LkMaterial("TestEffect", "resources//textures//texture2.bmp", "resources//textures//texture2_normal.bmp", "resources//textures//texture2_spec.bmp", "resources//textures//texture2_emissive.bmp");
				mtl->SetShininess(100.0f);
				rendercomp->GetModel()->SetMaterial(mtl, 0);
				rendercomp->GetModel()->SetUVScale(vec2(3.0f, 3.0f));			
				LkPhysicsComponent* phycomp = pawn->GetComponent<LkPhysicsComponent>();
				physics::RigidBodyInfo info;
				info.m_Shape = physics::CS_MESH_CONVEXTRIANGLEMESH;
				info.m_MeshData.m_Mesh = const_cast<renderer::LkMesh*>(rendercomp->GetModel()->GetMesh());
				info.m_Restitution = 0.75f;
				info.m_Mass = 100.0f;
				phycomp->CreateBodyFromInfo(info);
			}
		}

		{
			pawn = m_Level->SpawnPawn("Pawn1");
			pawn->RemoveComponent<LkPhysicsComponent>();
			rendercomp = pawn->GetComponent<LkRenderComponent>();
			rendercomp->SetModel(new loki::renderer::LkModel("resources//models//cube.dae"));
			mtl = new loki::renderer::LkMaterial("TestEffect", "resources//textures//texture6.bmp", "resources//textures//texture6_normal.bmp", "resources//textures//texture6_specular.bmp");
			mtl->SetShininess(100.0f);
			rendercomp->GetModel()->SetMaterial(mtl, 0);
			rendercomp->GetModel()->SetUVScale(vec2(3.0f, 3.0f));
		}
	}

	{
		//DirectionalLight* directionallight1 = m_Level->SpawnDirectionalLight("DirectionalLight1");
		//directionallight1->SetDirection(normalize(vec3(1.0f, -1.0f, 0.0f)));
	}

	//ui::Overlay* overlay = ui::g_OverlayManager->CreateOverlay("Overlay0");
	//ui::OverlayElement* element = overlay->CreateElement("Button0", "button");

	/*
	physics::RigidBodyInfo info;
	info.m_Shape = physics::CS_MESH_CONVEXTRIANGLEMESH;
	info.m_MeshData.m_Mesh = const_cast<renderer::LkMesh*>(m_Level->GetPawn("StanfordDragon")->GetComponent<LkRenderComponent>()->GetModel()->GetMesh());
	info.m_Restitution = 0.75f;
	info.m_Mass = 100.0f;
	body = physics::g_Physics->AddRigidBody(info);

	for (int i = 0; i < 10; ++i)
	{
		info.m_InitialTransform = mat4(1.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 10.0f * (i + 1), 0.0f, 1.0f);
		physics::g_Physics->AddRigidBody(info);
	}*/

	/*
	if (!loki::ui::g_OverlayManager->LoadOverlayStyle("Style0", "resources//ui//style0.sty"))
	{
		return false;
	}
	loki::ui::LkOverlay* overlay = loki::ui::g_OverlayManager->CreateOverlay("Overlay0", "Style0");

	if (overlay)
	{
		// Set the default button size before creating any.
		//loki::ui::LkOverlayButton::SetDefaultSize(vec2(0.428f, 0.116f));

		loki::ui::LkOverlayButton* button = (loki::ui::LkOverlayButton*)overlay->CreateElement("Button0", "button");
		if (button)
		{
			button->SetRelativePosition(vec2(0.1f, 0.1f));
// 			button->RegisterCallback(loki::ui::OCB_MOUSE_ENTER, testcallback);
// 			button->RegisterCallback(loki::ui::OCB_MOUSE_LEFT_PRESSED, testcallback1);
// 			button->RegisterCallback(loki::ui::OCB_MOUSE_LEFT_RELEASED, testcallback2);
		}

		loki::ui::LkOverlayCheckBox* checkbox = (loki::ui::LkOverlayCheckBox*)overlay->CreateElement("Checkbox0", "checkbox");
		if (checkbox)
		{
			checkbox->SetRelativePosition(vec2(0.1f, 0.4f));
			//checkbox->SetRelativeSize(vec2(0.08f, 0.08f));
		}

		loki::ui::LkOverlaySlider* slider = (loki::ui::LkOverlaySlider*)overlay->CreateElement("Slider0", "slider");
		if (slider)
		{
			slider->SetRelativePosition(vec2(0.1f, 0.6f));	
 			slider->RegisterCallback(OCB_SLIDER_VALUE_MOVE, testcallback1);
// 			slider->RegisterCallback(OCB_SLIDER_VALUE_RELEASE, testcallback2);
		}
	}*/

	LkParticleSystemDescriptor descr;
	LkParticleSourceDescriptor& srcdescr = descr.AddSource();
	
	srcdescr.m_Lifetime = 2.0f;
	srcdescr.m_Quota = 256;
	srcdescr.m_SpawnRate = 128;
	srcdescr.m_Callback = &testcb;

	LkParticleSystem* ps = m_Level->SpawnParticleSystem(descr);


	IEntity* entity = g_EntitySystem->SpawnEntity("TestEntity");

// 	loki::util::JSONDocument* doc = loki::util::JSONDocument::Open("resources//test.json");
// 	if (doc)
// 	{
// 		loki::util::JSONValue& root = doc->GetRoot();
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

	if (KEY_RELEASED(KEY_R))
	{
		m_Level->GetPawn("StanfordDragon")->GetComponent<LkMovableComponent>()->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	}
	
	if (KEY_RELEASED(KEY_G))
	{
		physics::RigidBodyInfo info;
		info.m_Shape = physics::CS_SPHERE;
		info.m_Mass = 10.0f;
		info.m_SphereData.m_Radius = 0.5f;
		info.m_InitialTransform = m_Level->GetCurrentCamera()->GetComponent<LkMovableComponent>()->GetTransformation();
		physics::LkRigidBody* b = physics::g_Physics->AddRigidBody(info);
		vec3 force = vec3(-info.m_InitialTransform[2]) * 200.0f;
		b->ApplyCentralImpulse(force);
	}

	
	{
		LkPointLight* p0 = m_Level->GetPointLight("PointLight3");
		LkMovableComponent* comp0 = p0->GetComponent<LkMovableComponent>();
		LkPointLight* p1 = m_Level->GetPointLight("PointLight2");
		LkMovableComponent* comp1 = p1->GetComponent<LkMovableComponent>();
		static float f = 0.0f;
		f += 0.01f;
		comp0->SetPosition( vec3(cos(f) * 10, cos(f) * 10, sin(f) * 10) );
		comp1->SetPosition( vec3(cos(-f) * 10, sin(f) * 10, sin(-f) * 10) );
	}

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

	loki::LkMovableComponent* movcomp = m_Level->GetPawn("StanfordDragon")->GetComponent<loki::LkMovableComponent>();
	renderer::debug::DrawAxes(movcomp->GetPosition(), movcomp->GetOrientation(), 1.0f, true);
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