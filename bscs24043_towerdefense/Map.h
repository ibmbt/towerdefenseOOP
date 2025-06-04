#ifndef MAP_H
#define MAP_H
#include "dynamicArray.h"
#include "Position.h"
#include "Cell.h"
#include "raylib.h"

const int CELL_SIZE = 64;
const int UI_HEIGHT = 150;

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
    Map(const string& filename) {

        grassTex = LoadTexture("resources/grass.png");
        pathTex = LoadTexture("resources/path.png");
        rockTex = LoadTexture("resources/rock.png");

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

    ~Map() {
        UnloadTexture(grassTex);
        UnloadTexture(pathTex);
        UnloadTexture(rockTex);
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