
// application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include "constants.h"
#include "sdl2.h"
#include "menu.h"
// #include <SDL2/SDL_mixer.h>

// Déclaration de la variable globale navSound
extern Mix_Chunk *navSound;

extern Mix_Music *gameOverMusic;

enum application_state {
    APPLICATION_STATE_MENU,
    APPLICATION_STATE_PLAY,
    APPLICATION_STATE_QUIT
};

struct application {
    enum application_state state;
    struct menu *menu;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Mix_Music *menuMusic;
    Mix_Music *gameMusic;
    Mix_Music *gameOverMusic;

    enum difficulty_level difficulty;
};

/**
 * Creates a new application
 *
 * @return  A pointer to a new application, NULL if there was an error
 */
struct application *application_initialize();

/**
 * Start running the application
 *
 * @param application  The application to run
 */
void application_run(struct application *application);

/**
 * Closes the given application
 *
 * @param application  The application to be closed
 */
void application_shut_down(struct application *application);

#endif // APPLICATION_H