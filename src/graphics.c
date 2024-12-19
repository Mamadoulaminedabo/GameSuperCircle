// graphics.c

#include "graphics.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>



void draw_circle(SDL_Renderer *renderer, int x0, int y0, int radius,
                 Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        // Dessiner des lignes horizontales pour remplir le cercle
        SDL_RenderDrawLine(renderer, x0 - x, y0 + y, x0 + x, y0 + y);
        SDL_RenderDrawLine(renderer, x0 - y, y0 + x, x0 + y, y0 + x);
        SDL_RenderDrawLine(renderer, x0 - x, y0 - y, x0 + x, y0 - y);
        SDL_RenderDrawLine(renderer, x0 - y, y0 - x, x0 + y, y0 - x);

        y += 1;
        err += 1 + 2 * y;
        if (2 * (err - x) + 1 > 0)
        {
            x -= 1;
            err += 1 - 2 * x;
        }
    }
}

void draw_arc(SDL_Renderer *renderer, int x, int y, int radius, float start_angle, float end_angle,
              Uint8 r, Uint8 g, Uint8 b, Uint8 a, float thickness)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    if (end_angle < start_angle)
    {
        float temp = start_angle;
        start_angle = end_angle;
        end_angle = temp;
    }

    float angle_range = end_angle - start_angle;
    int segments = (int)(radius * angle_range / 30); // Ajuster le dénominateur pour contrôler le nombre de segments
    if (segments < 12)
        segments = 12; // Minimum de segments pour la qualité
    float angle_step = angle_range / segments;

    for (float t = 0; t < thickness; t += 1.0f)
    {
        float angle = start_angle;
        for (int i = 0; i < segments; ++i)
        {
            float angle_rad1 = angle * M_PI / 180.0f;
            float angle_rad2 = (angle + angle_step) * M_PI / 180.0f;

            int x1 = x + cosf(angle_rad1) * (radius + t);
            int y1 = y + sinf(angle_rad1) * (radius + t);
            int x2 = x + cosf(angle_rad2) * (radius + t);
            int y2 = y + sinf(angle_rad2) * (radius + t);

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            angle += angle_step;
        }
    }
}

void draw_player_triangle(SDL_Renderer *renderer, float x, float y, float angle)
{
    // Définir la couleur du joueur (bleu clair)
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Bleu Clair

    // Taille du triangle
    float base_width = 20.0f;
    float height = 25.0f;

    // Convertir l'angle en radians
    float angle_rad = angle * M_PI / 180.0f;

    // Calculer l'angle de la tangente
    float tangent_angle_rad = angle_rad + M_PI / 2.0f;

    // Calculer les points de la base du triangle
    float half_base = base_width / 2.0f;
    float base_left_x = x + cosf(tangent_angle_rad) * half_base;
    float base_left_y = y + sinf(tangent_angle_rad) * half_base;

    float base_right_x = x - cosf(tangent_angle_rad) * half_base;
    float base_right_y = y - sinf(tangent_angle_rad) * half_base;

    // Calculer le sommet du triangle
    float apex_x = x + cosf(angle_rad) * height;
    float apex_y = y + sinf(angle_rad) * height;

    // Remplir le triangle
    fill_triangle(renderer, (int)base_left_x, (int)base_left_y,
                  (int)base_right_x, (int)base_right_y,
                  (int)apex_x, (int)apex_y);
}

/**
 * Remplit un triangle défini par trois points (x1, y1), (x2, y2), (x3, y3).
 * Utilise l'algorithme de scanline pour dessiner des lignes horizontales entre les bords du triangle.
 */
void fill_triangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3)
{
    // Trier les points par coordonnée y croissante (y1 <= y2 <= y3)
    if (y2 < y1)
    {
        // Échanger les points 1 et 2
        int temp_x = x1;
        x1 = x2;
        x2 = temp_x;
        int temp_y = y1;
        y1 = y2;
        y2 = temp_y;
    }
    if (y3 < y1)
    {
        // Échanger les points 1 et 3
        int temp_x = x1;
        x1 = x3;
        x3 = temp_x;
        int temp_y = y1;
        y1 = y3;
        y3 = temp_y;
    }
    if (y3 < y2)
    {
        // Échanger les points 2 et 3
        int temp_x = x2;
        x2 = x3;
        x3 = temp_x;
        int temp_y = y2;
        y2 = y3;
        y3 = temp_y;
    }

    // Vérifier si le triangle est dégénéré
    if (y1 == y3)
    {
        return;
    }

    // Calculer les pentes inverses des côtés
    float inv_slope1 = 0;
    float inv_slope2 = 0;

    if (y2 != y1)
        inv_slope1 = (float)(x2 - x1) / (float)(y2 - y1);
    if (y3 != y1)
        inv_slope2 = (float)(x3 - x1) / (float)(y3 - y1);

    float curx1 = x1;
    float curx2 = x1;

    // Parcourir chaque scanline entre y1 et y3
    for (int scanlineY = y1; scanlineY <= y3; scanlineY++)
    {
        if (scanlineY < y2)
        {
            if (y2 != y1)
                curx1 = x1 + inv_slope1 * (scanlineY - y1);
            if (y3 != y1)
                curx2 = x1 + inv_slope2 * (scanlineY - y1);
        }
        else
        {
            float inv_slope3 = 0;
            if (y3 != y2)
                inv_slope3 = (float)(x3 - x2) / (float)(y3 - y2);
            if (y3 != y2)
                curx1 = x2 + inv_slope3 * (scanlineY - y2);
            if (y3 != y1)
                curx2 = x1 + inv_slope2 * (scanlineY - y1);
        }

        // Assurer que curx1 est à gauche de curx2
        if (curx1 > curx2)
        {
            float temp = curx1;
            curx1 = curx2;
            curx2 = temp;
        }

        // Dessiner la ligne horizontale entre curx1 et curx2
        SDL_RenderDrawLine(renderer, (int)curx1, scanlineY, (int)curx2, scanlineY);
    }
}

// Nouvelles fonctions pour les segments de couleur

void draw_background_segments(SDL_Renderer* renderer, int centerX, int centerY, int radius, Zone* zones, int zone_count) {
    for (int i = 0; i < zone_count; ++i) {
        // Déterminer la couleur du segment
        if (i % 2 == 0) {
            SDL_SetRenderDrawColor(renderer, 211, 211, 211, 255); // Gris clair
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
        }

        float start_angle = zones[i].start_angle;
        float end_angle = start_angle + zones[i].width;

        // Dessiner le segment
        fill_pizza_slice(renderer, centerX, centerY, radius, start_angle, end_angle);
    }
}



void fill_pizza_slice(SDL_Renderer* renderer, int centerX, int centerY, int radius, float start_angle, float end_angle)
{
    // Convertir les angles en radians
    float start_rad = start_angle * M_PI / 180.0f;
    float end_rad = end_angle * M_PI / 180.0f;

    // Nombre de points pour dessiner le segment
    int points = 100; // Plus de points = segment plus lisse
    float angle_step = (end_rad - start_rad) / points;

    // Tableau de points pour le polygone
    SDL_Point* vertices = malloc((points + 2) * sizeof(SDL_Point));
    if (vertices == NULL)
    {
        printf("Erreur d'allocation mémoire pour les vertices\n");
        return;
    }

    // Premier point : centre du cercle
    vertices[0].x = centerX;
    vertices[0].y = centerY;

    // Calculer les points du segment
    for (int i = 0; i <= points; ++i)
    {
        float angle = start_rad + i * angle_step;
        vertices[i + 1].x = centerX + (int)(radius * cosf(angle));
        vertices[i + 1].y = centerY + (int)(radius * sinf(angle));
    }

    // Remplir le polygone
    filledPolygon(renderer, vertices, points + 2);

    free(vertices);
}

void filledPolygon(SDL_Renderer* renderer, SDL_Point* vertices, int count)
{
    // Trouver les bornes du polygone
    int minY = vertices[0].y;
    int maxY = vertices[0].y;
    for (int i = 1; i < count; ++i)
    {
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
    }

    // Pour chaque scanline
    for (int y = minY; y <= maxY; y++)
    {
        // Trouver les intersections avec les côtés du polygone
        int intersections[1000]; // Ajuster la taille si nécessaire
        int num_intersections = 0;

        for (int i = 0; i < count; i++)
        {
            int j = (i + 1) % count;
            int y0 = vertices[i].y;
            int y1 = vertices[j].y;

            if ((y0 < y && y1 >= y) || (y1 < y && y0 >= y))
            {
                int x0 = vertices[i].x;
                int x1 = vertices[j].x;

                int x = x0 + (y - y0) * (x1 - x0) / (y1 - y0);
                intersections[num_intersections++] = x;
            }
        }

        // Trier les intersections
        for (int i = 0; i < num_intersections - 1; i++)
        {
            for (int j = i + 1; j < num_intersections; j++)
            {
                if (intersections[i] > intersections[j])
                {
                    int temp = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = temp;
                }
            }
        }

        // Dessiner les lignes entre les paires d'intersections
        for (int i = 0; i < num_intersections; i += 2)
        {
            if (i + 1 < num_intersections)
            {
                SDL_RenderDrawLine(renderer, intersections[i], y, intersections[i + 1], y);
            }
        }
    }
}


void render_number(struct spritesheet *digits_spritesheet, const char *number_str, int x, int y, float scale)
{
    int len = strlen(number_str);
    for (int i = 0; i < len; ++i)
    {
        char c = number_str[i];
        int frame = -1;

        if (c >= '0' && c <= '9')
        {
            frame = c - '0'; // Indice pour les chiffres 0-9
        }
        else if (c == '.')
        {
            frame = 10; // Indice du point dans la spritesheet
        }

        if (frame != -1)
        {
            // Calculer la position de destination pour chaque caractère
            SDL_Rect dst_rect = {
                x + i * (digits_spritesheet->sprite_width * scale),
                y,
                (int)(digits_spritesheet->sprite_width * scale),
                (int)(digits_spritesheet->sprite_height * scale)
            };

            // Calculer la position source dans la spritesheet
            SDL_Rect src_rect = {
                frame * digits_spritesheet->sprite_width,
                0,
                digits_spritesheet->sprite_width,
                digits_spritesheet->sprite_height
            };

            // Rendu de chaque caractère
            SDL_RenderCopy(digits_spritesheet->renderer, digits_spritesheet->texture, &src_rect, &dst_rect);
        }
    }
}
