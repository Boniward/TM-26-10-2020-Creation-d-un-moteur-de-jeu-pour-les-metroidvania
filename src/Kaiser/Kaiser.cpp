#include "Kaiser.hpp"

std::vector<std::string> Kaiser::m_textures;
std::vector<std::string> Kaiser::m_sounds;
std::vector<std::string> Kaiser::m_fonts;
std::pair<std::string, std::string> Kaiser::m_map;
std::map<std::string, std::map<std::string, Value> > Kaiser::m_schema;
std::map<std::string, std::map<std::string, std::map<std::string, Value> > > Kaiser::m_models;
std::map<std::string, std::map<std::string, std::map<std::string, Value> > > Kaiser::m_entities;

char lowerCases[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
char upperCases[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
std::map<std::string, Token> keywords{
    {"import", IMPORT},
    {"asset", ASSET},
    {"texture", TEXTURE},
    {"sound", SOUND},
    {"font", FONT},
    {"entity", ENTITY},
    {"model", MODEL},
    {"schema", SCHEMA},
    {"component", COMPONENT},
    {"map", MAP},
    {"value", VALUE},
    {"true", TRUE},
    {"false", FALSE},
    {"int", INTEGER_TYPE},
    {"uint", UNSIGNED_INTEGER_TYPE},
    {"float", FLOAT_TYPE},
    {"ufloat", UNSIGNED_FLOAT_TYPE},
    {"str", STRING_TYPE},
    {"bool", BOOL_TYPE},
    {"spr", SPRITE_TYPE}
};

bool Kaiser::initialize(std::string filePath)
{
    std::ifstream file(filePath);

    if(file)
    {
        m_schema.clear();
        m_models.clear();
        m_entities.clear();
        m_textures.clear();
        m_sounds.clear();
        m_fonts.clear();

        if(parser(lexer(filePath)))
        {
            return true;
        }
        std::cerr << "ERROR : corrupted file" << std::endl;
        return false;

    }

    std::cerr << "ERROR : file not found" << std::endl;
    return false;
}

std::map<std::string, std::map<std::string, std::map<std::string, Value> > > Kaiser::read()
{
    return m_entities;
}

std::pair<std::string, std::string> Kaiser::getMap()
{
    return m_map;
}

std::vector<std::string> Kaiser::getTextures()
{
    return m_textures;
}

std::list<std::pair<Token, std::string> > Kaiser::lexer(std::string filePath)
{
    std::list<std::pair<Token, std::string> > statements;

    std::ifstream file(filePath);
    char character;

    do
    {
        file.get(character);

        // commentaire
        if(character == '/')
        {
            file.get(character);
            if(character == '/')
            {
                while(character != 10)
                {
                    file.get(character);
                }
            }
            else if(character == '*')
            {
                while(true)
                {
                    file.get(character);
                    if(character == '*')
                    {
                        file.get(character);
                        if(character == '/')
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                std::cerr << "ERROR : comment not completed" << std::endl;
                return {{BAD_READ, "BAD READ"}};
            }
        }

        // mot clé ou nom de valeur/model/entitee
        if(isLetter(character))
        {
            std::string identifier("");
            do
            {
                identifier += character;
                file.get(character);
            }while(isLetter(character) || isNumber(character) || character == '_');

            if(keywords.count(identifier))
            {
                statements.emplace_back(keywords[identifier], identifier);
            }
            else
            {
                statements.emplace_back(IDENTIFIER, identifier);
            }
        }

        // nombre
        if(isNumber(character) || character == '-')
        {
            std::string number("");
            bool decimal(false);
            do
            {
                number += character;
                file.get(character);
                if(character == '.')
                {
                    if(!decimal)
                    {
                        decimal = true;
                    }
                    else
                    {
                        std::cerr << "ERROR : double points in numeric value" << std::endl;
                        return {{BAD_READ, "BAD READ"}};
                    }
                }

            }while(isNumber(character) || character == '.');

            if(decimal)
            {
                statements.emplace_back(FLOAT, number);
            }
            else
            {
                statements.emplace_back(INTEGER, number);
            }

        }

        // chaîne de caractères
        if(character == '"')
        {
            std::string text = "";
            while(true)
            {
                file.get(character);
                if(character == '"')
                {
                    break;
                }
                text += character;
            }
            statements.emplace_back(STRING, text);
        }

        // sprite
        if(character == '#' || character == '&' || character == '@')
        {
            std::string text = "";
            do
            {
                text += character;
                file.get(character);
                if(isNumber(character))
                {
                    do
                    {
                        text += character;
                        file.get(character);
                    }while(isNumber(character));
                }
                else
                {
                    break;
                }

            }while(character == '#' || character == '&' || character == '@');
            statements.emplace_back(SPRITE, text);
        }

        // signes spécifiques
        else
        {
            switch(character)
            {
                case '(':
                    statements.emplace_back(OPENING_PARENTHESIS, "(");
                    break;
                case ')':
                    statements.emplace_back(CLOSING_PARENTHESIS, ")");
                    break;
                case '[':
                    statements.emplace_back(OPENING_HOOK, "[");
                    break;
                case ']':
                    statements.emplace_back(CLOSING_HOOK, "]");
                    break;
                case '{':
                    statements.emplace_back(OPENING_BRACE, "{");
                    break;
                case '}':
                    statements.emplace_back(CLOSING_BRACE, "}");
                    break;
                case '<':
                    statements.emplace_back(INFERIOR, "<");
                    break;
                case '>':
                    statements.emplace_back(SUPERIOR, ">");
                    break;
                case '=':
                    statements.emplace_back(EQUAL, "=");
                    break;
                case ':':
                    statements.emplace_back(TWO_POINTS, ":");
                    break;
                case ',':
                    statements.emplace_back(COMMA, ",");
                    break;
                default:
                    break;
            }
        }

    }while(!file.eof());

    return statements;
}

bool Kaiser::parser(std::list<std::pair<Token, std::string> > statements)
{
    while(!statements.empty())
    {
        switch(statements.front().first)
        {
            // importation d'un ou plusieurs fichier(s) de type ecs
            case IMPORT:
                statements.pop_front();
                if(statements.front().first == OPENING_HOOK)
                {
                    while(true)
                    {
                        statements.pop_front();
                        if(statements.front().first == STRING)
                        {
                            parser(lexer(statements.front().second));
                            statements.pop_front();
                            if(statements.front().first == CLOSING_HOOK)
                            {
                                break;
                            }
                            else if(statements.front().first == COMMA)
                            {
                                continue;
                            }
                            else
                            {
                                std::cerr << "ERROR : symbol ] or , missing in import instruction" << std::endl;
                                return false;
                            }
                        }
                        else
                        {
                            std::cerr << "ERROR : file path missing in import instruction" << std::endl;
                            return false;
                        }
                    }
                }
                else
                {
                    std::cerr << "ERROR : symbol [ missing after import" << std::endl;
                    return false;
                }
                break;

            // ajout d'assets
            case ASSET:
                statements.pop_front();
                if(statements.front().first == INFERIOR)
                {
                    statements.pop_front();
                    Token type(statements.front().first);
                    statements.pop_front();
                    if(statements.front().first == SUPERIOR)
                    {
                        statements.pop_front();
                        if(statements.front().first == STRING)
                        {
                            switch(type)
                            {
                                case TEXTURE:
                                    m_textures.emplace_back(statements.front().second);
                                    break;
                                case SOUND:
                                    m_sounds.emplace_back(statements.front().second);
                                    break;
                                case FONT:
                                    m_fonts.emplace_back(statements.front().second);
                                    break;
                                default:
                                    std::cerr << "ERROR : unknown asset type" << std::endl;
                                    break;
                            }
                        }
                        else
                        {
                            std::cerr << "ERROR : file path not written" << std::endl;
                            break;
                        }
                    }
                    else
                    {
                        std::cerr << "ERROR : symbol > missing" << std::endl;
                        return false;
                    }
                }
                else
                {
                    std::cerr << "ERROR : symbol < missing" << std::endl;
                    return false;
                }
                break;

            // ajout de la map
            case MAP:
                statements.pop_front();

                if(statements.front().first != OPENING_HOOK)
                {
                    std::cerr << "ERROR : symbol [ missing after map" << std::endl;
                }

                statements.pop_front();

                if(statements.front().first != STRING)
                {
                    std::cerr << "ERROR : map name missing after map" << std::endl;
                }

                m_map.first = statements.front().second;
                statements.pop_front();

                if(statements.front().first != TWO_POINTS)
                {
                    std::cerr << "ERROR : symbol : missing after map" << std::endl;
                }

                statements.pop_front();

                if(statements.front().first != STRING)
                {
                    std::cerr << "ERROR : asset name missing after map" << std::endl;
                }

                m_map.second = statements.front().second;
                statements.pop_front();

                if(statements.front().first != CLOSING_HOOK)
                {
                    std::cerr << "ERROR : symbol [ missing after map" << std::endl;
                }
                break;

            // création du schéma
            case SCHEMA:
            {
                statements.pop_front();

                if(statements.front().first == OPENING_BRACE)
                {
                    statements.pop_front();
                    while(true)
                    {
                        if(statements.front().first == CLOSING_BRACE)
                        {
                            break;
                        }
                        else if(statements.front().first == COMPONENT)
                        {
                            statements.pop_front();

                            if(statements.front().first != INFERIOR)
                            {
                                std::cerr << "ERROR : symbol < missing" << std::endl;
                                return false;
                            }

                            statements.pop_front();

                            if(statements.front().first != IDENTIFIER)
                            {
                                std::cerr << "ERROR : identifier for component missing" << std::endl;
                                return false;
                            }

                                std::string component(statements.front().second);
                                statements.pop_front();

                            if(statements.front().first != SUPERIOR)
                            {
                                std::cerr << "ERROR : symbol > missing" << std::endl;
                                return false;
                            }

                            std::map<std::string, Value> values;
                            m_schema.emplace(component, values);
                            statements.pop_front();

                            if(statements.front().first == OPENING_BRACE)
                            {
                                while(true)
                                {
                                    statements.pop_front();
                                    if(statements.front().first == CLOSING_BRACE)
                                    {
                                        statements.pop_front();
                                        break;
                                    }

                                    if(statements.front().first == VALUE)
                                    {
                                        statements.pop_front();
                                        if(statements.front().first != INFERIOR)
                                        {
                                            std::cerr << "ERROR : symbol < missing" << std::endl;
                                            return false;
                                        }

                                        statements.pop_front();
                                        Token type(statements.front().first);
                                        statements.pop_front();

                                        if(statements.front().first != SUPERIOR)
                                        {
                                            std::cerr << "ERROR : symbol > missing" << std::endl;
                                            return false;
                                        }

                                        statements.pop_front();

                                        if(statements.front().first != IDENTIFIER)
                                        {
                                            std::cerr << "ERROR : identifier for value missing" << statements.front().second<< std::endl;
                                            return false;
                                        }

                                        std::string valueName(statements.front().second);
                                        statements.pop_front();

                                        if(statements.front().first != EQUAL)
                                        {
                                            std::cerr << "ERROR : symbol = missing" << std::endl;
                                            return false;
                                        }

                                        statements.pop_front();

                                        switch(statements.front().first)
                                        {
                                            case STRING:
                                                if(type == STRING_TYPE)
                                                {
                                                    Value value = {type, statements.front().second};
                                                    m_schema[component].emplace(valueName, value);
                                                }
                                                break;

                                            case INTEGER:
                                                if(type == INTEGER_TYPE || type == UNSIGNED_INTEGER_TYPE)
                                                {
                                                    Value value = {type, statements.front().second};
                                                    m_schema[component].emplace(valueName, value);
                                                }
                                                break;
                                            case TRUE:
                                            case FALSE:
                                                if(type == BOOL_TYPE)
                                                {
                                                    Value value = {type, statements.front().second};
                                                    m_schema[component].emplace(valueName, value);
                                                }
                                                break;
                                            case FLOAT:
                                                if(type == FLOAT_TYPE || type == UNSIGNED_FLOAT_TYPE)
                                                {
                                                    Value value = {type, statements.front().second};
                                                    m_schema[component].emplace(valueName, value);
                                                }
                                                break;
                                            case SPRITE:
                                                if(type == SPRITE_TYPE)
                                                {
                                                    Value value = {type, statements.front().second};
                                                    m_schema[component].emplace(valueName, value);
                                                }
                                                break;

                                            default:
                                                std::cerr << "ERROR : type error" << std::endl;
                                                return false;
                                        }
                                    }
                                    else
                                    {
                                        std::cerr << "ERROR : excepted value" << std::endl;
                                        return false;
                                    }
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                    }
                }
                break;
            }

            // création d'un model
            case MODEL:
            {
                // Récupération du nom du modèle
                statements.pop_front();

                if(statements.front().first != OPENING_HOOK)
                {
                    std::cerr << "ERROR : symbol [ missing" << std::endl;
                    return false;
                }

                statements.pop_front();

                if(statements.front().first != IDENTIFIER)
                {
                    std::cerr << "ERROR : symbol identifier for model" << std::endl;
                    return false;
                }

                std::string model(statements.front().second);
                std::map<std::string, std::map<std::string, Value> > components;
                m_models.emplace(model, components);
                statements.pop_front();

                if(statements.front().first != CLOSING_HOOK)
                {
                    std::cerr << "ERROR : symbol [ missing" << std::endl;
                    return false;
                }

                statements.pop_front();

                // Attribution des composants et nouvelles valeurs
                if(statements.front().first == OPENING_BRACE)
                {
                    statements.pop_front();
                    while(true)
                    {
                        if(statements.front().first == CLOSING_BRACE)
                        {
                            break;
                        }
                        else if(statements.front().first == COMPONENT)
                        {
                            statements.pop_front();

                            if(statements.front().first != INFERIOR)
                            {
                                std::cerr << "ERROR : symbol < missing" << std::endl;
                                return false;
                            }

                            statements.pop_front();

                            if(statements.front().first != IDENTIFIER)
                            {
                                std::cerr << "ERROR : identifier for component missing" << std::endl;
                                return false;
                            }

                                std::string component(statements.front().second);
                                statements.pop_front();

                            if(statements.front().first != SUPERIOR)
                            {
                                std::cerr << "ERROR : symbol > missing" << std::endl;
                                return false;
                            }

                            std::map<std::string, Value> values;
                            m_models[model].emplace(component, m_schema[component]);
                            statements.pop_front();

                            if(statements.front().first == OPENING_BRACE)
                            {
                                while(true)
                                {
                                    statements.pop_front();
                                    if(statements.front().first == CLOSING_BRACE)
                                    {
                                        statements.pop_front();
                                        break;
                                    }

                                    if(statements.front().first != IDENTIFIER)
                                    {
                                        std::cerr << "ERROR : identifier for value missing" << std::endl;
                                        return false;
                                    }

                                    std::string valueName(statements.front().second);
                                    Token type(m_schema[component][valueName].valueType);
                                    statements.pop_front();

                                    if(statements.front().first != EQUAL)
                                    {
                                        std::cerr << "ERROR : symbol = missing" << std::endl;
                                        return false;
                                    }

                                    statements.pop_front();

                                    switch(statements.front().first)
                                    {
                                        case STRING:
                                            if(type == STRING_TYPE)
                                            {
                                                m_models[model][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                        case INTEGER:
                                            if(type == INTEGER_TYPE || type == UNSIGNED_INTEGER_TYPE)
                                            {
                                                m_models[model][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                        case FLOAT:
                                            if(type == FLOAT_TYPE || type == UNSIGNED_FLOAT_TYPE)
                                            {
                                                m_models[model][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                        case TRUE:
                                        case FALSE:
                                            if(type == BOOL_TYPE)
                                            {
                                                m_models[model][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                        case SPRITE:
                                            if(type == SPRITE_TYPE)
                                            {
                                                m_models[model][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                            default:
                                                std::cerr << "ERROR : type error" << std::endl;
                                                return false;
                                    }
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                    }
                }
                break;
            }

            // création d'une entitée
            case ENTITY:
            {
                // Récupération du nom du modèle
                statements.pop_front();

                if(statements.front().first != OPENING_HOOK)
                {
                    std::cerr << "ERROR : symbol [ missing" << std::endl;
                    return false;
                }

                statements.pop_front();

                if(statements.front().first != IDENTIFIER)
                {
                    std::cerr << "ERROR : symbol identifier for entity" << std::endl;
                    return false;
                }

                std::string entity(statements.front().second);
                 statements.pop_front();

                if(statements.front().first != TWO_POINTS)
                {
                    std::cerr << "ERROR : symbol : missing" << std::endl;
                    return false;
                }

                statements.pop_front();

                if(statements.front().first != IDENTIFIER)
                {
                    std::cerr << "ERROR : symbol identifier for model" << std::endl;
                    return false;
                }

                std::string model(statements.front().second);
                m_entities.emplace(entity, m_models[model]);
                statements.pop_front();

                if(statements.front().first != CLOSING_HOOK)
                {
                    std::cerr << "ERROR : symbol [ missing" << std::endl;
                    return false;
                }

                statements.pop_front();

                // Attribution des nouvelles valeurs
                if(statements.front().first == OPENING_BRACE)
                {
                    statements.pop_front();
                    while(true)
                    {
                        if(statements.front().first == CLOSING_BRACE)
                        {
                            break;
                        }
                        else if(statements.front().first == COMPONENT)
                        {
                            statements.pop_front();

                            if(statements.front().first != INFERIOR)
                            {
                                std::cerr << "ERROR : symbol < missing" << std::endl;
                                return false;
                            }

                            statements.pop_front();

                            if(statements.front().first != IDENTIFIER)
                            {
                                std::cerr << "ERROR : identifier for component missing" << std::endl;
                                return false;
                            }

                                std::string component(statements.front().second);
                                statements.pop_front();

                            if(statements.front().first != SUPERIOR)
                            {
                                std::cerr << "ERROR : symbol > missing" << std::endl;
                                return false;
                            }

                            statements.pop_front();

                            if(statements.front().first == OPENING_BRACE)
                            {
                                while(true)
                                {
                                    statements.pop_front();
                                    if(statements.front().first == CLOSING_BRACE)
                                    {
                                        statements.pop_front();
                                        break;
                                    }

                                    if(statements.front().first != IDENTIFIER)
                                    {
                                        std::cerr << "ERROR : identifier for value missing" << std::endl;
                                        return false;
                                    }

                                    std::string valueName(statements.front().second);
                                    Token type(m_schema[component][valueName].valueType);
                                    statements.pop_front();

                                    if(statements.front().first != EQUAL)
                                    {
                                        std::cerr << "ERROR : symbol = missing" << std::endl;
                                        return false;
                                    }

                                    statements.pop_front();

                                    switch(statements.front().first)
                                    {
                                        case STRING:
                                            if(type == STRING_TYPE)
                                            {
                                                m_entities[entity][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                        case INTEGER:
                                            if(type == INTEGER_TYPE || type == UNSIGNED_INTEGER_TYPE)
                                            {
                                                m_entities[entity][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                        case FLOAT:
                                            if(type == FLOAT_TYPE || type == UNSIGNED_FLOAT_TYPE)
                                            {
                                                m_entities[entity][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                        case TRUE:
                                        case FALSE:
                                            if(type == BOOL_TYPE)
                                            {
                                                m_entities[entity][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                        case SPRITE:
                                            if(type == SPRITE_TYPE)
                                            {
                                                m_entities[entity][component][valueName].value = statements.front().second;
                                            }
                                            break;

                                            default:
                                                std::cerr << "ERROR : type error" << std::endl;
                                                return false;
                                    }
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                    }
                }
                break;
            }

            // erreur de syntaxe (il manque une étape dans le tas)
            default:
                std::cerr << "ERROR : keyword missing" << std::endl;
                return false;
        }
        statements.pop_front();
    }
    return true;
}

bool Kaiser::isLetter(char symbol)
{
    for(char letter : lowerCases)
    {
        if(letter == symbol)
        {
            return true;
        }
    }
    for(char letter : upperCases)
    {
        if(letter == symbol)
        {
            return true;
        }
    }
    return false;
}

bool Kaiser::isNumber(char symbol)
{
    for(char number : numbers)
    {
        if(number == symbol)
        {
            return true;
        }
    }
    return false;
}
