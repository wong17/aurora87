#include "AIEntitiesTestLayer.h"  

void Test::AIEntitiesTestLayer::OnAttach()  
{  
}  

void Test::AIEntitiesTestLayer::OnDetach()  
{  
}  

void Test::AIEntitiesTestLayer::OnUpdate(float deltaTime)  
{  

   AIEnviroment::Player* player1 = new AIEnviroment::Player();
   AIEnviroment::GameEntityManager::Instance().RegisterEntity(player1);
}  

void Test::AIEntitiesTestLayer::OnImGuiRender()  
{  
}  

void Test::AIEntitiesTestLayer::OnEvent(Engine::Event& event)  
{  
}
