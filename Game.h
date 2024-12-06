// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <string>
#include <iostream>
#include "Actor.h"
#include "Temple.h"
using namespace std;

// You may add data members and other member functions to this class.

class Game{
public:
    Game(int goblinSmellDistance);
    void play();
    bool playerHasWon;
private:
    //create player object
    Player myPlayer;
    //create level object that will change for each descending level
    Level level;
    void pickUpItem();
    string whichScroll();
    void wieldWeapon();
    void useScroll();
    void descend();
    bool playerHasLost();
    void movePlayer(char move);
};

#endif // GAME_INCLUDED
