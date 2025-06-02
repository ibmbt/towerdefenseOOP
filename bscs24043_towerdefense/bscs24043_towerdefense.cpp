#define _CRT_SECURE_NO_WARNINGS
#include "DynamicArray.h"
#include "raylib.h"
#include "Position.h"
#include "Cell.h"
#include "Enemy.h"
#include "Tower.h"
#include <string>
#include <cmath>

// === Constants ===
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int CELL_SIZE = 64;
const int UI_HEIGHT = 150;

// === Enemy Types ===
class Clicker : public Enemy
{
public:
    Clicker(const DynamicArray<Position> &p) : Enemy('C', 30, 2, 15, p) {}
};

class Bloater : public Enemy
{
public:
    Bloater(const DynamicArray<Position> &p) : Enemy('B', 100, 1, 30, p) {}
};

// === Tower Types ===
class JoelTower : public Tower
{
public:
    JoelTower(Position p) : Tower('J', 25, 2, p, 50, 1.0f) {}
};

class EllieTower : public Tower
{
public:
    EllieTower(Position p) : Tower('E', 10, 3, p, 30, 0.5f) {}
};

// === Map Class ===
class Map {
private:
    DynamicArray<DynamicArray<Cell>> grid;
    DynamicArray<Position> path;
    int width;
    int height;

    void findPath() {
        path.clear();

        Position start = { -1, -1 };
        for (float y = 0; y < height; y++) {
            for (float x = 0; x < width; x++) {
                if (grid[y][x].isPath()) {
                    start = { x, y };
                    break;
                }
            }
            if (start.x != -1) break;
        }

        if (start.x == -1) return;

        DynamicArray<Position> visited;
        path.push(start);
        visited.push(start);

        while (true) {
            Position current = path[path.getSize() - 1];
            bool foundNext = false;

            // Check adjacent cells (up, down, left, right)
            int dx[] = { 0, 0, -1, 1 };
            int dy[] = { -1, 1, 0, 0 };

            for (int d = 0; d < 4; d++) {
                Position next = { current.x + dx[d], current.y + dy[d] };

                if (next.x >= 0 && next.x < width && next.y >= 0 && next.y < height) {
                    if (grid[next.y][next.x].isPath()) {
                        bool alreadyVisited = false;
                        for (int i = 0; i < visited.getSize(); i++) {
                            if (visited[i] == next) {
                                alreadyVisited = true;
                                break;
                            }
                        }

                        if (!alreadyVisited) {
                            path.push(next);
                            visited.push(next);
                            foundNext = true;
                            break;
                        }
                    }
                }
            }

            if (!foundNext) break;
        }
    }

public:
    Map(const std::string& filename) {
        FILE* file = fopen(filename.c_str(), "r");
        if (!file) {
            TraceLog(LOG_ERROR, "Failed to load map file");
            return;
        }

        char line[256];
        while (fgets(line, sizeof(line), file)) {
            DynamicArray<Cell> row;
            int x = 0;
            for (char* c = line; *c != '\0' && *c != '\n'; c++) {
                row.push(Cell(x, height, *c));
                x++;
            }
            grid.push(row);
            height++;
            if (width == 0) width = x;
        }
        fclose(file);

        findPath();
    }

    const DynamicArray<Position>& getPath() const { return path; }
    Cell& getCell(int x, int y) { return grid[y][x]; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void draw() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color color;
                if (grid[y][x].isPath()) {
                    color = Color{ 100, 100, 100, 255 };
                }
                else if (grid[y][x].getType() == '#') {
                    color = Color{ 50, 50, 50, 255 };
                }
                else {
                    color = Color{ 200, 200, 150, 255 };
                }
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE + UI_HEIGHT, CELL_SIZE, CELL_SIZE, color);
                DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE + UI_HEIGHT, CELL_SIZE, CELL_SIZE, BLACK);
            }
        }

        for (int i = 0; i < path.getSize() - 1; i++) {
            Vector2 start = { path[i].x * CELL_SIZE + CELL_SIZE / 2, path[i].y * CELL_SIZE + CELL_SIZE / 2 + UI_HEIGHT };
            Vector2 end = { path[i + 1].x * CELL_SIZE + CELL_SIZE / 2, path[i + 1].y * CELL_SIZE + CELL_SIZE / 2 + UI_HEIGHT };
            DrawLineEx(start, end, 5.0f, RED);
        }
    }
};

// === Player Class ===
class Player {
private:
    DynamicArray<Tower*> towers;
    int lives;
    int coins;

public:
    Player(int initialLives, int initialCoins) : lives(initialLives), coins(initialCoins) {}

    ~Player() {
        for (int i = 0; i < towers.getSize(); i++) {
            delete towers[i];
        }
    }

    bool canAfford(int cost) const { return coins >= cost; }
    void deductCoins(int amount) { coins -= amount; }
    void addCoins(int amount) { coins += amount; }
    void addTower(Tower* tower) { towers.push(tower); }
    void loseLife() { lives--; }

    int getLives() const { return lives; }
    int getCoins() const { return coins; }
    const DynamicArray<Tower*>& getTowers() const { return towers; }
};

class Game {
private:
    Player player;
    Map map;
    DynamicArray<Enemy*> enemies;
    bool gameOver;
    bool gameWon;
    int currentWave;
    int waveSize;
    bool placementPhase;
    float waveTimer;
    float waveDelay;
    int enemiesToSpawn;

public:

};




int main()
{

    return 0;
}