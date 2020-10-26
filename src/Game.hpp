#ifndef GAME_HPP
#define GAME_HPP

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Constants.hpp"
#include "Managers/EntityManager.hpp"
#include "Managers/AssetManager.hpp"
#include "Managers/QuestManager.hpp"

// Parser
#include "Kaiser/Kaiser.hpp"

class AssetManager;

class Game
{
    public:
        // Constructeurs / Destructeurs
        Game();
        ~Game();

        // Initialisation / Boucle / Destruction
        void initialize();
        void processInput();
        void update();
        void render();

        // Getters
        bool getIsRunning() const;
        static SDL_Renderer* getRenderer();
        static bool* getKeys();
        static SDL_Rect getCamera();

        // Setters
        static void setIsRunning();

        // Chargement du niveau
        static void loadLevel(std::string fileName);
        static void loadEntity(std::string fileName);
        static void generateEntities();
        static std::string loadQuests(std::string fileName);

    private:
        // Informations pour la boucle
        static bool m_isRunning;
        int m_ticksLastFrame;

        // Composants de la SDL
        SDL_Window* m_window;
        static SDL_Renderer* m_renderer;
        SDL_Event m_event;
        static bool m_keys[];

        // caméra
        static SDL_Rect m_camera;

        // Gestion du script
        static unsigned int m_enemyIndex;
        static unsigned int m_trapIndex;
        static unsigned int m_gateIndex;
        static unsigned int m_UIIndex;

        static unsigned int m_newPosition[2];
        static int m_newHealth;
};

#endif
