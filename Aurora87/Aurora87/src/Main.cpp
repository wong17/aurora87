#include "Engine/Core/Application.h"

#include "MultipleEntitiesTestLayer.h"
#include "MultipleModelsTestLayer.h"
#include "SimpleGammaCorrectionTestLayer.h"
#include "ShadowMappingTestLayer.h"
#include "UITestLayer.h"
#include "MainSceneTestLayer.h"

#include <iostream>
#include <memory>
#include <AIEntitiesTestLayer.h>

int main(void)
{
	auto app = std::make_unique<Engine::Application>();
	//app->PushLayer(std::make_shared<Test::SimpleGammaCorrectionTestLayer>());
	//app->PushLayer(std::make_shared<Test::ShadowMappingTestLayer>());
	//app->PushLayer(std::make_shared<Test::AIEntitiesTestLayer>());
	//app->PushLayer(std::make_shared<Test::UITestLayer>());
	app->PushLayer(std::make_shared<Test::MainSceneTestLayer>());
	app->Run();

	return 0;
}