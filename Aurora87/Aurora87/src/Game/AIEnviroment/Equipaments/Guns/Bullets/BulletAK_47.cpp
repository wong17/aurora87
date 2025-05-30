#include "BulletAK_47.h"
namespace AIEnviroment {

    BulletAK_47::BulletAK_47()
    {
    }

    BulletAK_47::BulletAK_47(float damage, glm::vec3 direction, float range, float speed) 
    {
        this->SetDagame(damage);
        this->SetDirection(direction);
        this->SetRange(range);
        this->SetSpeed(speed);
    }

    BulletAK_47::~BulletAK_47()
    {
    }

    void BulletAK_47::Update()
    {
    }

    void BulletAK_47::ChangeFiniteStateAK7()
    {
    }

}
