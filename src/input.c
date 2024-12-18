#include "input.h"

void handle_input(SDL_Event event, bool* running) {
    if (event.type == SDL_QUIT) {
        *running = false;
    }

    // Quitter le jeu avec Échap
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        *running = false;
    }
}