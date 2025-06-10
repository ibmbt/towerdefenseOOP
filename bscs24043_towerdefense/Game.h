#ifndef GAME_H
#define GAME_H
#include "Globals.h"
#include "Tower.h"
#include "Map.h"
#include "dynamicArray.h"
#include "JoelTower.h"
#include "EllieTower.h"
#include "Clicker.h"
#include "Bloater.h"
#include "SoundManager.h"
using namespace std;



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


    Tower* selectedTowerForUpgrade = nullptr;

    float wavePreparationTime;
    float waveTimerRemaining;
    bool showWaveTimer;

    // Textures
    Texture2D clickerTexture;
    Texture2D bloaterTexture;
    Texture2D joelTowerTexture;
    Texture2D ellieTowerTexture;
    Texture2D menuBackground;


    SoundManager audio;



    void spawnEnemy() {
        Enemy* newEnemy = nullptr;

        switch (currentWave) {
        case 1:
            newEnemy = new Clicker(map.getPath());
            break;

        default:
            int enemyType = rand() % 2;
            switch (enemyType) {
            case 0:
                newEnemy = new Clicker(map.getPath());
                break;
            case 1:
                newEnemy = new Bloater(map.getPath());
                break;
            }
            break;
        }

        if (newEnemy) {
            enemies.push(newEnemy);
            enemiesToSpawn--;
        }
    }

    void startWave() {
        placementPhase = false;
        enemiesToSpawn = currentWave * 3;
        waveTimer = 0.0f;
        showWaveTimer = false;
    }

    void endWave() {
        placementPhase = true;
        currentWave++;
        waveTimerRemaining = wavePreparationTime;
        showWaveTimer = true;

        if (currentWave > waveSize) {
            gameWon = true;
            gameOver = true;
        }
    }

    void updateGameplay(float deltaTime) {

        if (placementPhase) {
            waveTimerRemaining -= deltaTime;
            if (waveTimerRemaining <= 0) {
                startWave();
            }
        }

        // Update towers
        for (int i = 0; i < player.getTowers().getSize(); i++) {
            player.getTowers()[i]->update(deltaTime, enemies);
        }

        // Update enemies
        for (int i = 0; i < enemies.getSize(); i++) {
            enemies[i]->update(deltaTime);
            if (enemies[i]->reachedEnd()) {
                if (enemies[i]->getMaxHealth() > 199) {
                    player.loseLife();
                }
                player.loseLife();
                delete enemies[i];
                enemies.remove(i);
                i--;
                if (player.getLives() <= 0) {
                    gameOver = true;
                }
            }
        }

        // Remove dead enemies
        for (int i = 0; i < enemies.getSize(); ) {
            if (!enemies[i]->isAlive()) {
                audio.playEnemyDeath();
                player.addCoins(enemies[i]->getWorth());
                delete enemies[i];
                enemies.remove(i);
            }
            else {
                i++;
            }
        }

        // Wave spawning
        if (!placementPhase) {
            waveTimer += deltaTime;
            if (waveTimer >= waveDelay && enemiesToSpawn > 0) {
                spawnEnemy();
                waveTimer = 0.0f;
            }

            if (enemiesToSpawn == 0 && enemies.getSize() == 0) {
                endWave();
            }
        }
    }

    void drawUI() {
        // Clear UI background
        DrawRectangle(0, 0, SCREEN_WIDTH, UI_HEIGHT, Color{ 50, 50, 80, 255 });

        // Left section - Player stats
        int leftX = 20;
        int leftY = 20;
        int lineHeight = 25;

        DrawText(TextFormat("Lives: %d", player.getLives()), leftX, leftY, 20, WHITE);
        DrawText(TextFormat("Coins: %d", player.getCoins()), leftX, leftY + lineHeight, 20, WHITE);
        DrawText(TextFormat("Wave: %d/%d", currentWave, waveSize), leftX, leftY + lineHeight * 2, 20, WHITE);

        if (showWaveTimer) {
            const char* timerText = TextFormat("Next wave in: %.1f", waveTimerRemaining);
            int timerWidth = MeasureText(timerText, 20);

            Color timerColor = GREEN;
            if (waveTimerRemaining < 10.0f) {
                timerColor = YELLOW;
            }
            if (waveTimerRemaining < 5.0f) {
                timerColor = ORANGE;
            }
            if (waveTimerRemaining < 3.0f) {
                timerColor = RED;
            }

            //ADD WARNING SOUND TOO

            DrawText(timerText, SCREEN_WIDTH / 2 - timerWidth / 2, UI_HEIGHT - 60, 20, timerColor);
        }

        int rightX = SCREEN_WIDTH - 300;
        int rightY = 20;
        int iconSize = 20;

        DrawText("TOWERS", rightX, rightY, 20, WHITE);

        DrawTextureEx(joelTowerTexture, Vector2{ (float)rightX, (float)rightY + lineHeight }, 0, 0.5f, WHITE);
        DrawText("Joel (50g)", rightX + iconSize + 10, rightY + lineHeight, 20, WHITE);
        DrawText("High damage", rightX + iconSize + 10, rightY + lineHeight + 20, 16, LIGHTGRAY);

        DrawTextureEx(ellieTowerTexture, Vector2{ (float)rightX, (float)rightY + lineHeight * 3 }, 0, 0.5f, WHITE);
        DrawText("Ellie (30g)", rightX + iconSize + 10, rightY + lineHeight * 3, 20, WHITE);
        DrawText("Fast attack", rightX + iconSize + 10, rightY + lineHeight * 3 + 20, 16, LIGHTGRAY);

        const char* statusText = placementPhase ?
            "PLACEMENT PHASE - PRESS SPACE TO START WAVE" :
            "WAVE IN PROGRESS";

        int textWidth = MeasureText(statusText, 20);
        DrawText(statusText, leftX, UI_HEIGHT - 30, 20,
            placementPhase ? YELLOW : ORANGE);

        if (SELECTED_TOWER != '\0') {
            const char* msg = "";

            switch (toupper(SELECTED_TOWER)) {
            case 'J':
                msg = "Selected: Joel Tower (50g)";
                break;
            case 'E':
                msg = "Selected: Ellie Tower (30g)";
                break;

                // Add new towers if needed

            default:
                msg = "Select a tower";
                break;
            }

            DrawText(msg, (SCREEN_WIDTH / 2) - 150, leftY + 10, 20, YELLOW);
        }

    }

    void drawGame() {

        map.draw();

        for (int i = 0; i < player.getTowers().getSize(); i++) {
            player.getTowers()[i]->drawRange();
        }


        for (int i = 0; i < enemies.getSize(); i++) {
            Enemy* e = enemies[i];
            Texture2D* texture = nullptr;

            switch (e->getSymbol()) {
            case 'C':
                texture = &clickerTexture;
                break;
            case 'B':
                texture = &bloaterTexture;
                break;

                // Add new enemies here in case
            }

            if (texture) {
                float x = e->getPosition().x * CELL_SIZE;
                float y = e->getPosition().y * CELL_SIZE + UI_HEIGHT;
                DrawTexture(*texture, x, y, WHITE);

                float healthPercent = (float)e->getHealth() / e->getMaxHealth();
                DrawRectangle(x, y - 10, CELL_SIZE * healthPercent, 5, DARKGREEN);
                DrawRectangleLines(x, y - 10, CELL_SIZE, 5, BLACK);
            }
        }

        for (int i = 0; i < player.getTowers().getSize(); i++) {
            Tower* t = player.getTowers()[i];
            Texture2D* texture = nullptr;

            switch (t->getSymbol()) {
            case 'J':
                texture = &joelTowerTexture;
                break;
            case 'E':
                texture = &ellieTowerTexture;
                break;
                // Add new towers in case statement
            }

            if (texture) {
                DrawTexture(*texture,
                    int(t->getPosition().x) * CELL_SIZE,
                    int(t->getPosition().y) * CELL_SIZE + UI_HEIGHT,
                    WHITE);
            }
        }


        if (!(IsKeyDown(KEY_LEFT_ALT))) {
            for (int i = 0; i < player.getTowers().getSize(); i++) {
                Vector2 mousepos = GetMousePosition();
                mousepos.y += 20;
                player.getTowers()[i]->displayInfo(mousepos);
            }
        }

    }

    Tower* getTowerUnderMouse(Vector2 mousePos) {
        int gridX = mousePos.x / CELL_SIZE;
        int gridY = (mousePos.y - UI_HEIGHT) / CELL_SIZE;

        for (int i = 0; i < player.getTowers().getSize(); i++) {
            Tower* t = player.getTowers()[i];
            if (t->getPosition().x == gridX &&
                t->getPosition().y == gridY) {
                return t;
            }
        }
        return nullptr;
    }


public:

    Game() : player(10, 200), map(), gameOver(false), gameWon(false),
        currentWave(1), waveSize(5), placementPhase(true), waveTimer(0.0f),
        waveDelay(1.0f), enemiesToSpawn(0),
        wavePreparationTime(15.0f), 
        waveTimerRemaining(30.0f),
        showWaveTimer(false) {

        clickerTexture = LoadTexture("resources/sprites/clicker.png");
        bloaterTexture = LoadTexture("resources/sprites/bloater.png");
        joelTowerTexture = LoadTexture("resources/sprites/joel.png");
        ellieTowerTexture = LoadTexture("resources/sprites/ellie.png");
        menuBackground = LoadTexture("resources/sprites/menu2.png");


    }

    ~Game() {

        UnloadTexture(clickerTexture);
        UnloadTexture(bloaterTexture);
        UnloadTexture(joelTowerTexture);
        UnloadTexture(ellieTowerTexture);
        UnloadTexture(menuBackground);


        for (int i = 0; i < enemies.getSize(); i++) {
            delete enemies[i];
        }
    }


    GameState currentState;
    int selectedLevel;


    void drawMainMenu() {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        DrawTexturePro(
            menuBackground,
            Rectangle{ 0, 0, (float)menuBackground.width, (float)menuBackground.height },
            Rectangle{ 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT },
            Vector2{ 0, 0 },
            0.0f,
            WHITE
        );

        // Title
        DrawText("THE LAST OF US TOWER DEFENSE",
            SCREEN_WIDTH / 2 - MeasureText("THE LAST OF US TOWER DEFENSE", 40) / 2,
            100, 40, WHITE);

        // Level buttons
        Rectangle level1Btn = { SCREEN_WIDTH / 2 - 100, 200, 200, 50 };
        Rectangle level2Btn = { SCREEN_WIDTH / 2 - 100, 280, 200, 50 };
        Rectangle level3Btn = { SCREEN_WIDTH / 2 - 100, 360, 200, 50 };

        // Draw buttons
        DrawRectangleRec(level1Btn, selectedLevel == 1 ? RED : GRAY);
        DrawRectangleRec(level2Btn, selectedLevel == 2 ? RED : GRAY);
        DrawRectangleRec(level3Btn, selectedLevel == 3 ? RED : GRAY);

        // Button labels
        DrawText("LEVEL 1", level1Btn.x + 50, level1Btn.y + 15, 20, BLACK);
        DrawText("LEVEL 2", level2Btn.x + 50, level2Btn.y + 15, 20, BLACK);
        DrawText("LEVEL 3", level3Btn.x + 50, level3Btn.y + 15, 20, BLACK);

        // Instructions
        DrawText("Use ARROW KEYS to select, ENTER to confirm, ESC to exit",
            SCREEN_WIDTH / 2 - MeasureText("Use ARROW KEYS to select, ENTER to confirm, ESC to exit", 20) / 2,
            500, 20, LIGHTGRAY);

        EndDrawing();
    }

    void handleMenuInput() {
        if (IsKeyPressed(KEY_DOWN)) {
            selectedLevel = (selectedLevel % 3) + 1;
        }
        if (IsKeyPressed(KEY_UP)) {
            selectedLevel = (selectedLevel - 2 + 3) % 3 + 1;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            switch (selectedLevel) {
            case 1:
                player.setPlayer(10, 200);
                map.load("resources/maps/map1.txt");
                currentState = LEVEL_1;

                break;
            case 2:
                player.setPlayer(8, 150);
                map.load("resources/maps/map2.txt");
                currentState = LEVEL_2;
                break;
            case 3:
                player.setPlayer(5, 100);
                map.load("resources/maps/map3.txt");
                currentState = LEVEL_3;
                break;
            }

            currentWave = 1;
            player.resetPlayer();
            map.reset();
            gameOver = false;
        }
    }


    void handleInput() {

        Vector2 mousepos = GetMousePosition();
        mousepos.y += 20;
        Tower* hoveredTower = getTowerUnderMouse(mousepos);

        for (int i = 0; i < player.getTowers().getSize(); i++) {
            player.getTowers()[i]->setShowRange(false);
        }

        if (hoveredTower) {
            hoveredTower->setShowRange(true);
        }
        selectedTowerForUpgrade = hoveredTower;
        handleUpgradeInput();
        selectedTowerForUpgrade = nullptr;

        if (IsKeyDown(KEY_LEFT_ALT)) {
            for (int i = 0; i < player.getTowers().getSize(); i++) {
                player.getTowers()[i]->setShowRange(true);
            }
        }

        if (IsKeyPressed(KEY_J)) SELECTED_TOWER = 'J';
        if (IsKeyPressed(KEY_E)) SELECTED_TOWER = 'E';

        if (placementPhase) {

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                int gridX = mousePos.x / CELL_SIZE;
                int gridY = (mousePos.y - UI_HEIGHT + 20) / CELL_SIZE;

                if (map.getCell(gridX, gridY).isEmpty()) {
                    Tower* newTower = nullptr;
                    switch (toupper(SELECTED_TOWER)) {
                    case 'J':
                        if (player.canAfford(50)) {
                            newTower = new JoelTower({ (float)gridX, (float)gridY });
                            audio.playTowerPlace();
                            player.deductCoins(50);
                        }
                        break;
                    case 'E':
                        if (player.canAfford(30)) {
                            newTower = new EllieTower({ (float)gridX, (float)gridY });
                            audio.playTowerPlace();
                            player.deductCoins(30);
                        }
                        break;
                    }
                    if (newTower) {
                        player.addTower(newTower);
                        map.getCell(gridX, gridY).placeTower();
                    }
                }
            }


            if (IsKeyPressed(KEY_SPACE)) {
                startWave();
            }
        }
    }


    void update(float deltaTime) {
        if (gameOver) return;
        updateGameplay(deltaTime);

    }

    void draw() {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawUI();
        drawGame();


        if (gameOver) {
            endWave();
            enemies.clear();
            audio.playGameOver();
            currentState = GAME_OVER;
        }

        EndDrawing();
    }


    void drawGameOverScreen() {
        BeginDrawing();

        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{ 0, 0, 0, 200 });

        Rectangle box = {
            SCREEN_WIDTH / 2 - 250,
            SCREEN_HEIGHT / 2 - 150,
            500,
            300
        };
        DrawRectangleRounded(box, 0.1f, 8, Color{ 40, 40, 60, 255 });
        DrawRectangleRoundedLines(box, 0.1f, 8, PURPLE);

        const char* title = gameWon ? "LEVEL COMPLETE!" : "GAME OVER";
        Color titleColor = gameWon ? GREEN : RED;
        DrawText(title,
            SCREEN_WIDTH / 2 - MeasureText(title, 40) / 2,
            SCREEN_HEIGHT / 2 - 100,
            40, titleColor);

        DrawText(TextFormat("Waves Survived: %d/%d", currentWave - 1, waveSize),
            SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 30, 20, WHITE);
        DrawText(TextFormat("Towers Built: %d", player.getTowers().getSize()),
            SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, 20, WHITE);
        DrawText(TextFormat("Coins Earned: %d", player.getCoins()),
            SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 30, 20, WHITE);

        Rectangle menuBtn = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 130, 200, 40 };


        DrawRectangleRounded(menuBtn, 0.3f, 8, DARKGRAY);
        DrawRectangleRoundedLines(menuBtn, 0.3f, 8, LIGHTGRAY);
        DrawText("MAIN MENU",
            menuBtn.x + menuBtn.width / 2 - MeasureText("MAIN MENU", 20) / 2,
            menuBtn.y + 10, 20, WHITE);

        DrawText("M for menu",
            SCREEN_WIDTH / 2 - MeasureText("M for menu", 18) / 2,
            SCREEN_HEIGHT - 50, 18, LIGHTGRAY);

        EndDrawing();
    }


    void handleUpgradeInput() {

        if (IsKeyPressed(KEY_ONE)) {
            if (selectedTowerForUpgrade->canUpgrade(1, &player)) {
                player.deductCoins(selectedTowerForUpgrade->getUpgradeCost(1));
                audio.playSell();
                selectedTowerForUpgrade->upgradePath1();
            }
        }

        if (IsKeyPressed(KEY_TWO)) {
            if (selectedTowerForUpgrade->canUpgrade(2, &player)) {
                player.deductCoins(selectedTowerForUpgrade->getUpgradeCost(2));
                audio.playSell();
                selectedTowerForUpgrade->upgradePath2();
            }
        }

        if (IsKeyPressed(KEY_S)) {
            if (player.sellTower(selectedTowerForUpgrade)) {
                player.addCoins((float)(selectedTowerForUpgrade->getCost()) * 0.5);
                Position pos = selectedTowerForUpgrade->getPosition();
                map.getCell(pos.x, pos.y).removeTower();

                selectedTowerForUpgrade = nullptr;

                audio.playSell();
            }
        }

    }



    bool shouldClose() const {
        return gameOver && IsKeyPressed(KEY_ESCAPE);
    }
};


#endif