#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <vector>

#include "Components/TileComponent.hpp"

class Map
{
    public:
        Map();
        ~Map();

        void initialize(std::string filePath);
        void update();
        void render();
        unsigned int getMapSize(bool axis);
        TileComponent* getTile(unsigned int x, unsigned int y);

    private:
        std::vector< std::vector<TileComponent*> > m_tiles;
        unsigned int setTileTexture(std::vector<bool> tiles);
        std::vector<bool> getNeighbours(int x, int y, std::vector<std::vector<bool> > tab);
};

#endif
