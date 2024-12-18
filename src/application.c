#include "application.h"
#include "sdl2.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

// Définition de la variable globale navSound
Mix_Chunk *navSound = NULL;
Mix_Music *gameOverMusic = NULL; // Variable globale

struct application *application_initialize()
{
    struct application *application;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "SDL failed to initialize: %s\n", SDL_GetError());
        return NULL;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        fprintf(stderr, "Warning: Linear texture filtering not enabled!");
    }
    application = malloc(sizeof(struct application));
    if (!application)
    {
        fprintf(stderr, "Failed to allocate memory for application.\n");
        return NULL;
    }
    application->window = SDL_CreateWindow("Circle",
                                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (application->window == NULL)
    {
        fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
        free(application);
        return NULL;
    }
    application->renderer = SDL_CreateRenderer(application->window, -1,
                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (application->renderer == NULL)
    {
        fprintf(stderr, "Renderer could not be created: %s\n", SDL_GetError());
        SDL_DestroyWindow(application->window);
        free(application);
        return NULL;
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(application->renderer);
        SDL_DestroyWindow(application->window);
        free(application);
        return NULL;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(application->renderer);
        SDL_DestroyWindow(application->window);
        free(application);
        return NULL;
    }

    // Charger le menu
    application->menu = menu_initialize(application->renderer);
    if (application->menu == NULL)
    {
        fprintf(stderr, "Failed to initialize menu: %s\n", SDL_GetError());
        TTF_Quit();
        Mix_CloseAudio();
        SDL_DestroyRenderer(application->renderer);
        SDL_DestroyWindow(application->window);
        free(application);
        return NULL;
    }

    // Charger les musiques et sons
    Mix_Music *menuMusic = Mix_LoadMUS(MENU_MUSIC);
    Mix_Music *gameMusic = Mix_LoadMUS(GAME_MUSIC);
    gameOverMusic = Mix_LoadMUS(GAME_OVER_MUSIC);
    if (!gameOverMusic)
    {
        fprintf(stderr, "Failed to load game over music: %s\n", Mix_GetError());
    }
    navSound = Mix_LoadWAV(NAV_SOUND); // Assignation à la variable globale

    if (!menuMusic || !gameMusic || !navSound)
    {
        fprintf(stderr, "Failed to load audio: %s\n", Mix_GetError());
        // Vous pouvez choisir de quitter ou de continuer
    }

    application->menuMusic = menuMusic;
    application->gameMusic = gameMusic;
    application->gameOverMusic = gameOverMusic;

    Mix_PlayMusic(application->menuMusic, -1); // Musique du menu en boucle
    application->state = APPLICATION_STATE_MENU;
    application->difficulty = DIFF_MEDIUM; // Valeur par défaut
    return application;
}

void application_run(struct application *application)
{
    while (application->state != APPLICATION_STATE_QUIT)
    {
        switch (application->state)
        {
        case APPLICATION_STATE_MENU:
            application->menu->state = MENU_PLAY_FOCUS;
            menu_run(application->menu);

            if (application->menu->state == MENU_QUIT)
            {
                application->state = APPLICATION_STATE_QUIT;
            }
            else if (application->menu->state == MENU_PLAY)
            {
                // Récupérer la difficulté choisie dans le menu
                application->difficulty = application->menu->selected_difficulty;

                // Changer la musique pour le jeu
                Mix_HaltMusic();
                Mix_PlayMusic(application->gameMusic, -1);

                // Définir la difficulté dans le jeu
                set_current_difficulty(application->difficulty);

                application->state = APPLICATION_STATE_PLAY;
            }
            break;

        case APPLICATION_STATE_PLAY:
            if (init_game(application->window, application->renderer))
            {
                run_game();

                // Après le game over, revenir au menu
                clean_game();

                // Rejouer la musique du menu
                Mix_HaltMusic();
                Mix_PlayMusic(application->menuMusic, -1);
            }
            else
            {
                fprintf(stderr, "Failed to initialize the game.\n");
            }
            application->state = APPLICATION_STATE_MENU;
            break;

        case APPLICATION_STATE_QUIT:
            break;
        }
    }
}

void application_shut_down(struct application *application)
{
    Mix_FreeMusic(application->menuMusic);
    Mix_FreeMusic(application->gameMusic);
    Mix_FreeMusic(application->gameOverMusic);
    Mix_FreeChunk(navSound);
    Mix_CloseAudio();

    SDL_DestroyRenderer(application->renderer);
    SDL_DestroyWindow(application->window);
    menu_delete(application->menu);
    free(application);
    TTF_Quit();
    SDL_Quit();
}