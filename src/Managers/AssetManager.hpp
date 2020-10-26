#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include "TextureManager.hpp"
#include <vector>
#include <string>

class TextureManager;
class FontManager;

class AssetManager
{
    public:
        AssetManager();
        ~AssetManager();

        void addTexture(std::string id, std::string filePath);

        TextureManager* getTexture(std::string id) const;

    private:
        std::vector<TextureManager*> m_textures;
};

#endif
