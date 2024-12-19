#ifndef MENU_H
#define MENU_H

#include "sdl2.h"
#include "constants.h"

enum menu_state {
    MENU_PLAY_FOCUS,
    MENU_QUIT_FOCUS,
    MENU_DIFFICULTY_FOCUS, // Nouveau pour la difficulté
    MENU_PLAY,
    MENU_QUIT
};

struct menu {
    enum menu_state state;
    struct spritesheet *background;
    struct spritesheet *title;
    struct spritesheet *play;
    struct spritesheet *quit;

    // Ajouts difficulté
    struct spritesheet *difficulty_label; 
    struct spritesheet *easy;
    struct spritesheet *medium;
    struct spritesheet *hard;

    SDL_Renderer *renderer;
    enum difficulty_level selected_difficulty; 
};

struct menu *menu_initialize(SDL_Renderer *renderer);
void menu_run(struct menu *menu);
void menu_delete(struct menu *menu);

#endif