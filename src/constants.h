#define SCREEN_WIDTH  (0.5 * 2048)
#define HALF_SCREEN_WIDTH  (0.5 * 2048)/2
#define SCREEN_HEIGHT (0.5 * 1535)
#define HALF_SCREEN_HEIGHT (0.5 * 1535)/2


#ifndef CONSTANTS_H
#define CONSTANTS_H

enum difficulty_level {
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD
};

// Menu constants
#define BACKGROUND_FILENAME "../assets/background-small.png"
#define BACKGROUND_X 0
#define BACKGROUND_Y 0
#define TITLE_FILENAME "../assets/title-small.png"
#define TITLE_WIDTH 588
#define TITLE_X ((SCREEN_WIDTH - TITLE_WIDTH)/2)
#define TITLE_Y (0.05 * SCREEN_HEIGHT)
#define PLAY_FILENAME "../assets/play.png"
#define PLAY_WIDTH 122
#define PLAY_X (0.25 * SCREEN_WIDTH - 0.5 * PLAY_WIDTH)
#define PLAY_Y (0.45 * SCREEN_HEIGHT)
#define QUIT_FILENAME "../assets/Quit.png"
#define QUIT_WIDTH 160
#define QUIT_X (0.75 * SCREEN_WIDTH - 0.5 * QUIT_WIDTH)
#define QUIT_Y (0.45 * SCREEN_HEIGHT)
#define DIFFICULTY_FILENAME "../assets/difficulty-small.png"
#define TIMER_FILENAME "../assets/digits-small.png"
#define DIFFICULTY_WIDTH 325
#define DIFFICULTY_X (0.5 * SCREEN_WIDTH - 0.5 * DIFFICULTY_WIDTH)
#define DIFFICULTY_Y (0.7 * SCREEN_HEIGHT)
#define EASY_FILENAME "../assets/easy-small.png"
#define EASY_WIDTH 122
#define EASY_X (0.25 * SCREEN_WIDTH - 0.5 * EASY_WIDTH)
#define EASY_Y (0.9 * SCREEN_HEIGHT)
#define MEDIUM_FILENAME "../assets/medium-small.png"
#define MEDIUM_WIDTH 160
#define MEDIUM_X (0.5 * SCREEN_WIDTH - 0.5 * MEDIUM_WIDTH)
#define MEDIUM_Y (0.9 * SCREEN_HEIGHT)
#define HARD_FILENAME "../assets/hard-small.png"
#define HARD_WIDTH 122
#define HARD_X (0.75 * SCREEN_WIDTH - 0.5 * HARD_WIDTH)
#define HARD_Y (0.9 * SCREEN_HEIGHT)
#define NAV_SOUND "../assets/nav.wav"
#define MENU_MUSIC "../assets/menu.wav"
#define GAME_MUSIC "../assets/MissionImpossible.mp3"
#define GAME_OVER_MUSIC "../assets/Game-Over.mp3"
#define GAME_OVER_BACKGROUND_FILENAME "../assets/gameover.jpg"





#endif // CONSTANTS_H