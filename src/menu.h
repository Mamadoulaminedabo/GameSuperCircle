#ifndef MENU_H
#define MENU_H

#include "sdl2.h"
#include "constants.h"

// --------------- //
// Data structures //
// --------------- //

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

// --------- //
// Functions //
// --------- //

/**
 * Creates a new menu.
 *
 * @param renderer  The renderer
 * @return          A pointer to the menu, NULL if there was an error
 */
struct menu *menu_initialize(SDL_Renderer *renderer);

/**
 * Start running the menu.
 *
 * @param menu  The menu to show
 */
void menu_run(struct menu *menu);

/**
 * Delete the menu.
 *
 * @param menu  The menu to delete
 */
void menu_delete(struct menu *menu);


int menu_alpha(bool chosen);


#endif
