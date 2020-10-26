#ifndef ENTITYREWARD_HPP
#define ENTITYREWARD_HPP

#include "../Reward.hpp"

class EntityReward : public Reward
{
    public:
        EntityReward(std::string entity, void (*addEntity)(std::string));
        ~EntityReward();

        void obtain() override;
        void listInfos() override;

    private:
        std::string m_entity;
        void (*m_addEntity)(std::string entity);
};

#endif
