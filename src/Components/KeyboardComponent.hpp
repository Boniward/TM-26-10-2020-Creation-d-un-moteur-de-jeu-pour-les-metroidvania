#ifndef KEYBOARDCOMPONENT_HPP
#define KEYBOARDCOMPONENT_HPP

#include "SDL.h"
#include "../Component.hpp"
#include "MovementComponent.hpp"
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "AttackComponent.hpp"
#include "GateComponent.hpp"
#include "../Game.hpp"

class KeyboardComponent : public Component
{
    public:
        KeyboardComponent(Entity* owner, SDL_Scancode upKey, SDL_Scancode downKey, SDL_Scancode leftKey, SDL_Scancode rightKey, SDL_Scancode sprintKey, SDL_Scancode attackKey, SDL_Scancode actionKey);

        void update() override;
        void render() override;

    private:
        SDL_Scancode m_upKey;
        SDL_Scancode m_downKey;
        SDL_Scancode m_rightKey;
        SDL_Scancode m_leftKey;
        SDL_Scancode m_sprintKey;
        SDL_Scancode m_attackKey;
        SDL_Scancode m_actionKey;
        bool* m_keys;
        bool m_lastDirection;
        unsigned int m_lastAttack;

};

#endif
