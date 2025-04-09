#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

struct Player {
    int x, y;
    int health, maxHealth;
    int attack;
    int level;
    int exp;
    std::vector<std::string> inventory;
};

void initGame();
void drawMap(const Player& player);
void movePlayer(Player& player, char direction);
void gameLoop();
void saveGame(const Player& player, int slot);
void loadGame(Player& player, int slot);
void interact(Player& player);
void showMenu();
void useItem(Player& player);
void levelUp(Player& player);
void loadMap(const std::string& filename);

#endif