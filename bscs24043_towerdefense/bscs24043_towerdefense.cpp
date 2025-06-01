#include <iostream>
#include <fstream>
#include "raylib.h"
#include "dynamicArray.h"
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <conio.h>
using namespace std;


const int REFRESH_RATE_MS = 500;


struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};


class Enemy {
protected:
    char symbol;
    int health;
    Position pos;
    int pathIndex;
    DynamicArray<Position> path;

public:
    Enemy(char sym, int hp, const DynamicArray<Position>& p)
        : symbol(sym), health(hp), path(p), pathIndex(0) {
        pos = path[0];
    }

    virtual ~Enemy() {}


    virtual void move() {
        if (pathIndex + 1 >= path.getSize()) return;

        Position next = path[pathIndex + 1];

        if (pos.x < next.x) pos.x++;
        else if (pos.x > next.x) pos.x--;
        else if (pos.y < next.y) pos.y++;
        else if (pos.y > next.y) pos.y--;

        if (pos == next) {
            pathIndex++;
        }
    }



    int getHealth() const {
        return health;
    }

    virtual char getSymbol() const { return symbol; }
    virtual Position getPosition() const { return pos; }
    virtual bool isAlive() const { return health > 0; }
    virtual void takeDamage(int dmg) {
        health = health - dmg;
        if (health < 0) health = 0;
    }

    bool reachedEnd() const {
        return pos == path[path.getSize() - 1];
    }

};

// === CLICKER ===
class Clicker : public Enemy {
public:
    Clicker(const DynamicArray<Position>& p) : Enemy('C', 30, p) {}
};

// === BLOATER ===
class Bloater : public Enemy {
public:
    Bloater(const DynamicArray<Position>& p) : Enemy('B', 100, p) {}
};

class Tower {
protected:
    char symbol;
    int damage;
    int range;
    Position pos;

public:
    Tower(char sym, int dmg, int rng, Position p) : symbol(sym), damage(dmg), range(rng), pos(p) {}

    virtual ~Tower() {}

    virtual void attack(DynamicArray<Enemy*>& enemies) {
        for (int i = 0; i < enemies.getSize(); ++i) {
            Enemy* e = enemies[i];
            if (e->isAlive()) {
                Position ep = e->getPosition();
                int dx = abs(ep.x - pos.x);
                int dy = abs(ep.y - pos.y);
                if (dx + dy <= range) {
                    cout << symbol << " hits " << e->getSymbol()
                        << " at (" << ep.x << "," << ep.y << ") for "
                        << damage << " damage! "
                        << "Remaining HP: " << e->getHealth() << std::endl;
                    e->takeDamage(damage);
                    break; 
                }
            }
        }
    }


    virtual Position getPosition() const { return pos; }
    virtual char getSymbol() const { return symbol; }
};


// === JOEL TOWER ===
class JoelTower : public Tower {
public:
    JoelTower(Position p) : Tower('J', 25, 2, p) {}
};

// === ELLIE TOWER ===
class EllieTower : public Tower {
public:
    EllieTower(Position p) : Tower('E', 10, 3, p) {}
};

class Game {
private:
    DynamicArray<string> map;
    DynamicArray<Enemy*> enemies;
    DynamicArray<Tower*> towers;
    DynamicArray<Position> path;
    int money;
    int lives;
    bool gameOver;
    bool gameWon;
    int currentWave;
    int waveSize;
    bool placementPhase;

    void clearScreen() {
        system("cls");
    }

    void findPath() {
        path.clear();

        int rows = map.getSize();
        if (rows == 0) return;
        int cols = map[0].length();

        Position start = { -1, -1 };
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                if (map[y][x] == 'x') {
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

            int dx[] = { 0,  0, -1, 1 };
            int dy[] = { -1, 1,  0, 0 };

            for (int d = 0; d < 4; d++) {
                Position next = { current.x + dx[d], current.y + dy[d] };

                if (next.x >= 0 && next.x < cols && next.y >= 0 && next.y < rows) {
                    if (map[next.y][next.x] == 'x') {
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

    void drawMap() {
        clearScreen();
        cout << "Money: $" << money << " | Lives: " << lives
            << " | Wave: " << currentWave << "/" << waveSize << "\n";
        cout << (placementPhase ? "PLACEMENT PHASE" : "WAVE IN PROGRESS") << "\n\n";

        DynamicArray<string> display = map;

        for (int i = 0; i < towers.getSize(); i++) {
            Position pos = towers[i]->getPosition();
            display[pos.y][pos.x] = towers[i]->getSymbol();
        }

        for (int i = 0; i < enemies.getSize(); i++) {
            if (enemies[i]->isAlive()) {
                Position pos = enemies[i]->getPosition();
                display[pos.y][pos.x] = enemies[i]->getSymbol();
            }
        }

        for (int i = 0; i < display.getSize(); i++) {
            cout << display[i] << endl;
        }
    }

    void spawnWave() {
        if (currentWave == 1) {
            enemies.push(new Clicker(path));
            //enemies.push(new Bloater(path));
        }
        else {
            enemies.push(new Clicker(path));
            enemies.push(new Clicker(path));
            enemies.push(new Bloater(path));
            enemies.push(new Bloater(path));
            enemies.push(new Bloater(path));
        }
    }

public:
    Game(const string& mapFile)
        : money(200), lives(3), gameOver(false),
        gameWon(false), currentWave(1), waveSize(2),
        placementPhase(true) {
        loadMap(mapFile);
        findPath();
    }

    ~Game() {
        for (int i = 0; i < enemies.getSize(); i++) delete enemies[i];
        for (int i = 0; i < towers.getSize(); i++) delete towers[i];
    }

    void loadMap(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            map.push(line);
        }
    }

    void showInstructions() {
        clearScreen();
        cout << "=== LAST OF US TOWER DEFENSE ===" << endl;
        cout << "Instructions:" << endl;
        cout << "- Place towers (J for Joel, E for Ellie) on '.' tiles" << endl;
        cout << "- Press 's' to start waves after placement" << endl;
        cout << "- Joel: High damage (25), short range (2), $50" << endl;
        cout << "- Ellie: Low damage (10), long range (3), $30" << endl;
        cout << "- Clickers (C): 30 HP, fast" << endl;
        cout << "- Bloaters (B): 100 HP, slow" << endl;
        cout << "\nPress Enter to begin placement phase...";
        cin.ignore();
    }

    bool placeTower(char type, int x, int y) {
        if (y < 0 || y >= map.getSize() || x < 0 || x >= map[y].length() || map[y][x] != '.') {
            cout << "Cannot place tower here!" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            return false;
        }

        for (int i = 0; i < towers.getSize(); i++) {
            if (towers[i]->getPosition().x == x && towers[i]->getPosition().y == y) {
                cout << "Position already occupied!" << endl;
                this_thread::sleep_for(chrono::seconds(1));
                return false;
            }
        }

        Tower* newTower = NULL;
        Position pos = { x, y };

        if (type == 'J' && money >= 50) {
            newTower = new JoelTower(pos);
            money -= 50;
        }
        else if (type == 'E' && money >= 30) {
            newTower = new EllieTower(pos);
            money -= 30;
        }
        else {
            cout << "Not enough money!" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            return false;
        }

        if (newTower != NULL) {
            towers.push(newTower);
            cout << "Tower placed successfully!" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            return true;
        }

        return false;
    }

    void update() {
        if (placementPhase) return;

        for (int i = 0; i < enemies.getSize(); i++) {
            if (enemies[i]->isAlive()) {
                enemies[i]->move();
                if (enemies[i]->reachedEnd()) {
                    lives--;
                    enemies[i]->takeDamage(enemies[i]->getHealth());
                    if (lives <= 0) gameOver = true;
                }
            }
        }

        for (int i = 0; i < towers.getSize(); i++) {
            towers[i]->attack(enemies);
        }

        for (int i = 0; i < enemies.getSize(); ) {
            if (!enemies[i]->isAlive()) {
                money += (enemies[i]->getSymbol() == 'C') ? 15 : 30;
                delete enemies[i];
                enemies.remove(i);
            }
            else {
                i++;
            }
        }

        if (enemies.getSize() == 0) {
            if (currentWave < waveSize) {
                currentWave++;
                placementPhase = true;
            }
            else {
                gameWon = true;
                gameOver = true;
            }
        }
    }

    void run() {
        showInstructions();

        while (!gameOver) {
            drawMap();

            if (placementPhase) {
                cout << "\nPlace towers (J/E) or press 's' to start wave: ";
                char input;
                cin >> input;
                input = toupper(input);

                if (input == 'S') {
                    placementPhase = false;
                    spawnWave();
                }
                else if (input == 'J' || input == 'E') {
                    int x, y;
                    cout << "Enter coordinates (x y): ";
                    cin >> x >> y;
                    placeTower(input, x, y);
                }
            }
            else {
                if (_kbhit()) {
                    char input = _getch();
                    if (toupper(input) == 'P') {
                        placementPhase = true;
                    }
                }
                update();
                this_thread::sleep_for(chrono::milliseconds(REFRESH_RATE_MS));
            }
        }

        drawMap();
        if (gameWon) {
            cout << "\nCongratulations! You survived the outbreak!" << endl;
        }
        else {
            cout << "\nGame Over! The infected overran your defenses!" << endl;
        }
    }
};


int main() {

	return 0;
}