// game.h
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
static void create_obstacle(int zone_index, float *player_speed);
void spawn_arc_obstacles();
void spawn_obstacle();
void update_obstacles(float delta_time);


bool check_collision();
void set_current_difficulty(enum difficulty_level diff);
void clean_game();

#endif // GAME_H


