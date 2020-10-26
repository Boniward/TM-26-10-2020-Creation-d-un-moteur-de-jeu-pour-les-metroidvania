#include "TextLabelComponent.hpp"

int TextLabelComponent::m_textIndex(0);

TextLabelComponent::TextLabelComponent(Entity* owner, std::string text, std::string filePath, int fontSize, const SDL_Color color, bool fixed) :
Component(owner),
 m_fixed(fixed)
{
    m_desSize = {m_owner->getComponent<TransformComponent>()->getPosition(0), m_owner->getComponent<TransformComponent>()->getPosition(1), 0, 0};
    std::string name("text" + std::to_string(m_textIndex));
    m_textIndex++;
    m_texture = new TextureManager(name, filePath, fontSize, color, &m_desSize, text);
}

TextLabelComponent::~TextLabelComponent()
{

}

void TextLabelComponent::update()
{
        m_desSize.x = m_owner->getComponent<TransformComponent>()->getPosition(0);
        m_desSize.y = m_owner->getComponent<TransformComponent>()->getPosition(1);
}

void TextLabelComponent::render()
{
    m_texture->draw(NULL, &m_desSize, m_fixed);
}
