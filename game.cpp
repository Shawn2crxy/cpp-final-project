#include "game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int WIDTH = 5, HEIGHT = 5;
char map[HEIGHT][WIDTH] = {
    {'#', '#', '#', '#', '#'},
    {'#', 'T', ' ', 'E', '#'},
    {'#', '^', '#', ' ', '#'},
    {'#', ' ', 'G', ' ', '#'},
    {'#', '#', '#', '#', '#'}
};

void initGame() {
    srand(time(0));
    showMenu();
    gameLoop();
}

void drawMap(const Player& player) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (i == player.y && j == player.x)
                cout << 'P';
            else
                cout << map[i][j];
        }
        cout << endl;
    }
    cout << "Health: " << player.health << " | Level: " << player.level << " | EXP: " << player.exp << "\n";
    cout << "Inventory: ";
    for (const auto& item : player.inventory) cout << item << " ";
    cout << endl;
}

void movePlayer(Player& player, char direction) {
    int newX = player.x, newY = player.y;
    switch (direction) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
    }
    if (map[newY][newX] != '#') {
        player.x = newX;
        player.y = newY;
        interact(player);
    } else {
        cout << "You hit a wall!\n";
    }
}

void interact(Player& player) {
    char tile = map[player.y][player.x];
    if (tile == 'G') {
        cout << "A wild goblin appears!\n";
        while (true) {
            cout << "Choose action: (A)ttack, (D)odge, or (F)ireball: ";
            char choice;
            cin >> choice;
            if (choice == 'a' || choice == 'A') {
                int dmg = rand() % 15 + 5;
                cout << "You hit the goblin for " << dmg << " damage!\n";
                player.exp += 10;
                if (rand() % 2 == 0) {
                    cout << "The goblin drops a potion!\n";
                    player.inventory.push_back("Potion");
                }
                cout << "Goblin defeated!\n";
                map[player.y][player.x] = ' ';
                break;
            } else if (choice == 'd' || choice == 'D') {
                int goblinHit = rand() % 10 + 5;
                cout << "You try to dodge but take " << goblinHit << " damage.\n";
                player.health -= goblinHit;
                if (player.health <= 0) {
                    cout << "You have been defeated... Game Over.\n";
                    exit(0);
                }
                break;
            } else if (choice == 'f' || choice == 'F') {
                cout << "You cast a fireball! The goblin is incinerated.\n";
                map[player.y][player.x] = ' ';
                player.exp += 10;
                break;
            } else {
                cout << "Invalid action. Try again.\n";
            }
        }
    } else if (tile == 'T') {
        cout << "A terrifying Troll blocks your path!\n";
        int trollHealth = 2;
        while (trollHealth > 0) {
            cout << "Choose action: (A)ttack or (D)odge: ";
            char choice;
            cin >> choice;
            if (choice == 'a' || choice == 'A') {
                int dmg = rand() % 10 + 10;
                cout << "You slash the Troll for " << dmg << " damage!\n";
                trollHealth--;
                if (trollHealth <= 0) {
                    cout << "The Troll collapses with a roar!\n";
                    map[player.y][player.x] = ' ';
                    player.exp += 20;
                    if (rand() % 2 == 0) {
                        player.inventory.push_back("Potion");
                        cout << "The Troll dropped a Potion!\n";
                    }
                    break;
                }
            } else if (choice == 'd' || choice == 'D') {
                int hit = rand() % 15 + 10;
                player.health -= hit;
                cout << "You try to dodge but the Troll hits you for " << hit << " damage!\n";
                if (player.health <= 0) {
                    cout << "The Troll crushes you... Game Over.\n";
                    exit(0);
                }
            } else {
                cout << "Invalid action. Try again.\n";
            }
        }
    } else if (tile == '^') {
        int trapDmg = rand() % 15 + 5;
        player.health -= trapDmg;
        cout << "You stepped on a trap! Took " << trapDmg << " damage.\n";
        map[player.y][player.x] = ' ';
        if (player.health <= 0) {
            cout << "You succumbed to your injuries... Game Over.\n";
            exit(0);
        }
    } else if (tile == 'E') {
        cout << "You found the exit! You win!\n";
        exit(0);
    }

    if (player.exp >= 30 && player.level == 1) {
        player.level++;
        player.health += 20;
        cout << "You leveled up to Level " << player.level << "! Health increased!\n";
    }
}

void usePotion(Player& player) {
    for (size_t i = 0; i < player.inventory.size(); ++i) {
        if (player.inventory[i] == "Potion") {
            player.health += 20;
            cout << "You used a Potion! Health is now " << player.health << "\n";
            player.inventory.erase(player.inventory.begin() + i);
            return;
        }
    }
    cout << "You have no potions!\n";
}

void saveGame(const Player& player) {
    ofstream out("save.txt");
    out << player.x << ' ' << player.y << ' ' << player.health << ' ' << player.level << ' ' << player.exp << endl;
    for (const string& item : player.inventory)
        out << item << endl;
    out.close();
}

void loadGame(Player& player) {
    ifstream in("save.txt");
    if (in) {
        in >> player.x >> player.y >> player.health >> player.level >> player.exp;
        string item;
        while (in >> item)
            player.inventory.push_back(item);
    }
    in.close();
}

void showMenu() {
    cout << "=== Escape the Dungeon Deluxe ===\n";
    cout << "WASD to move, Q to quit, U to use potion\n";
}

void gameLoop() {
    Player player = {1, 1, 100, {}, 1, 0};
    char input;
    while (true) {
        drawMap(player);
        cout << "Enter command: ";
        cin >> input;
        if (input == 'q') break;
        else if (input == 'u') usePotion(player);
        else movePlayer(player, input);
    }
}
