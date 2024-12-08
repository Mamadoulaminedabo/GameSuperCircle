#include "menu.h"
#include "spritesheet.h"
#include "sdl2.h"
#include <stdbool.h>
#include <stdio.h>

// Déclaration externe de navSound (à faire dans un header commun si besoin)
extern Mix_Chunk *navSound;

struct menu *menu_initialize(SDL_Renderer *renderer)
{
    struct menu *menu = malloc(sizeof(struct menu));
    menu->renderer = renderer;
    menu->state = MENU_PLAY_FOCUS;
    menu->background = spritesheet_create(BACKGROUND_FILENAME, 1, 1, 1, renderer);
    menu->title = spritesheet_create(TITLE_FILENAME, 1, 1, 1, renderer);
    menu->play = spritesheet_create(PLAY_FILENAME, 1, 1, 1, renderer);
    menu->quit = spritesheet_create(QUIT_FILENAME, 1, 1, 1, renderer);

    menu->difficulty_label = spritesheet_create(DIFFICULTY_FILENAME, 1, 1, 1, renderer);
    menu->easy = spritesheet_create(EASY_FILENAME, 1, 1, 1, renderer);
    menu->medium = spritesheet_create(MEDIUM_FILENAME, 1, 1, 1, renderer);
    menu->hard = spritesheet_create(HARD_FILENAME, 1, 1, 1, renderer);

    if (!menu->difficulty_label || !menu->easy || !menu->medium || !menu->hard)
    {
        fprintf(stderr, "Failed to load difficulty sprites.\n");
        return NULL; 
    }

    menu->selected_difficulty = DIFF_MEDIUM; // Par défaut MEDIUM
    return menu;
}

int menu_alpha(bool chosen)
{
    return chosen ? 255 : 64;
}

void menu_run(struct menu *menu)
{
    SDL_Event e;
    bool running = true;
    while (running && (menu->state != MENU_PLAY && menu->state != MENU_QUIT))
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                menu->state = MENU_QUIT;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    if (menu->state == MENU_DIFFICULTY_FOCUS) {
                        // Remonter sur PLAY_FOCUS
                        menu->state = MENU_PLAY_FOCUS;
                        Mix_PlayChannel(-1, navSound, 0);
                    }
                    break;
                case SDLK_DOWN:
                    // Descendre sur la difficulté si on est sur la ligne PLAY/QUIT
                    if (menu->state == MENU_PLAY_FOCUS || menu->state == MENU_QUIT_FOCUS) {
                        menu->state = MENU_DIFFICULTY_FOCUS;
                        Mix_PlayChannel(-1, navSound, 0);
                    }
                    break;
                case SDLK_LEFT:
                    if (menu->state == MENU_PLAY_FOCUS) {
                        menu->state = MENU_QUIT_FOCUS;
                        Mix_PlayChannel(-1, navSound, 0);
                    } else if (menu->state == MENU_QUIT_FOCUS) {
                        menu->state = MENU_PLAY_FOCUS;
                        Mix_PlayChannel(-1, navSound, 0);
                    } else if (menu->state == MENU_DIFFICULTY_FOCUS) {
                        // Changer la difficulté vers la gauche
                        if (menu->selected_difficulty == DIFF_MEDIUM)
                            menu->selected_difficulty = DIFF_EASY;
                        else if (menu->selected_difficulty == DIFF_HARD)
                            menu->selected_difficulty = DIFF_MEDIUM;
                        Mix_PlayChannel(-1, navSound, 0);
                    }
                    break;
                case SDLK_RIGHT:
                    if (menu->state == MENU_PLAY_FOCUS) {
                        menu->state = MENU_QUIT_FOCUS;
                        Mix_PlayChannel(-1, navSound, 0);
                    } else if (menu->state == MENU_QUIT_FOCUS) {
                        menu->state = MENU_PLAY_FOCUS;
                        Mix_PlayChannel(-1, navSound, 0);
                    } else if (menu->state == MENU_DIFFICULTY_FOCUS) {
                        // Changer la difficulté vers la droite
                        if (menu->selected_difficulty == DIFF_EASY)
                            menu->selected_difficulty = DIFF_MEDIUM;
                        else if (menu->selected_difficulty == DIFF_MEDIUM)
                            menu->selected_difficulty = DIFF_HARD;
                        Mix_PlayChannel(-1, navSound, 0);
                    }
                    break;
                case SDLK_SPACE:
                case SDLK_RETURN:
                    if (menu->state == MENU_PLAY_FOCUS)
                    {
                        menu->state = MENU_PLAY;
                    }
                    else if (menu->state == MENU_QUIT_FOCUS)
                    {
                        menu->state = MENU_QUIT;
                    }
                    break;
                case SDLK_ESCAPE:
                    menu->state = MENU_QUIT;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(menu->renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(menu->renderer);

        // Rendu du background et title
        spritesheet_render(menu->background, BACKGROUND_X, BACKGROUND_Y, 255, 0);
        spritesheet_render(menu->title, TITLE_X, TITLE_Y, 255, 0);

        // Rendu des PLAY/QUIT
        spritesheet_render(menu->play, PLAY_X, PLAY_Y, menu_alpha(menu->state == MENU_PLAY_FOCUS), 0);
        spritesheet_render(menu->quit, QUIT_X, QUIT_Y, menu_alpha(menu->state == MENU_QUIT_FOCUS), 0);

        // Rendu de la difficulté
        spritesheet_render(menu->difficulty_label, DIFFICULTY_X, DIFFICULTY_Y, 255, 0);

        int alpha_easy = (menu->selected_difficulty == DIFF_EASY) ? 255 : 100;
        int alpha_medium = (menu->selected_difficulty == DIFF_MEDIUM) ? 255 : 100;
        int alpha_hard = (menu->selected_difficulty == DIFF_HARD) ? 255 : 100;

        spritesheet_render(menu->easy, EASY_X, EASY_Y, alpha_easy, 0);
        spritesheet_render(menu->medium, MEDIUM_X, MEDIUM_Y, alpha_medium, 0);
        spritesheet_render(menu->hard, HARD_X, HARD_Y, alpha_hard, 0);

        SDL_RenderPresent(menu->renderer);
    }
}

void menu_delete(struct menu *menu)
{
    if (menu != NULL)
    {
        spritesheet_delete(menu->background);
        spritesheet_delete(menu->title);
        spritesheet_delete(menu->play);
        spritesheet_delete(menu->quit);
        spritesheet_delete(menu->difficulty_label);
        spritesheet_delete(menu->easy);
        spritesheet_delete(menu->medium);
        spritesheet_delete(menu->hard);
        free(menu);
    }
}