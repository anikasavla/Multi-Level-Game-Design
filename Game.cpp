// Game.cpp

#include "Game.h"
#include "Temple.h"
#include "Actor.h"
#include "utilities.h"
#include <iostream>
using namespace std;


Game::Game(int goblinSmellDistance) : level(goblinSmellDistance) {
    playerHasWon = false;
}

void Game::play()
{
    //generate the level and set the player's starting location to a random location
    level.generateLevel(myPlayer.getLevel());
    Coord empty = level.findRandomEmpty();
    myPlayer.setCurrentPos(empty.getY(), empty.getX());
    level.changeCoordType(myPlayer.getCurrentPos(), PLAYER);
    
    //print out level, display player stats and collect user move
    level.printLevel();
    myPlayer.displayStats();
    char move = getCharacter();
    cout << endl;
    clearScreen();

    // while the player has not lost, won, or quit
    while (move != 'q' && !playerHasWon && !playerHasLost()) {
        //1/10 chance that player gains a hit point every turn
        if (trueWithProbability(.1)){ myPlayer.setHitPoints(myPlayer.getHitPoints() + 1); }
        // collect the player's moves
        if (move == ARROW_LEFT || move == ARROW_RIGHT || move == ARROW_DOWN || move == ARROW_UP) {
            movePlayer(move);
        }
        else {
            switch(move) {
                case 's':
                    myPlayer.setSleepTime(myPlayer.getSleepTime() -1);
                    break;
                case 'g':
                    pickUpItem();
                    break;
                case 'w':
                    wieldWeapon();
                    //trying to wield weapon
                    break;
                case 'r':
                    useScroll();
                    //scroll
                    break;
                case 'i':
                    myPlayer.displayInventory();
                    break;
                case 'c':
                    //cheat
                    myPlayer.setMaxHitPoints(50);
                    myPlayer.setHitPoints(50);
                    myPlayer.setStrengthPoints(9);
                    break;
                case '>':
                    descend();
                    break;
                default:
                    break;
            }
        }
        //while the player has been put to sleep
        if (myPlayer.getSleepTime() > 0) {
            level.moveMonsters(myPlayer);
            level.printLevel();
            myPlayer.displayStats();
            move = 's';
            clearScreen();
        }
        else {
            //player is not asleep and has not won or lost so the game continues
            level.moveMonsters(myPlayer);
            level.printLevel();
            myPlayer.displayStats();
            move = getCharacter();
            cout<< endl;
            clearScreen();
        }
        
        //if the player has won or lost
        if (playerHasWon || playerHasLost()) {
            cout << "Press q to quit the game."<< endl;
            move = getCharacter();
        }
    }
}

void Game::pickUpItem() {
    //when the players aimed next location is a weapon
    
    //if the weapon is one of the one's dropped by the monsters who were killed, assigns accordinly
    if (level.getOldType() == WEAPON) {
        for (int i = 0; i < level.getKilledMonsters()->size(); i++) {
            Coord& monsterCoord = (*level.getKilledMonsters())[i];
            if (monsterCoord.getX() == myPlayer.getCurrentPos().getX() &&
                monsterCoord.getY() == myPlayer.getCurrentPos().getY()) {
                myPlayer.addWeaponToInventory(Weapons((*level.getDroppedWeapons())[i]));
                level.removeDroppedWeapon((*level.getDroppedWeapons())[i]);
                level.removeKilledMonster(monsterCoord);
                level.setOldType(EMPTY);
                return;
            }
        }
        //otherwise randomly chooses one of the three possible types of weapons
        int whichItem = randInt(3);
        if (whichItem == 1) {
            myPlayer.addWeaponToInventory(Weapons("short sword"));
        }
        else if (whichItem == 2) {
            myPlayer.addWeaponToInventory(Weapons("long sword"));
        }
        else {
            myPlayer.addWeaponToInventory(Weapons("mace"));
        }
        level.setOldType(EMPTY);
    }
    else if (level.getOldType() == SCROLL) {
        myPlayer.addScrollToInventory(Scrolls(whichScroll()));
        level.setOldType(EMPTY);
    }
    else if (level.getOldType() == IDOL) {
        playerHasWon = true;
        cout << "the golden idol" << endl;
        cout << "Congratulations, you won!" << endl;
    }
}

//randomly choose which scroll is picked up
string Game::whichScroll() {
    int whichItem = randInt(5);
    if (whichItem == 1) {
        return "scroll of teleportation";
    }
    else if (whichItem == 2) {
        return "scroll of strength";
    }
    else if (whichItem == 3) {
        return "scroll of improve armor";
    }
    else if (whichItem == 4) {
        return "scroll of enhance health";
    }
    else {
        return "scroll of enhance dexterity";
    }
}

//check if within inventory, the item that the player is picking is a weapon
void Game::wieldWeapon() {
    clearScreen();
    myPlayer.displayInventory();
    char selection = getCharacter();
    Weapons whichWeapon("");
    if (myPlayer.isWeapon(selection, whichWeapon)) {
        myPlayer.setWeapon(whichWeapon);
        cout << "You are wielding a " << whichWeapon.getObjectType() << endl;
    }
    else {
        cout << "You can't wield a " << whichWeapon.getObjectType() << endl;
    }
}

//check if within inventory, the item that the player is picking is a scroll
//once the scroll is read, adjust the player stats accordingly and destroy scroll, removing from inventory
void Game::useScroll() {
    clearScreen();
    myPlayer.displayInventory();
    char selection = getCharacter();
    Scrolls whichScroll("");
    if (myPlayer.isScroll(selection, whichScroll)) {
        cout << "You read a scroll called " << whichScroll.getObjectType() << endl;
        if (whichScroll.getObjectType() == "scroll of teleportation") {
            cout << "You feel your body wrenched in space and time." << endl;
            level.setOldType(EMPTY);
            level.changeCoordType(myPlayer.getCurrentPos(), level.getOldType());
            Coord empty = level.findRandomEmpty();
            myPlayer.setCurrentPos(empty.getY(), empty.getX());
            level.changeCoordType(myPlayer.getCurrentPos(), PLAYER);
        }
        else if (whichScroll.getObjectType() == "scroll of strength") {
            cout << "You muscles bulge." << endl;
            myPlayer.setStrengthPoints(myPlayer.getStrengthPoints() + randInt(1, 3));
        }
        else if (whichScroll.getObjectType() == "scroll of improve armor") {
            cout << "Your armor glows blue." << endl;
            myPlayer.setArmorPoints(myPlayer.getArmorPoints()+ randInt(1,3));
        }
        else if (whichScroll.getObjectType() == "scroll of enhance health") {
            cout << "You feel your heart beating stronger." << endl;
            myPlayer.setMaxHitPoints(myPlayer.getMaxHitPoints() + randInt(3,8));
        }
        else if (whichScroll.getObjectType() == "scroll of enhance dexterity") {
            cout << "You feel like less of a klutz." << endl;
            myPlayer.setDexterityPoints(myPlayer.getDexterityPoints() + 1);
        }
        myPlayer.removeScrollFromInventory(selection);
    }
    else {
        cout << "You can't read a " << whichScroll.getObjectType() << endl;
    }
}

//check that the player is over a stairwell and if so create a new level

void Game::descend() {
    if (level.getOldType() == STAIRWAY) {
        level.cleanUpLevel();
        level.generateLevel(myPlayer.getLevel()+1);
        Coord empty = level.findRandomEmpty();
        myPlayer.setCurrentPos(empty.getY(), empty.getX());
        level.changeCoordType(myPlayer.getCurrentPos(), PLAYER);
        level.setOldType(EMPTY);
        myPlayer.setLevel(myPlayer.getLevel()+1);
    }
    
}


// if the player has taken too much damage, they have lost

bool Game::playerHasLost() {
    if (myPlayer.getHitPoints() <= 0) {
        return true;
    }
    return false;
}


//movement of the player
void Game::movePlayer(char move) {
    //determine what move the player is making and what is in the spot they are aiming to move to
    int changeInY = 0, changeInX = 0;
    if (move == ARROW_LEFT) { changeInX = -1;}
    if (move == ARROW_RIGHT) { changeInX = 1;}
    if (move == ARROW_DOWN) { changeInY = -1; }
    if (move == ARROW_UP) {changeInY = 1;}
    // what type the next Coord is
    char spotType = level.getType(myPlayer.getCurrentPos().getY() + changeInY, myPlayer.getCurrentPos().getX() + changeInX);
    //check that the next place a player is going is a monster, so it must attack
    if (spotType == BOGEYMEN || spotType == GOBLINS || spotType == SNAKEWOMEN || spotType == DRAGONS) {
        Monster* monsterPointer = level.whichMonster(myPlayer.getCurrentPos().getY() + changeInY, myPlayer.getCurrentPos().getX() + changeInX);
        string monster;
        if (spotType == BOGEYMEN) {monster = "Bogeyman";}
        else if (spotType == GOBLINS) {monster = "Goblin";}
        else if (spotType == SNAKEWOMEN) {monster = "Snakewoman";}
        else if (spotType == DRAGONS) {monster = "Dragon";}
        // attack monster and see if it was a hit or miss
        if (myPlayer.getWeapon().weaponAttack(myPlayer.getDexterityPoints(), monsterPointer->getDexterityPoints(), monsterPointer->getArmorPoints())) {
            int damage = myPlayer.getWeapon().damageAmount(myPlayer.getStrengthPoints());
            monsterPointer->setHitPoints(monsterPointer->getHitPoints() - damage);
            
            // case that the monster is destroyed
            
            if (monsterPointer -> getHitPoints() <= 0) {
                cout << "Player " << myPlayer.getWeapon().getActionString() << monster << " dealing a final blow." << endl;
                
                // checking if the destroyed monsters dropped anything
                
                if (spotType == DRAGONS && monsterPointer->getOldType() == EMPTY){
                    level.changeCoordType(monsterPointer->getCurrentPos(), SCROLL);
                }
                else if (spotType == SNAKEWOMEN && monsterPointer->getOldType() == EMPTY && randInt(1,3) == 3) {
                    level.changeCoordType(monsterPointer->getCurrentPos(),WEAPON);
                    level.addKilledMonster(monsterPointer->getCurrentPos());
                    level.addDroppedWeapon("magic fangs");
                }
                else if (spotType == GOBLINS && monsterPointer->getOldType() == EMPTY && randInt(1,3) == 3) {
                    level.changeCoordType(monsterPointer->getCurrentPos(), WEAPON);
                    level.addKilledMonster(monsterPointer->getCurrentPos());
                    if (trueWithProbability(0.5)) { level.addDroppedWeapon("magic fangs");}
                    else { level.addDroppedWeapon("magic axe");}
                }
                else if (spotType == BOGEYMEN && monsterPointer->getOldType() == EMPTY && randInt(1,10) == 10) {
                    level.changeCoordType(monsterPointer->getCurrentPos(), WEAPON);
                    level.addDroppedWeapon("magic axe");
                }
                else {
                    level.changeCoordType(monsterPointer->getCurrentPos(), monsterPointer->getOldType());
                }
            
                // destroys the monster object and removes from vector
                level.destroyMonster(monsterPointer);
            }
            
            // case that the monster is alive but hit
            else {
                cout << "Player " << myPlayer.getWeapon().getActionString() << monster << " and hits." << endl;
            }
        }
        else {
            cout << "Player " << myPlayer.getWeapon().getActionString() << monster << " and misses." << endl;
        }
    }
    // player moving to an open space
    else if (spotType != WALL) {
        Coord newPos(myPlayer.getCurrentPos().getY() + changeInY, myPlayer.getCurrentPos().getX() + changeInX);
        level.changeCoordType(myPlayer.getCurrentPos(), level.getOldType());
        level.setOldType(spotType);
        myPlayer.setCurrentPos(newPos.getY(), newPos.getX());
        level.changeCoordType(newPos, PLAYER);
    }
}



