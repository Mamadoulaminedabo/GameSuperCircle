#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "sdl2.h"
#include <stdbool.h>

typedef struct {
    float angle;
    float speed;
    float distance;
    float width;
    int segment_index;
} Obstacle;

bool init_game(SDL_Window* window_param, SDL_Renderer* renderer_param);
void run_game();
void init_zones();
bool check_collision();
void set_current_difficulty(enum difficulty_level diff);
void spawn_obstacle();
void update_obstacles(float delta_time);
void clean_game();

#endif // GAME_H