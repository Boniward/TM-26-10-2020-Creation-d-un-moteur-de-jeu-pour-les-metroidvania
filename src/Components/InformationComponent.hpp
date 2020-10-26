#ifndef INFORMATION_COMPONENT_HPP
#define INFORMATION_COMPONENT_HPP

#include "../Component.hpp"

class InformationComponent : public Component
{
    public:
        InformationComponent(Entity* owner, std::string id, std::string type);
        ~InformationComponent();

        void update() override;
        void render() override;

        std::string getId();
        EntityType getType();

    private:
        std::string m_id;
        EntityType m_type;
};

#endif
