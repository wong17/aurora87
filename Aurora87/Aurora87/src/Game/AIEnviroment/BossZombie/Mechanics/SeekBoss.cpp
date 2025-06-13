#include "SeekBoss.h"

namespace AIEnviroment 
{
    SeekBoss* SeekBoss::Instance()
    {
        if (!m_instance)
        {
            m_instance = new SeekBoss();
        }
        return m_instance;
    }

    void SeekBoss::Enter(BossZombie* entity)
    {
        entity->ClosestPlayerTarget(); // Find the closest player target

    }

    void SeekBoss::Execute(BossZombie* entity)
    {
        entity->SetSpeed(8.0f);
        entity->ClosestPlayerTarget();
        Player* entityPlayer = dynamic_cast<Player*>(GameEntityManager::Instance().GetEntity(entity->GetPlayerTargetID()));

        glm::vec3 toEvader = entityPlayer->GetPosition() - entity->GetPosition();

        double relativeHeading = glm::dot(entity->Heading(), entityPlayer->Heading());
        if ((glm::dot(toEvader, entity->Heading()) > 0) && (relativeHeading < -0.95)) {
            entity->SetTargetPosition(entityPlayer->GetPosition());
        }
        else {
            double LookAheadTime = glm::length(toEvader) / (entity->GetSpeed() + entityPlayer->GetSpeed());
            glm::vec3 futurePosition = entityPlayer->GetPosition() + entityPlayer->GetVelocity() * static_cast<float>(LookAheadTime);
            entity->SetTargetPosition(futurePosition);

        }

        glm::vec3 targetPosition = entity->GetTargetPosition();
        glm::vec3 currentPosition = entity->GetPosition();

        glm::vec3 direction = glm::normalize(targetPosition - currentPosition);
        entity->SetDirection(direction);
        glm::vec3 desiredVelocity = direction * entity->GetSpeed();
        entity->SetTargetVelocity(desiredVelocity);
        //std::cout << "Zombie " << entity->GetID()<< " ID Position: " << entity->GetPosition().x << ", " << entity->GetPosition().y << ", " << entity->GetPosition().z << std::endl;

        float distanceToPlayer = glm::length(targetPosition - currentPosition);
        if (distanceToPlayer > entity->GetAttackRange()) {
            glm::vec3 newPosition = currentPosition + desiredVelocity * entity->GetDeltaTime();
            entity->SetPosition(newPosition);
        }
        if (distanceToPlayer <= entity->GetAttackRange())
        {
            entity->SetAttackCooldownTimer(entity->GetAttackCooldownTimer() + entity->GetDeltaTime());
            EventData attackEvent(EventType::ZOMBIE_ATTACK);
            attackEvent.Set("zombieDumbDamage", entity->GetAttackDamage());
            if (entity->GetAttackCooldownTimer() >= entity->GetAttackCooldown())
            {
                attackEvent.Set("zombieID", entity->GetID());
                attackEvent.Set("playerID", entity->GetPlayerTargetID());
                EventManager::GetInstance()->DispatchEvent(attackEvent);
                entity->SetAttackCooldownTimer(0.0f); // Reset cooldown timer
            }

        }
        if (entity->GetDistanceCooldownTimer() >= entity->GetDistanceCooldown()) {
            if (distanceToPlayer <= entity->GetDistanceAttack()) {
                entity->SetDistanceCooldownTimer(0.0f); // Reset cooldown timer
                entity->ChangeFiniteStateBossZombie(AttackDistanceBoss::Instance());
            }
        }
        if (entity->GetChargeAttackCooldownTimer() >= entity->GetChargeAttackCooldown())
        {
            if (distanceToPlayer <= entity->GetDistanceToPlayerCharge())
            {
                entity->SetChargeAttackCooldownTimer(0.0f); // Reset cooldown timer
                entity->ChangeFiniteStateBossZombie(ChargeBoss::Instance());
            }
        }
        entity->SetDistanceCooldownTimer(entity->GetDistanceCooldownTimer() + entity->GetDeltaTime());
        entity->SetChargeAttackCooldownTimer(entity->GetChargeAttackCooldownTimer() + entity->GetDeltaTime());
    }

    void SeekBoss::Exit(BossZombie* entity)
    {
    }
    SeekBoss* SeekBoss::m_instance = nullptr; // Initialize the static instance pointer
}