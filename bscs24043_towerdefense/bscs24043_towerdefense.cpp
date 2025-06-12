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
#include "myString.h"
#include "Logger.h"
#include <cmath>



int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last of Us Tower Defense");
    SetTargetFPS(60);

    try {
        Game game;
        Logger::getInstance()->log("System", "Game initialized", "INFO");


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
                    Logger::getInstance()->log("NewState", "Returned to main menu from GAME_OVER");
                    game.currentState = MAIN_MENU;
                }
                break;
            }

            if (game.shouldClose()) break;
        }

        Logger::getInstance()->log("System", "Game shutdown", "INFO");
        delete Logger::getInstance();

    }
    catch (const FileNotFound& e) {
        cout << e.getMessage().c_str() << endl;
    }
    
    CloseWindow();
    return 0;
}