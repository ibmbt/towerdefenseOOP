#define _CRT_SECURE_NO_WARNINGS
#include "DynamicArray.h"
#include "raylib.h"
#include "Position.h"
#include "Cell.h"
#include "Enemy.h"
#include "Tower.h"
#include "Map.h"
#include "Player.h"
#include "Game.h"
#include <string>
#include <cmath>

// === Constants ===
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int CELL_SIZE = 64;
const int UI_HEIGHT = 150;

// === Global Variable ===
char SELECTED_TOWER = '\0';


// === Enemy Types ===
class Clicker : public Enemy {
public:
    Clicker(const DynamicArray<Position>& p) : Enemy('C', 50, 3, 15, p) {}
};

class Bloater : public Enemy {
public:
    Bloater(const DynamicArray<Position>& p) : Enemy('B', 200, 1, 30, p) {}
};

// === Tower Types ===
class JoelTower : public Tower {
public:
    JoelTower(Position p) : Tower('J', 20, 2, p, 50, 1.0f) {}

    void displayInfo(Vector2 mousePos) const override {
        if (showRange) {
            float panelWidth = 180;
            float panelHeight = 80;
            float panelX = min(mousePos.x + 15, SCREEN_WIDTH - panelWidth - 5);
            float panelY = min(mousePos.y - panelHeight - 10, SCREEN_HEIGHT - panelHeight - 5);

            DrawRectangleRounded(
                Rectangle{ panelX, panelY, panelWidth, panelHeight },
                0.1f, 8, Color{ 40, 40, 60, 230 }
            );
            DrawRectangleRoundedLines(
                Rectangle{ panelX, panelY, panelWidth, panelHeight },
                0.1f, 8, PURPLE
            );

            DrawText(TextFormat("%s Tower", "Joel"),
                panelX + 10, panelY + 10, 20, WHITE);
            DrawText(TextFormat("Damage: %d", damage), panelX + 10, panelY + 35, 18, LIGHTGRAY);
            DrawText(TextFormat("Range: %d", range), panelX + 10, panelY + 55, 18, LIGHTGRAY);
        }
    }
};

class EllieTower : public Tower {
public:
    EllieTower(Position p) : Tower('E', 7, 3, p, 30, 0.5f) {}

    void displayInfo(Vector2 mousePos) const override {
        if (showRange) {
            float panelWidth = 180;
            float panelHeight = 80;
            float panelX = min(mousePos.x + 15, SCREEN_WIDTH - panelWidth - 5);
            float panelY = min(mousePos.y - panelHeight - 10, SCREEN_HEIGHT - panelHeight - 5);

            DrawRectangleRounded(
                Rectangle{ panelX, panelY, panelWidth, panelHeight },
                0.1f, 8, Color{ 40, 40, 60, 230 }
            );
            DrawRectangleRoundedLines(
                Rectangle{ panelX, panelY, panelWidth, panelHeight },
                0.1f, 8, PURPLE
            );

            DrawText(TextFormat("%s Tower", "Ellie"),
                panelX + 10, panelY + 10, 20, WHITE);
            DrawText(TextFormat("Damage: %d", damage), panelX + 10, panelY + 35, 18, LIGHTGRAY);
            DrawText(TextFormat("Range: %d", range), panelX + 10, panelY + 55, 18, LIGHTGRAY);
        }
    }

};


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last of Us Tower Defense");
    SetTargetFPS(60);

    //Game game;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        //game.handleInput();
        //game.update(deltaTime);
        //game.draw();

        //if (game.shouldClose()) break;
    }

    CloseWindow();
    return 0;
}