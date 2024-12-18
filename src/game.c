#include "game.h"
#include "graphics.h"
#include "input.h"
#include "sdl2.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_OBSTACLES 100
#define ARC_THICKNESS 8.0f
#define SEGMENT_COUNT 6

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;

static Obstacle obstacles[MAX_OBSTACLES];
static int obstacle_count = 0;
static Zone zones[SEGMENT_COUNT];
static float player_angle = 0.0f;
float player_speed = 250.0f;
static const float orange_circle_radius = 38.0f;
static const float player_rotational_radius = 45.0f;
static const float player_size = 10.0f;
static const float player_height = 10.0f;

static enum difficulty_level current_difficulty = DIFF_MEDIUM;
extern Mix_Music *gameOverMusic;

void set_current_difficulty(enum difficulty_level diff)
{
    current_difficulty = diff;
}


bool init_game(SDL_Window *window_param, SDL_Renderer *renderer_param)
{
    window = window_param;
    renderer = renderer_param;

    if (TTF_WasInit() == 0)
    {
        if (TTF_Init() == -1)
        {
            printf("Erreur lors de l'initialisation de SDL_ttf: %s\n", TTF_GetError());
            return false;
        }
    }

    font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 24);
    if (!font)
    {
        printf("Erreur lors du chargement de la police: %s\n", TTF_GetError());
        return false;
    }

    init_zones();
    return true;
}



void init_zones()
{
    float angle_per_segment = 360.0f / SEGMENT_COUNT;
    for (int i = 0; i < SEGMENT_COUNT; ++i)
    {
        zones[i].start_angle = i * angle_per_segment;
        zones[i].width = angle_per_segment;
    }
}


// Fonction auxiliaire pour créer un obstacle
static void create_obstacle(int zone_index, float *player_speed)
{
    // Vérifier qu'on ne dépasse pas le tableau d'obstacles
    if (obstacle_count >= MAX_OBSTACLES)
        return;

    Obstacle *obs = &obstacles[obstacle_count++];
    obs->segment_index = zone_index;
    Zone *zone = &zones[zone_index];

    obs->width = zone->width * 1.0f;
    float zone_center_angle = zone->start_angle + zone->width / 2.0f;
    obs->angle = zone_center_angle - obs->width / 2.0f;

    float difficulty_factor;
    switch (current_difficulty)
    {
    case DIFF_EASY:
        difficulty_factor = 0.75f;
        if (player_speed) *player_speed = 200.0f; 
        break;
    case DIFF_MEDIUM:
        difficulty_factor = 1.0f;
        if (player_speed) *player_speed = 250.0f; 
        break;
    case DIFF_HARD:
        difficulty_factor = 2.0f;
        if (player_speed) *player_speed = 300.0f; 
        break;
    }

    obs->speed = 150.0f * difficulty_factor;
    obs->distance = HALF_SCREEN_WIDTH;
}



void spawn_arc_obstacles()
{
    int arc_zones = 2 + (rand() % 3); // Va donner 2, 3 ou 4
    int start_zone = rand() % SEGMENT_COUNT;

    for (int i = 0; i < arc_zones; i++)
    {
        int zone_index = (start_zone + i) % SEGMENT_COUNT;
        // Ici, on ne modifie pas player_speed, donc passer NULL
        create_obstacle(zone_index, NULL);
    }
}

void spawn_obstacle()
{
    // 1 fois sur 3, on génère un arc
    if (rand() % 5 == 0)
    {
        spawn_arc_obstacles();
    }
    else
    {
        // Générer un obstacle classique
        int zone_index = rand() % SEGMENT_COUNT;
        // Ici, on modifie player_speed, donc on passe &player_speed
        create_obstacle(zone_index, &player_speed);
    }
}


void update_obstacles(float delta_time)
{
    float difficulty_factor;
    switch (current_difficulty)
    {
    case DIFF_EASY:
        difficulty_factor = 0.75f;
        break;
    case DIFF_MEDIUM:
        difficulty_factor = 1.0f;
        break;
    case DIFF_HARD:
        difficulty_factor = 1.25f;
        break;
    }

    for (int i = 0; i < obstacle_count; ++i)
    {
        Obstacle *obs = &obstacles[i];
        obs->distance -= obs->speed * delta_time;
        obs->speed += 5.0f * difficulty_factor * delta_time;

        float width_decrease_rate = 1.0f;
        obs->width -= width_decrease_rate * delta_time;
        if (obs->width < 5.0f)
        {
            obs->width = 5.0f;
        }

        Zone *zone = &zones[obs->segment_index];
        float zone_center_angle = zone->start_angle + zone->width / 2.0f;
        obs->angle = zone_center_angle - obs->width / 2.0f;

        if (obs->distance <= orange_circle_radius)
        {
            *obs = obstacles[--obstacle_count];
            --i;
        }
    }
}

bool check_collision()
{
    float apex_x = HALF_SCREEN_WIDTH + cosf(player_angle * M_PI / 180.0f) * (player_rotational_radius + player_height);
    float apex_y = HALF_SCREEN_HEIGHT + sinf(player_angle * M_PI / 180.0f) * (player_rotational_radius + player_height);

    // collision_factor < 1 : plus difficile de déclencher la collision sur les bords
    float collision_factor = 0.1f;

    for (int i = 0; i < obstacle_count; ++i)
    {
        Obstacle *obs = &obstacles[i];

        float arc_start_angle = obs->angle;
        float arc_end_angle = obs->angle + obs->width;

        float apex_angle = atan2f(apex_y - HALF_SCREEN_HEIGHT, apex_x - HALF_SCREEN_WIDTH) * 180.0f / M_PI;
        if (apex_angle < 0)
        {
            apex_angle += 360.0f;
        }

        // Déterminer si l'angle de l'apex est dans l'arc (zone frontale)
        bool angle_within = false;
        if (arc_start_angle <= arc_end_angle)
        {
            if (apex_angle >= arc_start_angle && apex_angle <= arc_end_angle)
            {
                angle_within = true;
            }
        }
        else
        {
            // Si l'arc traverse 0°
            if (apex_angle >= arc_start_angle || apex_angle <= arc_end_angle)
            {
                angle_within = true;
            }
        }

        float closest_angle;
        if (angle_within)
        {
            closest_angle = apex_angle;
        }
        else
        {
            float diff_start = fabsf(apex_angle - arc_start_angle);
            float diff_end = fabsf(apex_angle - arc_end_angle);
            closest_angle = (diff_start < diff_end) ? arc_start_angle : arc_end_angle;
        }

        float closest_rad = closest_angle * M_PI / 180.0f;
        float closest_x = HALF_SCREEN_WIDTH + cosf(closest_rad) * obs->distance;
        float closest_y = HALF_SCREEN_HEIGHT + sinf(closest_rad) * obs->distance;

        float dx = apex_x - closest_x;
        float dy = apex_y - closest_y;
        float distance = sqrtf(dx * dx + dy * dy);

        // Calcul du rayon de collision
        float base_radius = (player_size + ARC_THICKNESS);
        float effective_radius;

        if (angle_within)
        {
            // Si l'arc est directement devant l'apex, collision immédiate
            effective_radius = base_radius;
        }
        else
        {
            // Sur les bords, on demande à l'arc de rentrer plus (moins strict)
            // On réduit donc le rayon effectif
            effective_radius = base_radius * collision_factor;
        }

        if (distance <= effective_radius)
        {
            return true; // Collision
        }
    }
    return false;
}

SDL_Texture *render_text(const char *message, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, message, color);
    if (!surface)
    {
        printf("Erreur lors de la création de la surface de texte: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        printf("Erreur lors de la création de la texture de texte: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;
}

void run_game()
{
    SDL_Event event;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = NOW;
    float spawn_timer = 0.0f;
    bool game_over = false;
    float elapsed_time = 0.0f;
    bool running = true;

    player_angle = 0.0f;
    obstacle_count = 0;
    elapsed_time = 0.0f;
    spawn_timer = 0.0f;

    struct spritesheet *digits_spritesheet = spritesheet_create(TIMER_FILENAME, 1, 11, 11, renderer);
    if (!digits_spritesheet)
    {
        fprintf(stderr, "Failed to load digits spritesheet.\n");
        return;
    }

    float scale = 1.5f;

    // Charger la texture de l'arrière-plan de Game Over
    SDL_Texture *game_over_bg = IMG_LoadTexture(renderer, GAME_OVER_BACKGROUND_FILENAME);
    if (!game_over_bg)
    {
        fprintf(stderr, "Erreur lors du chargement de l'image de Game Over: %s\n", IMG_GetError());
        // Vous pouvez choisir de continuer sans arrière-plan ou de quitter
    }

    // Configuration pour limiter les FPS
    const int TARGET_FPS = 60;
    const double FRAME_DELAY = 1000.0 / TARGET_FPS; // ~16.67 ms

    while (running)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        double delta_time = (double)(NOW - LAST) / (double)SDL_GetPerformanceFrequency();

        // Gestion des événements utilisateur
        while (SDL_PollEvent(&event))
        {
            handle_input(event, &running);
        }

        // Obtenir l'état actuel du clavier
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT]) {
            player_angle -= player_speed * delta_time;
            if (player_angle < 0.0f) {
                player_angle += 360.0f;
            }
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            player_angle += player_speed * delta_time;
            if (player_angle >= 360.0f) {
                player_angle -= 360.0f;
            }
        }

        if (!running)
        {
            break;
        }

        if (!game_over)
        {
            elapsed_time += (float)delta_time;
            spawn_timer += (float)delta_time;
            if (spawn_timer >= 0.5f)
            {
                spawn_obstacle();
                spawn_timer = 0.0f;
            }
            update_obstacles((float)delta_time);

            if (check_collision())
            {
                game_over = true;
                printf("Game Over! Time: %.1f s\n", elapsed_time);
                Mix_HaltMusic();
                Mix_PlayMusic(gameOverMusic, 1);
            }
        }

        // Rendu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (!game_over)
        {
            draw_background_segments(renderer, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT, 800, zones, SEGMENT_COUNT);
            draw_circle(renderer, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT, (int)orange_circle_radius, 100, 100, 100, 255);
            for (int i = 0; i < obstacle_count; ++i)
            {
                Obstacle *obs = &obstacles[i];
                draw_arc(renderer, HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT, (int)obs->distance,
                        obs->angle,
                        obs->angle + obs->width,
                        255, 0, 0, 255, ARC_THICKNESS);
            }
            float px = HALF_SCREEN_WIDTH + cosf(player_angle * M_PI / 180.0f) * player_rotational_radius;
            float py = HALF_SCREEN_HEIGHT + sinf(player_angle * M_PI / 180.0f) * player_rotational_radius;
            draw_player_triangle(renderer, px, py, player_angle);

            // Formatage conditionnel du timer
            char timer_text[20]; // Suffisant pour "MM:SS.S"
            if (elapsed_time < 60.0f)
            {
                // Affichage en secondes avec une décimale
                snprintf(timer_text, sizeof(timer_text), "%.1f", elapsed_time);
            }
            else
            {
                // Calcul des minutes et secondes
                int minutes = (int)(elapsed_time / 60.0f);
                float sec_remainder = elapsed_time - (minutes * 60.0f);
                // Affichage en minutes:secondes avec une décimale
                snprintf(timer_text, sizeof(timer_text), "%d:%.1f", minutes, sec_remainder);
            }

            // Calculer la largeur du texte
            int text_length = (int)strlen(timer_text);
            int digit_width = (int)(digits_spritesheet->sprite_width * scale);
            int total_width = text_length * digit_width;
            int x_position = SCREEN_WIDTH - total_width - 10;
            int y_position = 10;

            // Rendre le numéro
            render_number(digits_spritesheet, timer_text, x_position, y_position, scale);
        }
        else
        {
            // Attendre 2 secondes avant de quitter
            SDL_Delay(2000);

            char game_over_text[100];
            snprintf(game_over_text, sizeof(game_over_text), "Game Over! Time: %.1f s", elapsed_time);
            SDL_Color textColor = {255, 0, 0, 255};

            bool wait_for_input = true;
            while (wait_for_input)
            {
                while (SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        running = false;
                        wait_for_input = false;
                    }
                    else if (event.type == SDL_KEYDOWN)
                    {
                        if (event.key.keysym.sym == SDLK_RETURN ||
                            event.key.keysym.sym == SDLK_ESCAPE ||
                            event.key.keysym.sym == SDLK_SPACE)
                        {
                            wait_for_input = false;
                        }
                    }
                }

                // Rendu de l'arrière-plan de Game Over
                if (game_over_bg)
                {
                    SDL_RenderCopy(renderer, game_over_bg, NULL, NULL); // Remplit tout l'écran
                }
                else
                {
                    // Si l'arrière-plan n'est pas chargé, utilisez une couleur de fond
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                }

                // Rendu du texte Game Over
                SDL_Texture *game_over_texture = render_text(game_over_text, textColor);
                if (game_over_texture)
                {
                    int go_width = 0, go_height = 0;
                    SDL_QueryTexture(game_over_texture, NULL, NULL, &go_width, &go_height);
                    SDL_Rect go_rect = {(SCREEN_WIDTH - go_width) / 2,
                                        (SCREEN_HEIGHT - go_height) / 2 + 130,
                                        go_width, go_height};
                    SDL_RenderCopy(renderer, game_over_texture, NULL, &go_rect);
                    SDL_DestroyTexture(game_over_texture);
                }

                // Rendu du texte d'instructions
                char instructions_text[] = "Press Enter or SPACE to return to Menu";
                SDL_Texture *instructions_texture = render_text(instructions_text, textColor);
                int inst_width = 0, inst_height = 0;
                if (instructions_texture)
                {
                    SDL_QueryTexture(instructions_texture, NULL, NULL, &inst_width, &inst_height);
                    SDL_Rect inst_rect = {(SCREEN_WIDTH - inst_width) / 2,
                                        (SCREEN_HEIGHT - inst_height) / 2 + 180,
                                        inst_width, inst_height};
                    SDL_RenderCopy(renderer, instructions_texture, NULL, &inst_rect);
                    SDL_DestroyTexture(instructions_texture);
                }

                SDL_RenderPresent(renderer);
                SDL_Delay(16); // ~60 FPS
            }

            break; // Quitter la boucle principale après Game Over
        }

        SDL_RenderPresent(renderer);

        // Gestion du délai pour limiter les FPS (optionnel)
        Uint32 frame_time = SDL_GetTicks() - (LAST / SDL_GetPerformanceFrequency() * 1000); // Approximation
        if (frame_time < FRAME_DELAY)
        {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    // Libérer la texture de Game Over
    if (game_over_bg)
    {
        SDL_DestroyTexture(game_over_bg);
    }

    spritesheet_delete(digits_spritesheet);
}

void clean_game()
{
    if (font)
    {
        TTF_CloseFont(font);
        font = NULL;
    }
}
