#pragma once

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int CELL_SIZE = 64;
const int UI_HEIGHT = 150;

enum GameState {
    MAIN_MENU,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    GAME_OVER
};

// === Global Variables ===
extern char SELECTED_TOWER;
