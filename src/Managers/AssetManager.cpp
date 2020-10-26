#include "AssetManager.hpp"
#include <iostream>

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::addTexture(std::string id, std::string filePath)
{
    TextureManager* newTexture(new TextureManager(id, filePath));
    m_textures.emplace_back(newTexture);
}

TextureManager* AssetManager::getTexture(std::string id) const
{
    for(auto& texture : m_textures)
    {
        if(texture->getName() == id)
        {
            return texture;
        }
    }

    std::cerr << "ERREUR : ID DE TEXTURE INCORRECT" << std::endl;
}
