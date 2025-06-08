#define _CRT_SECURE_NO_WARNINGS
#include "DynamicArray.h"
#include "raylib.h"
#include "Globals.h"
#include "Position.h"
#include "Cell.h"
#include "Enemy.h"
#include "Clicker.h"
#include "Bloater.h"
#include "Map.h"
#include "Game.h"
#include "Tower.h"
#include "JoelTower.h"
#include "EllieTower.h"
#include <string>
#include <cmath>



int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last of Us Tower Defense");
    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();


        switch (game.currentState) {
        case MAIN_MENU:
            game.handleMenuInput();
            game.drawMainMenu();
            break;

        case LEVEL_1:
        case LEVEL_2:
        case LEVEL_3:
            game.handleInput();
            game.update(deltaTime);
            game.draw();
            break;

        case GAME_OVER:
            game.drawGameOverScreen();
            if (IsKeyPressed(KEY_M)) {
                game.currentState = MAIN_MENU;
            }
            break;
        }

        if (game.shouldClose()) break;
    }


    CloseWindow();
    return 0;
}