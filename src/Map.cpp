#include "Map.hpp"
#include <iostream>
#include <fstream>

Map::Map() : m_tiles{{NULL}}
{
}

Map::~Map()
{
    /*
    std::cout << "Suppression de la map" << std::endl;

    for(unsigned int x(0); x < m_tiles.size(); x++)
    {
        for(unsigned int y(0); y < m_tiles[x].size(); y++)
        {
            delete m_tiles[x][y];
        }
    }

    std::cout << "SUCCES !" << std::endl;*/
}

void Map::initialize(std::string filePath)
{
    m_tiles = {{NULL}};


    std::ifstream file(filePath);

    if(file)
    {
        // IDENTIFICATION DES COLLISIONS
        std::string tilesCollisionsLine("1");
        std::vector< std::vector<bool> > tilesCollisions{{true}};
        unsigned int line(0);

        while(tilesCollisionsLine != "%")
        {

            for(char tileInfo : tilesCollisionsLine)
            {
                tilesCollisions[line].emplace_back(static_cast<bool>(static_cast<unsigned int>(tileInfo-48)));
            }

            file >> tilesCollisionsLine;

            tilesCollisions[line].emplace_back(true);

            tilesCollisions.emplace_back(std::vector<bool>{true});
            line++;
        }

        // AJOUT DES BORDURES HORIZONTALES
        tilesCollisions[0].assign(tilesCollisions[4].size(), true);
        tilesCollisions[tilesCollisions.size()-1].assign(tilesCollisions[4].size(), true);

        // AFFICHAGE DE LA BORDURE
        std::cout << "Apparence de la map : \n" << std::endl;

        for(std::vector<bool> range : tilesCollisions)
        {
            for(bool element : range)
            {
                char caseVisual(177);

                if(element)
                {
                    caseVisual = 178;
                }

                std::cout << caseVisual << caseVisual;
            }

            std::cout << std::endl;
        }

        std::cout << std::endl;

        // CRÉATION DES OBJETS "TILECOMPONENTS"

        m_tiles[0][0] = new TileComponent(0, 0, tilesCollisions[1][1], setTileTexture(getNeighbours(1, 1, tilesCollisions)));
        for(int i(1); i < (tilesCollisions[1].size()-2); i++)
        {
            m_tiles[0].emplace_back(new TileComponent(0, i, tilesCollisions[1][i+1], setTileTexture(getNeighbours(1, i+1, tilesCollisions))));
        }

        for(int i(1); i < (line-1); i++)
        {
            m_tiles.emplace_back(NULL);

            for(int j(0); j < (tilesCollisions[i].size()-2); j++)
            {
                m_tiles[i].emplace_back(new TileComponent(i, j, tilesCollisions[i+1][j+1], setTileTexture(getNeighbours(i+1, j+1, tilesCollisions))));
            }
        }
    }

    else
    {
        std::cerr << "ERREUR : OUVERTURE DU FICHIER MAP" << std::endl;
    }
}

void Map::update()
{
    for(unsigned int x(0); x < m_tiles.size(); x++)
    {
        for(unsigned int y(0); y < m_tiles[x].size(); y++)
        {
            m_tiles[x][y]->update();
        }
    }
}

void Map::render()
{
    for(unsigned int x(0); x < m_tiles.size(); x++)
    {
        for(unsigned int y(0); y < m_tiles[x].size(); y++)
        {
            m_tiles[x][y]->render();
        }
    }
}

unsigned int Map::getMapSize(bool axis)
{
    if(axis)
    {
        return m_tiles.size();
    }

    return m_tiles[0].size();
}

TileComponent* Map::getTile(unsigned int x, unsigned int y)
{
    return m_tiles[y][x];
}

std::vector<bool> Map::getNeighbours(int x, int y, std::vector<std::vector<bool> > tab)
{
    return {tab[x-1][y-1], tab[x][y-1], tab[x+1][y-1], tab[x-1][y], tab[x][y], tab[x+1][y], tab[x-1][y+1], tab[x][y+1], tab[x+1][y+1]};
}

unsigned int Map::setTileTexture(std::vector<bool> tiles)
{
    if(tiles[4])
    {
        int faces = tiles[1] + tiles[3] + tiles[5] + tiles[7];
        int corners = tiles[0] + tiles[2] + tiles[6] + tiles[8];
        switch(faces)
        {
            // #################################
            case 0:
                return 15;
            // #################################
            case 1:
                if(tiles[1])
                    return 14;
                if(tiles[3])
                    return 11;
                if(tiles[5])
                    return 3;
                if(tiles[7])
                    return 12;
            // #################################
            case 2:
                if(tiles[3] && tiles[5])
                    return 7;
                if(tiles[1] && tiles[7])
                    return 13;
                if(tiles[1] && tiles[3])
                {
                    if(tiles[0])
                        return 10;
                    return 26;
                }
                if(tiles[1] && tiles[5])
                {
                    if(tiles[2])
                        return 2;
                    return 22;
                }
                if(tiles[3] && tiles[7])
                {
                    if(tiles[6])
                        return 8;
                    return 25;
                }
                if(tiles[5] && tiles[7])
                {
                    if(tiles[8])
                        return 0;
                    return 21;
                }
            // #################################
            case 3:
                if(!tiles[1])
                {
                    if(tiles[6] && tiles[8])
                        return 4;
                    if(tiles[6])
                        return 20;
                    if(tiles[8])
                        return 28;
                    return 24;
                }
                if(!tiles[3])
                {
                    if(tiles[2] && tiles[8])
                        return 1;
                    if(tiles[2])
                        return 16;
                    if(tiles[8])
                        return 18;
                    return 17;
                }
                if(!tiles[5])
                {
                    if(tiles[0] && tiles[6])
                        return 9;
                    if(tiles[0])
                        return 29;
                    if(tiles[6])
                        return 31;
                    return 30;
                }
                if(!tiles[7])
                {
                    if(tiles[0] && tiles[2])
                        return 6;
                    if(tiles[0])
                        return 19;
                    if(tiles[2])
                        return 27;
                    return 23;
                }
            // #################################
            case 4:
                switch(corners)
                {
                    case 0:
                        return 46;

                    case 1:
                        if(tiles[0])
                            return 40;
                        if(tiles[2])
                            return 44;
                        if(tiles[6])
                            return 41;
                        if(tiles[8])
                            return 45;

                    case 2:
                        if(tiles[0] && tiles[2])
                            return 38;
                        if(tiles[0] && tiles[6])
                            return 34;
                        if(tiles[2] && tiles[8])
                            return 39;
                        if(tiles[6] && tiles[8])
                            return 35;
                        if(tiles[0] && tiles[8])
                            return 43;
                        if(tiles[2] && tiles[6])
                            return 42;

                    case 3:
                        if(!tiles[0])
                            return 37;
                        if(!tiles[2])
                            return 33;
                        if(!tiles[6])
                            return 36;
                        if(!tiles[8])
                            return 32;

                    case 4:
                        return 5;
                }
            // #################################
        }
    }

    return 47;
}
