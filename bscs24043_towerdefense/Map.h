#ifndef MAP_H
#define MAP_H
#include <fstream>
#include <string>
#include "dynamicArray.h"
#include "Globals.h"
#include "Position.h"
#include "Cell.h"
#include "raylib.h"
using namespace std;


class Map {
private:
    DynamicArray<DynamicArray<Cell>> grid;
    DynamicArray<Position> path;
    int width;
    int height;

    Texture2D pathTex;
    Texture2D grassTex;
    Texture2D rockTex;


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
    Map() {
        grassTex = LoadTexture("resources/sprites/grass.png");
        pathTex = LoadTexture("resources/sprites/path.png");
        rockTex = LoadTexture("resources/sprites/rock.png");
    }

    ~Map() {
        UnloadTexture(grassTex);
        UnloadTexture(pathTex);
        UnloadTexture(rockTex);
    }

    void load(const string& filename) {

        grid.clear();
        path.clear();
        width = height = 0;


        ifstream file(filename);


        /*
        if (!file.is_open()) {

            return;
        }
        */

        string line;
        while (getline(file, line)) {
            DynamicArray<Cell> row;
            int x = 0;
            for (char c : line) {
                row.push(Cell(x, height, c));
                x++;
            }
            grid.push(row);
            height++;
            if (width == 0) width = x;
        }

        file.close();
        findPath();
    }

    void reset() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                grid[y][x].removeTower();
                grid[y][x].removeEnemy();
            }
        }
    }


    const DynamicArray<Position>& getPath() const { return path; }
    Cell& getCell(int x, int y) { return grid[y][x]; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void draw() {

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Texture2D* currentTex = nullptr;
                if (grid[y][x].isPath()) {
                    currentTex = &pathTex;
                }
                else if (grid[y][x].getType() == '#') {
                    currentTex = &rockTex;
                }
                else {
                    currentTex = &grassTex;
                }

                DrawTexture(
                    *currentTex,
                    x * CELL_SIZE,
                    y * CELL_SIZE + UI_HEIGHT,
                    WHITE
                );

                DrawRectangleLines(
                    x * CELL_SIZE,
                    y * CELL_SIZE + UI_HEIGHT,
                    CELL_SIZE, CELL_SIZE,
                    Color{ 0, 0, 0, 50 }
                );
            }
        }


    }
};

#endif