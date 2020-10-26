#ifndef TEXTLABELCOMPONENT_HPP
#define TEXTLABELCOMPONENT_HPP

#include "../Component.hpp"
#include "../Managers/TextureManager.hpp"

#include "TransformComponent.hpp"

class TextLabelComponent : public Component
{
    public:
        TextLabelComponent(Entity* owner, std::string text, std::string filePath, int fontSize, const SDL_Color color, bool fixed);
        ~TextLabelComponent();

        void update() override;
        void render() override;

    private:
        SDL_Rect m_desSize;
        TextureManager* m_texture;
        bool m_fixed;
        static int m_textIndex;
};

#endif
