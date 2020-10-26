#ifndef GATECOMPONENT_HPP
#define GATECOMPONENT_HPP

#include "../Component.hpp"
#include "ColliderComponent.hpp"

class GateComponent : public Component
{
    public:
        GateComponent(Entity* owner, std::string destination, unsigned int x, unsigned int y);
        ~GateComponent();

        void update() override;
        void render() override;

        void setActivity();

        bool getActivity() const;
        std::string getDestination() const;
        unsigned int getPosition(bool index) const;

        void enter();

    private:
        std::string m_destination;
        bool m_actived;
        unsigned int m_newPosition[2];
        static int m_cooldown;
        static int m_lastChange;
};

#endif
