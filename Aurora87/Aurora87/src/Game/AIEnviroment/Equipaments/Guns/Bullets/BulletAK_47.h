#pragma once
#include "AIEnviroment/Base/BaseGameEntity.h"
#include "AIEnviroment/Equipaments/Guns/Bullets/BulletManager.h"
namespace AIEnviroment {
	class BulletAK_47 : public BaseGameEntity, public BulletManager
	{
	public:
		BulletAK_47();
		BulletAK_47(float damage, glm::vec3 direction, float range, float speed);
		~BulletAK_47(); 
	};

}