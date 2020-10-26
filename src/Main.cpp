#define SDL_MAIN_HANDLED
#include "Game.hpp"

int main(int args, int* argv[])
{
    // Création de la partie jeu vidéo
    Game* game = new Game{};
    game->initialize();

    // Boucle principale
    while(game->getIsRunning())
    {
        game->processInput();
        game->update();
        game->render();
    }

    // Libération de la mémoire
    delete game;

    return 0;
}
