#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

struct Player {
    int x, y;
    int health;
    std::vector<std::string> inventory;
    int level;
    int exp;
};

void initGame();
void drawMap(const Player& player);
void movePlayer(Player& player, char direction);
void gameLoop();
void saveGame(const Player& player);
void loadGame(Player& player);
void interact(Player& player);
void showMenu();
void usePotion(Player& player);

#endif
