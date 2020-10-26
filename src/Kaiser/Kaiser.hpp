#ifndef KAISER_HPP
#define KAISER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <iterator>
#include "../Constants.hpp"

enum Token
{
    IMPORT,
    ASSET,
    TEXTURE,
    SOUND,
    FONT,
    ENTITY,
    MODEL,
    SCHEMA,
    COMPONENT,
    MAP,
    VALUE,
    INTEGER,
    FLOAT,
    TRUE,
    FALSE,
    STRING,
    SPRITE,
    INTEGER_TYPE,
    UNSIGNED_INTEGER_TYPE,
    FLOAT_TYPE,
    UNSIGNED_FLOAT_TYPE,
    STRING_TYPE,
    BOOL_TYPE,
    SPRITE_TYPE,
    IDENTIFIER,
    DOUBLE_QUOTES,
    OPENING_PARENTHESIS,
    CLOSING_PARENTHESIS,
    OPENING_HOOK,
    CLOSING_HOOK,
    OPENING_BRACE,
    CLOSING_BRACE,
    INFERIOR,
    SUPERIOR,
    EQUAL,
    TWO_POINTS,
    COMMA,
    BAD_READ
};

struct Value
{
    Token valueType;
    std::string value;

    std::string getString()
    {
        if(valueType == STRING_TYPE)
        {
            return value;
        }
        std::cerr << "ERROR : wrong return type (string)" << std::endl;
        return "";
    }
    int getInt()
    {
        if(valueType == INTEGER_TYPE)
        {
            return std::stoi(value);
        }
        std::cerr << "ERROR : wrong return type (int)" << std::endl;
        return 0;
    }
    unsigned int getUint()
    {
        if(valueType == UNSIGNED_INTEGER_TYPE)
        {
            return std::stoul(value);
        }
        std::cerr << "ERROR : wrong return type (uint)" << std::endl;
        return 0;
    }
    float getFloat()
    {
        if(valueType == FLOAT_TYPE)
        {
            return std::stof(value);
        }
        std::cerr << "ERROR : wrong return type (float)" << std::endl;
        return 0;
    }
    bool getBool()
    {
        if(valueType == BOOL_TYPE)
        {
            if(value == "true")
            {
                return true;
            }
            return false;
        }
        std::cerr << "ERROR : wrong return type (bool)" << std::endl;
        return false;
    }
    std::vector<std::pair<SpriteType, unsigned int> > getSprite()
    {
        if(valueType == SPRITE_TYPE)
        {
            std::vector<std::pair<SpriteType, unsigned int> > frames;
            SpriteType spriteType(STATIC_SPRITE);
            std::string framesNumber("");
            unsigned int index(0);

            while(index < value.length())
            {
                switch(value[index])
                {
                    case '#':
                        spriteType = STATIC_SPRITE;
                        index++;
                        continue;
                    case '@':
                        spriteType = LOOP_SPRITE;
                        index++;
                        continue;
                    case '&':
                        spriteType = LINEAR_SPRITE;
                        index++;
                        continue;
                    default:
                        {
                            framesNumber = "";

                            while(value[index] != '#' && value[index] != '&' && value[index] != '@')
                            {
                                framesNumber += value[index];
                                index++;
                            }
                        }
                        frames.emplace_back(spriteType, std::stoul(framesNumber));
                        break;
                }
            }
            return frames;
        }
        std::cerr << "ERROR : wrong return type (sprite)" << std::endl;
        return {{STATIC_SPRITE, 1}};
    }

};

class Kaiser
{
    public:
        static bool initialize(std::string filePath);
        static std::map<std::string, std::map<std::string, std::map<std::string, Value> > > read();
        static std::pair<std::string, std::string> getMap();
        static std::vector<std::string> getTextures();

    private:
        static std::list<std::pair<Token, std::string> > lexer(std::string filePath);
        static bool parser(std::list<std::pair<Token, std::string> > statements);

        static bool isLetter(char symbol);
        static bool isNumber(char symbol);

        static std::vector<std::string> m_textures;
        static std::vector<std::string> m_sounds;
        static std::vector<std::string> m_fonts;
        static std::pair<std::string, std::string> m_map;
        static std::map<std::string, std::map<std::string, Value> > m_schema;
        static std::map<std::string, std::map<std::string, std::map<std::string, Value> > > m_models;
        static std::map<std::string, std::map<std::string, std::map<std::string, Value> > > m_entities;
};

#endif
