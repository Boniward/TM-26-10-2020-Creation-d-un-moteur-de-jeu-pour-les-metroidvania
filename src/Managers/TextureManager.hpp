#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "../Game.hpp"
#include <string>

class TextureManager
{
    public:
        TextureManager(std::string name, std::string filePath);
        TextureManager(std::string name, std::string filePath, int fontSize, const SDL_Color color, SDL_Rect* desSize, std::string text);
        ~TextureManager();

        void draw(SDL_Rect* srcSize, SDL_Rect* desSize, bool fixed);

        std::string getName();
        void clear();

    private:
        std::string m_name;
        SDL_Texture* m_texture;
};

#endif
