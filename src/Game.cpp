#include "Game.hpp"

// STL
#include <iostream>
#include <exception>
#include <algorithm>
#include <fstream>

// Composants
#include "Components/TransformComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/ColliderComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/KeyboardComponent.hpp"
#include "Components/StatisticsComponent.hpp"
#include "Components/BasicEnemyComponent.hpp"
#include "Components/BasicTrapComponent.hpp"
#include "Components/AdvancedEnemyComponent.hpp"
#include "Components/AttackComponent.hpp"
#include "Components/GateComponent.hpp"
#include "Components/TextLabelComponent.hpp"
#include "Components/InformationComponent.hpp"

// Quêtes
#include "Quests/EliminationQuest.hpp"

// Récompenses
#include "Rewards/EntityReward.hpp"

// Map
#include "Map.hpp"

SDL_Renderer* Game::m_renderer;
bool Game::m_isRunning(false);
bool Game::m_keys[322];
SDL_Rect Game::m_camera{0, 0, 0, 0};
unsigned int Game::m_newPosition[2];
int Game::m_newHealth;
AssetManager assetManager;
EntityManager entityManager;
QuestManager questManager;
TransformComponent* playerPosition;
Map map;

// Constructeurs / Destructeurs

Game::Game() : m_ticksLastFrame(0)
{
}

Game::~Game()
{
    std::cout << "Suppression de l'objet Game en cours...\n" << std::endl;

    entityManager.clear();
    questManager.clear();
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    std::cout << "\nSUCCES !" << std::endl;
}

// Initialisation / Boucle

void Game::initialize()
{
    // Déclaration de la variable qui va permettre de récupérer les dimensions de la fenêtre
    SDL_DisplayMode SCREEN_DIMENSIONS;

    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "ERREUR : INITIALISATION DE LA SDL ECHOUEE" << std::endl;
        return;
    }

    // Récupération des dimensions de la fenêtre
    if(SDL_GetCurrentDisplayMode(0, &SCREEN_DIMENSIONS) != 0)
    {
        std::cerr << "ERREUR : OBTENTION DES DIMENSIONS DE L'ECRAN ECHOUEE" << std::endl;
        return;
    }

    m_camera.w = SCREEN_DIMENSIONS.w;
    m_camera.h = SCREEN_DIMENSIONS.h;

    // Création de la fenêtre
    m_window = SDL_CreateWindow("Among the Death", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, NULL, NULL, SDL_WINDOW_FULLSCREEN);

    if(m_window == NULL)
    {
        std::cerr << "ERREUR : CREATION DE LA FENETRE ECHOUEE" << std::endl;
        return;
    }

    // Initialisation de ttf
    if(TTF_Init() != 0)
    {
        std::cerr << "ERREUR : INITIALISATION DE TTF ECHOUEE" << std::endl;
        return;
    }

    // Création du rendu
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    if(m_renderer == NULL)
    {
        std::cerr << "ERREUR : CREATION DU RENDU ECHOUEE" << std::endl;
        return;
    }

    // Activation de la boucle
    m_isRunning = true;

    // Génération du niveau (premier niveau)
    loadLevel("level1");
    loadQuests("save1");
}

void Game::processInput()
{
    SDL_PollEvent(&m_event);
    switch(m_event.type)
    {
        case SDL_QUIT:
            m_isRunning = false;
            break;

        case SDL_KEYDOWN:
            m_keys[m_event.key.keysym.scancode] = true;
            if(m_event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_isRunning = false;
            }
            break;

        case SDL_KEYUP:
            m_keys[m_event.key.keysym.scancode] = false;
            break;

        default:
            break;
    }
}

void Game::update()
{
    // GESTION DU TEMPS DU RAFRAÎCHISSEMENT DE LA PAGE
    while(SDL_GetTicks() < m_ticksLastFrame + TIME_BETWEEN_FRAMES){}

    // GESTION DES CHANGEMENTS MAJEURS DES DONNÉES
    for(unsigned int i(0); i < entityManager.getEntities().size(); i++)
    {
        Entity* entity{EntityManager::getEntities()[i]};
        switch(entity->getComponent<InformationComponent>()->getType())
        {
            case PLAYER_TYPE:
                if(entity->hasComponent<StatisticsComponent>())
                {
                    if(!entity->getComponent<StatisticsComponent>()->getHealth())
                    {
                        m_isRunning = false;
                    }
                }
                break;

            case ENEMY_TYPE:
                if(entity->hasComponent<StatisticsComponent>())
                {
                    if(!entity->getComponent<StatisticsComponent>()->getHealth())
                    {
                        questManager.addVictim(entity->getComponent<InformationComponent>()->getId());
                        entityManager.deleteEntity(i);
                    }
                }
                break;

            case GATE_TYPE:
                if(entity->getComponent<GateComponent>()->getActivity())
                {
                    m_newPosition[0] = entity->getComponent<GateComponent>()->getPosition(0);
                    m_newPosition[1] = entity->getComponent<GateComponent>()->getPosition(1);
                    m_newHealth = entityManager.getPlayer()->getComponent<StatisticsComponent>()->getHealth() - entityManager.getPlayer()->getComponent<StatisticsComponent>()->getMaxHealth();
                    loadLevel(entity->getComponent<GateComponent>()->getDestination());
                }
                break;

            default:
                break;
        }
    }

    // MISE À JOUR LA MAP ET DES MANAGERS
    map.update();
    entityManager.update();
    questManager.update();

    // CALIBRAGE DE LA CAMERA
    if(playerPosition->getPosition(0) + playerPosition->getSize(0) / 2 < m_camera.w / 2)
    {
        m_camera.x = 0;
    }
    else if(playerPosition->getPosition(0) + playerPosition->getSize(0) / 2 > map.getMapSize(0) * TILE_SIZE * SCALE - m_camera.w / 2)
    {
        m_camera.x = map.getMapSize(0) * TILE_SIZE * SCALE - m_camera.w;
    }
    else
    {
        m_camera.x = playerPosition->getPosition(0) - (m_camera.w - playerPosition->getSize(0)) / 2;
    }

    if(playerPosition->getPosition(1) + playerPosition->getSize(1) / 2 < m_camera.h / 2)
    {
        m_camera.y = 0;
    }
    else if(playerPosition->getPosition(1) + playerPosition->getSize(1) / 2 > map.getMapSize(1) * TILE_SIZE * SCALE -  m_camera.h / 2)
    {
        m_camera.y = map.getMapSize(1) * TILE_SIZE * SCALE - m_camera.h;
    }
    else
    {
        m_camera.y = playerPosition->getPosition(1) - (m_camera.h - playerPosition->getSize(1)) / 2;
    }

    m_ticksLastFrame = SDL_GetTicks();
}

void Game::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    map.render();
    entityManager.render();
    SDL_RenderPresent(m_renderer);
}

// Getters

bool Game::getIsRunning() const
{
    return m_isRunning;
}

SDL_Renderer* Game::getRenderer()
{
    return m_renderer;
}

bool* Game::getKeys()
{
    return m_keys;
}

SDL_Rect Game::getCamera()
{
    return m_camera;
}

// Setters

void Game::setIsRunning()
{
    m_isRunning = !m_isRunning;
}

void Game::loadLevel(std::string fileName)
{
    std::cout << "Nettoyage en cours..." << std::endl;

    entityManager.clear();

    if(Kaiser::initialize("assets/scripts/levels/" + fileName + ".ecs"))
    {
        std::cout << "SUCCES !\n\nGeneration des textures en cours..." << std::endl;

        for(std::string texture : Kaiser::getTextures())
        {
            std::size_t extension(texture.find(".png"));
            std::cout << texture << std::endl;
            std::string name(texture.substr(0, extension));
            assetManager.addTexture(name + "-texture", "assets/images/" + texture);
        }

        std::cout << "SUCCES !\n\nGeneration de la map en cours..." << std::endl;

        assetManager.addTexture("map", "assets/images/" + Kaiser::getMap().second);
        map.initialize("assets/maps/" + Kaiser::getMap().first);

        std::cout << "SUCCES !\n\nGeneration des entites en cours..." << std::endl;

        generateEntities();

        std::cout << "SUCCES !\n" << std::endl;
    }
    else
    {
        std::cerr << "ERREUR : OUVERTURE DU SCRIPT : " << fileName << ".ecs" << std::endl;
    }

    // LISTAGE DES ÉLÉMENTS GÉNÉRÉS
    entityManager.listEntities();
    questManager.listQuests();

    // CALIBRAGE DE LA CAMERA
    playerPosition = entityManager.getEntity("player")->getComponent<TransformComponent>();
    if(m_newPosition[0] != 0 && m_newPosition[1] != 0)
    {
        playerPosition->setPosition(m_newPosition[0] * TILE_SIZE * SCALE, m_newPosition[1] * TILE_SIZE * SCALE);
        entityManager.getPlayer()->getComponent<StatisticsComponent>()->setHealth(m_newHealth);
    }
}

void Game::loadEntity(std::string fileName)
{
    if(Kaiser::initialize("assets/scripts/entities/" + fileName + ".ecs"))
    {
        generateEntities();
    }
    else
    {
        std::cerr << "ERREUR : OUVERTURE DU SCRIPT : " << fileName << ".ecs" << std::endl;
    }
}

void Game::generateEntities()
{
    std::map<std::string, std::map<std::string, std::map<std::string, Value> > > entities(Kaiser::read());

    for(std::map<std::string, std::map<std::string, std::map<std::string, Value> > >::iterator entity(entities.begin()); entity != entities.end(); entity++)
    {
        if(entity->first == "player" && entityManager.getEntity("player") != nullptr)
        {
            entityManager.getEntity("player")->getComponent<TransformComponent>()->setPosition(entities["player"]["transform"]["x"].getInt(), entities["player"]["transform"]["y"].getInt());
            continue;
        }
        entityManager.addEntity(entity->first);

        if(entity->second.find("information") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<InformationComponent>(
                entity->second["information"]["id"].getString(),
                entity->second["information"]["type"].getString()
                );
        }
        if(entity->second.find("transform") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<TransformComponent>(
                entity->second["transform"]["x"].getInt() * TILE_SIZE,
                entity->second["transform"]["y"].getInt() * TILE_SIZE,
                entity->second["transform"]["w"].getUint(),
                entity->second["transform"]["h"].getUint(),
                entity->second["transform"]["vx"].getFloat(),
                entity->second["transform"]["vy"].getFloat(),
                entity->second["transform"]["ax"].getFloat(),
                entity->second["transform"]["ay"].getFloat()
                );
        }
        if(entity->second.find("statistics") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<StatisticsComponent>(
                entity->second["statistics"]["hp"].getInt(),
                entity->second["statistics"]["mp"].getInt(),
                entity->second["statistics"]["dmg"].getInt()
                );
        }
        if(entity->second.find("collider") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<ColliderComponent>(
                entity->second["collider"]["w"].getUint(),
                entity->second["collider"]["h"].getUint()
                );
        }
        if(entity->second.find("sprite") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<SpriteComponent>(
                assetManager.getTexture(entity->second["sprite"]["name"].getString()),
                entity->second["sprite"]["w"].getUint(),
                entity->second["sprite"]["h"].getUint(),
                entity->second["sprite"]["fixed"].getBool(),
                entity->second["sprite"]["speed"].getUint(),
                entity->second["sprite"]["frames"].getSprite()
                );
        }
        if(entity->second.find("movement") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<MovementComponent>(
                entity->second["movement"]["speed"].getUint()
                );
        }
        if(entity->second.find("basicTrap") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<BasicTrapComponent>();
        }
        if(entity->second.find("basicEnemy") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<BasicEnemyComponent>(
                entity->second["basicEnemy"]["start"].getInt(),
                entity->second["basicEnemy"]["distance"].getInt(),
                entity->second["basicEnemy"]["speed"].getInt()
                );
        }
        if(entity->second.find("advancedEnemy") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<AdvancedEnemyComponent>(
                entity->second["advancedEnemy"]["w"].getInt(),
                entity->second["advancedEnemy"]["h"].getInt()
                );
        }
        if(entity->second.find("attack") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<AttackComponent>(
                entity->second["attack"]["x"].getInt(),
                entity->second["attack"]["y"].getInt(),
                entity->second["attack"]["w"].getInt(),
                entity->second["attack"]["h"].getInt(),
                entity->second["attack"]["frame"].getUint(),
                entity->second["attack"]["cooldown"].getUint()
                );
        }
        if(entity->second.find("keyboard") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<KeyboardComponent>(
                static_cast<SDL_Scancode>(entity->second["keyboard"]["up"].getUint()),
                static_cast<SDL_Scancode>(entity->second["keyboard"]["down"].getUint()),
                static_cast<SDL_Scancode>(entity->second["keyboard"]["left"].getUint()),
                static_cast<SDL_Scancode>(entity->second["keyboard"]["right"].getUint()),
                static_cast<SDL_Scancode>(entity->second["keyboard"]["sprint"].getUint()),
                static_cast<SDL_Scancode>(entity->second["keyboard"]["attack"].getUint()),
                static_cast<SDL_Scancode>(entity->second["keyboard"]["action"].getUint())
                );
        }
        if(entity->second.find("textLabel") != entity->second.end())
        {
            const SDL_Color color{
                entity->second["textLabel"]["r"].getInt(),
                entity->second["textLabel"]["g"].getInt(),
                entity->second["textLabel"]["b"].getInt(),
                entity->second["textLabel"]["a"].getInt()};
            entityManager.getEntity(entity->first)->addComponent<TextLabelComponent>(
                entity->second["textLabel"]["text"].getString(),
                entity->second["textLabel"]["fontName"].getString(),
                entity->second["textLabel"]["size"].getUint(),
                color,
                entity->second["textLabel"]["fixed"].getBool()
                );
        }
        if(entity->second.find("gate") != entity->second.end())
        {
            entityManager.getEntity(entity->first)->addComponent<GateComponent>(
                entity->second["gate"]["destination"].getString(),
                entity->second["gate"]["x"].getUint(),
                entity->second["gate"]["y"].getUint()
                );
        }
    }
    entityManager.sortEntities();
}

std::string Game::loadQuests(std::string fileName)
{
    std::ifstream file("saves/" + fileName + ".txt");

    std::string instruction;

    if(file)
    {
        std::string name;
        do
        {
            file >> instruction;

            if(instruction == "Elimination")
            {
                QuestState state;
                unsigned int rawState;
                std::string targetId;
                unsigned int numberToSlay;
                unsigned int numberSlayed;

                file >> name;
                file >> rawState;
                file >> targetId;
                file >> numberToSlay;
                file >> numberSlayed;

                state = static_cast<QuestState>(rawState);

                questManager.addQuest<EliminationQuest>(name, state, targetId, numberToSlay, numberSlayed);
            }

            else if(instruction == "Entity")
            {
                std::string entityFileName;

                file >> entityFileName;

                questManager.getQuest(name)->addReward<EntityReward>(entityFileName, loadEntity);
            }


        }while(instruction != "#");

        questManager.listQuests();

        return name;
    }
    else
    {
        std::cerr << "ERREUR : OUVERTURE DU SCRIPT : " << fileName << ".txt" << std::endl;
    }
}
