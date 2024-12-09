// graphics.h

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "spritesheet.h"


typedef struct {
    float start_angle;  // Angle de départ de la zone
    float width;        // Largeur de la zone en degrés
} Zone;

// Dessine un cercle rempli
void draw_circle(SDL_Renderer *renderer, int x0, int y0, int radius,
                Uint8 r, Uint8 g, Uint8 b, Uint8 a);

// Dessine un arc avec une épaisseur spécifiée
void draw_arc(SDL_Renderer *renderer, int x, int y, int radius, float start_angle, float end_angle,
            Uint8 r, Uint8 g, Uint8 b, Uint8 a, float thickness);

// Dessine le joueur sous forme de triangle rempli
void draw_player_triangle(SDL_Renderer *renderer, float x, float y, float angle);

// Remplit un triangle défini par trois points
void fill_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3);

// Dessine les segments de couleur de fond alternant gris clair et blanc
void draw_background_segments(SDL_Renderer* renderer, int centerX, int centerY, int radius, Zone* zones, int zone_count);

// Remplit une tranche de pizza (secteur de cercle)
void fill_pizza_slice(SDL_Renderer* renderer, int centerX, int centerY, int radius, float start_angle, float end_angle);

// Remplit un polygone donné par un tableau de points
void filledPolygon(SDL_Renderer* renderer, SDL_Point* vertices, int count);

void render_number(struct spritesheet *digits_spritesheet, const char *number_str, int x, int y, float scale);


#endif // GRAPHICS_H

