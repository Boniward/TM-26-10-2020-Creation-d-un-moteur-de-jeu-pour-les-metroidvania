#include "TextureManager.hpp"
#include <iostream>

TextureManager::TextureManager(std::string name, std::string filePath) : m_name(name)
{
    m_texture = IMG_LoadTexture(Game::getRenderer(), filePath.c_str());

    if(m_texture == NULL)
    {
        std::cerr << "ERREUR : " << IMG_GetError() << std::endl;
    }
}

TextureManager::TextureManager(std::string name, std::string filePath, int fontSize, const SDL_Color color, SDL_Rect* desSize, std::string text) : m_name(name)
{
    TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    m_texture = SDL_CreateTextureFromSurface(Game::getRenderer(), surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(m_texture, NULL, NULL, &desSize->w, &desSize->h);
}

TextureManager::~TextureManager()
{
}

void TextureManager::draw(SDL_Rect* srcSize, SDL_Rect* desSize, bool fixed)
{
    if(fixed)
    {
        if(SDL_RenderCopy(Game::getRenderer(), m_texture, srcSize, desSize) != 0)
        {
            std::cerr << "ERREUR : PROBLEME D'AFFICHAGE" << std::endl;
        }
    }
    else
    {
        SDL_Rect camera{Game::getCamera()};
        SDL_Rect desCamera{desSize->x - camera.x, desSize->y - camera.y, desSize->w, desSize->h};

        if(SDL_RenderCopy(Game::getRenderer(), m_texture, srcSize, &desCamera) != 0)
        {
            std::cerr << "ERREUR : PROBLEME D'AFFICHAGE" << std::endl;
        }
    }

}

std::string TextureManager::getName()
{
    return m_name;
}

void TextureManager::clear()
{
    SDL_DestroyTexture(m_texture);
}
