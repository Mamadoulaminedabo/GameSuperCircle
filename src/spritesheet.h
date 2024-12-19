// spritesheet.h
#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "sdl2.h"


#include <stdio.h>
#include <stdlib.h>


struct spritesheet {
    int num_rows;           // The number of rows in the sprite sheet
    int num_columns;        // The number of columns in the sprite sheet
    int num_sprites;        // The total number of sprites
    int sprite_width;       // The width of an individual sprite
    int sprite_height;      // The height of an individual sprite
    float scale;            // The scale of the rendered sprite
    SDL_Texture *texture;   // The texture (image) of the sprite sheet
    SDL_Renderer *renderer; // The renderer
};

/**
 * Creates a sprite sheet.
 *
 * @param filename     The filename of the sprite sheet image
 * @param num_rows     The number of rows in the sprite sheet
 * @param num_columns  The number of columns in the sprite sheet
 * @param num_sprites  The total number of sprites
 * @param renderer     The renderer used for loading the sprite sheet
 * @return             The sprite sheet
 */
struct spritesheet *spritesheet_create(const char *filename,
                                       int num_rows,
                                       int num_columns,
                                       int num_sprites,
                                       SDL_Renderer* renderer);

/**
 * Delete the sprite sheet.
 *
 * @param spritesheet  The spritesheet to delete
 */
void spritesheet_delete(struct spritesheet *spritesheet);

/**
 * Renders the current sprite.
 *
 * @param spritesheet  The spritesheet to render
 * @param x            The top-left corner x-coordinate for the render
 * @param y            The top-left corner y-coordinate for the render
 * @param alpha        The transparence when rendering
 * @param sprite       The sprite to render
 */
void spritesheet_render(struct spritesheet *spritesheet,
                        int x, int y, int alpha, int frame);

#endif
