#include "Engine/Core/Application.h"

#include "MainSceneTransitionLayer.h"

#include <iostream>
#include <memory>

int main(void)
{
	auto app = std::make_unique<Engine::Application>();
	app->PushLayer(std::make_shared<Test::MainSceneTransitionLayer>());
	app->Run();

	return 0;
}