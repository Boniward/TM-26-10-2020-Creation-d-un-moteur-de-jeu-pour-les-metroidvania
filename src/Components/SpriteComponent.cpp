#include "SpriteComponent.hpp"

SpriteComponent::SpriteComponent(Entity* owner, TextureManager* texture, unsigned int width, unsigned int height, bool fixed, unsigned int speed, std::vector<std::pair<SpriteType, unsigned int> > frames) :
Component(owner),
m_texture(texture),
m_srcSize{0, 0, width, height},
m_fixed(fixed),
m_frames{frames},
m_action(0),
m_activity(true),
m_start(0),
m_speed(speed)
{
}

SpriteComponent::~SpriteComponent()
{
    m_texture->clear();
}

void SpriteComponent::update()
{
    m_desSize.x = m_owner->getComponent<TransformComponent>()->getPosition(0);
    m_desSize.y = m_owner->getComponent<TransformComponent>()->getPosition(1);
    m_desSize.w = m_owner->getComponent<TransformComponent>()->getSize(0);
    m_desSize.h = m_owner->getComponent<TransformComponent>()->getSize(1);

    switch(m_frames[m_action/2].first)
    {
        case STATIC_SPRITE:
            m_srcSize.x = 0;
            m_srcSize.y = m_action * m_srcSize.h;
            break;

        case LOOP_SPRITE:
            m_srcSize.x = (SDL_GetTicks() / m_speed % m_frames[m_action/2].second) * m_srcSize.w;
            m_srcSize.y = m_action * m_srcSize.h;
            break;

        case LINEAR_SPRITE:
            if((SDL_GetTicks() - m_start) / m_speed > (m_frames[m_action/2].second-1))
            {
                m_action = m_action % 2;
                break;
            }
            m_srcSize.x = ((SDL_GetTicks() - m_start) / m_speed) * m_srcSize.w;
            m_srcSize.y = m_action * m_srcSize.h;
            break;

        default:
            break;
    }
}

void SpriteComponent::render()
{
    if(m_activity)
    {
        m_texture->draw(&m_srcSize, &m_desSize, m_fixed);
    }
}

void SpriteComponent::setAction(unsigned int index)
{
    m_action = index;

    if(m_frames[m_action/2].first == LINEAR_SPRITE)
    {
        m_start = SDL_GetTicks();
    }
}

void SpriteComponent::setActivity(bool state)
{
    m_activity = state;
}

std::pair<unsigned int, unsigned int> SpriteComponent::getCurrentFrameIndex() const
{
    return {m_action, (SDL_GetTicks() - m_start) / m_speed % m_frames[m_action/2].second};
}

bool SpriteComponent::getActivity() const
{
    return m_activity;
}

SpriteType SpriteComponent::getCurrentSpriteType() const
{
    return m_frames[m_action/2].first;
}
