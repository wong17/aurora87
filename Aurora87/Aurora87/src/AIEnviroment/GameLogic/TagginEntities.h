#include <vector>
#include "AIEnviroment/GameLogic/IEntityTag.h"
namespace AIEnviroment {
    // Tagging class, useful to divide entities, obstacles and objects from game.
    template <typename EntityType>
    class NeighborTagger {
    private:
        std::vector<EntityType*> m_entities;
        double m_radius;

    public:
        NeighborTagger(double radius) : m_radius(radius) {}

        void SetEntities(const std::vector<EntityType*>& entities) {
            m_entities = entities;
        }

        void TagNeighbors(const EntityType* entity) {
            for (auto& e : m_entities) {
                e->UnTag();

                if (e == entity) continue;

                glm::vec3 to = e->GetPosition() - entity->GetPosition();
                double range = m_radius + e->BRadius();

                if (glm::length(to) < range * range) {
                    e->Tag();
                }
            }
        }

        std::vector<EntityType*> GetTagged() const {
            std::vector<EntityType*> tagged;
            for (auto& e : m_entities) {
                if (e->IsTagged()) {
                    tagged.push_back(e);
                }
            }
            return tagged;
        }
    };
}