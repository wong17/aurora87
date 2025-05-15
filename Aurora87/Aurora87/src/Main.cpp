#include "Engine/Core/Application.h"

#include "MultipleEntitiesTestLayer.h"
#include "MultipleModelsTestLayer.h"
#include "SimpleGammaCorrectionTestLayer.h"

#include <iostream>
#include <memory>

int main(void)
{
	auto app = std::make_unique<Engine::Application>();
	//app->PushLayer(std::make_shared<Test::MultipleEntitiesTestLayer>());
	//app->PushLayer(std::make_shared<Test::MultipleModelsTestLayer>());
	app->PushLayer(std::make_shared<Test::SimpleGammaCorrectionTestLayer>());
	app->Run();

	return 0;
}