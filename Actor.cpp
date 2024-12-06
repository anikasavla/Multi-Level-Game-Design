#include "Actor.h"
#include "Temple.h"
#include "GameObjects.h"
#include "utilities.h"
#include <iostream>
#include <string>

using namespace std;

//constructor for Actor
Actor::Actor(int specHitPoints, string specWeaponType, int specArmorPoints, int specStrengthPoints, int specDexterityPoints) : currentPos(randInt(0, 18), randInt(0, 70)), weapon(specWeaponType) {
    hitPoints = specHitPoints;
    armorPoints = specArmorPoints;
    strengthPoints = specStrengthPoints;
    dexterityPoints = specDexterityPoints;
    //inventory
    
}

Actor::~Actor(){}

// get the actor's current position
Coord Actor::getCurrentPos(){
    return currentPos;
}

//set the actor's location
void Actor::setCurrentPos(int specY, int specX) {
    currentPos.setY(specY);
    currentPos.setX(specX);
}

int Actor::getHitPoints() {
    return hitPoints;
}

void Actor::setHitPoints(int specHitPoints) {
    hitPoints = specHitPoints;
}

int Actor::getArmorPoints() {
    return armorPoints;
}

void Actor::setArmorPoints(int specArmorPoints) {
    armorPoints = specArmorPoints;
}

int Actor::getStrengthPoints() {
    return strengthPoints;
}
    
void Actor::setStrengthPoints(int specStrengthPoints) {
    strengthPoints = specStrengthPoints;
}

int Actor::getDexterityPoints() {
    return dexterityPoints;
}

void Actor::setDexterityPoints(int specDexterityPoints) {
    dexterityPoints = specDexterityPoints;
}

//set the weapon that the actor has
void Actor::setWeapon(Weapons specWeapon) {
    weapon = specWeapon;
}

// get the weapon the actor is using
Weapons Actor::getWeapon() {
    return weapon;
}

//add sleep time
void Actor::setSleepTime(int specSleepTime) {
    if (specSleepTime > 9) {
        sleepTime = 9;
    }
    else {
        sleepTime = specSleepTime;
    }
}

int Actor::getSleepTime() {
    return sleepTime;
}

// player constructor
Player::Player() : Actor(20, "short sword", 2, 2, 2) {
    inventoryItems = 0;
    maxHitPoints = 20;
    addWeaponToInventory(Weapons("short sword"));
    level = 0;
}

//player destructor
Player::~Player() {
    for (int i = 0; i < inventoryItems; i++) {
            delete inventory[i];
            inventory[i] = nullptr;
        }
}

//change the player's max hit points
void Player::setMaxHitPoints(int specMaxHitPoints) {
    maxHitPoints = specMaxHitPoints;
}

int Player::getMaxHitPoints() {
    return maxHitPoints;
}

// change the player's hit points to parameter or max if exceeding current max

void Player::setHitPoints(int specHitPoints) {
    if (specHitPoints > maxHitPoints) {
        Actor::setHitPoints(maxHitPoints);
    }
    else {
        Actor::setHitPoints(specHitPoints);
    }
}

int Player::getLevel() {
    return level;
}

void Player::setLevel(int newLevel){
    level = newLevel;
}

void Player::displayStats() {
    if (getHitPoints() < 0) {setHitPoints(0);}
    cout << "Level: " << level << ", ";
    cout << "Hit points: " << getHitPoints() << ", ";
    cout << "Armor points: " << getArmorPoints() << ", ";
    cout << "Strength points: " << getStrengthPoints() << ", ";
    cout << "Dexterity points: " << getDexterityPoints() << endl;
}

void Player::displayInventory() {
    cout << "Inventory: " << endl;
    for (int num = 0; num < inventoryItems; num++ ) {
        cout << alphabet[num] << ". " << inventory[num]->getObjectType() << endl;
    }
}

//checks that the character the player chose is designated for a weapon
bool Player::isWeapon(char character, Weapons &whichWeapon) {
    //match the alphabet letter to the corresponding item in the inventory
    int i = 0;
    for (; i < 25; i++) {
        if (character == alphabet[i]) {
            break;
        }
    }
    if (i < inventoryItems) {
        whichWeapon.setObjectType(inventory[i]->getObjectType());
        if (inventory[i]->getObjectType() == "mace" || inventory[i]->getObjectType() == "short sword" || inventory[i]->getObjectType() == "long sword" || inventory[i]->getObjectType() == "magic axe" || inventory[i]->getObjectType() == "magic fangs") {
            whichWeapon.setActionString(inventory[i]->getActionString());
            whichWeapon.setDexterityBonus(inventory[i]->getDexterityBonus());
            whichWeapon.setDamageAmount(inventory[i]->getDamageAmount());
            return true;
        }
    }
    return false;
}

//checks that the character the player chose is designated for a scroll

bool Player::isScroll(char character, Scrolls &whichScroll) {
    //match the alphabet letter to the corresponding item in the inventory

    int i = 0;
    for (; i < 25; i++) {
        if (character == alphabet[i]) {
            break;
        }
    }
    if (i < inventoryItems) {
        whichScroll.setObjectType(inventory[i]->getObjectType());
        if (inventory[i]->getObjectType() == "scroll of teleportation" || inventory[i]->getObjectType() == "scroll of improve armor" || inventory[i]->getObjectType() == "scroll of strength" || inventory[i]->getObjectType() ==  "scroll of enhance health" || inventory[i]->getObjectType() == "scroll of enhance dexterity") {
            return true;
        }
    }
    return false;
}

//add a weapon to the player's inventory if there is space
void Player::addWeaponToInventory(Weapons item) {
    if (inventoryItems < 25) {
        inventory[inventoryItems] = new Weapons(item.getObjectType());
        inventoryItems++;
        if (inventoryItems != 1) {
            cout << "You pick up a " << item.getObjectType() << endl;
        }
    }
    else {
        cout << "Your knapsack is full; you can't pick that up." << endl;;
    }
}

//add a scroll to the player's inventory if there is space

void Player::addScrollToInventory(Scrolls item) {
    if (inventoryItems < 25) {
        inventory[inventoryItems] = new Scrolls(item.getObjectType());
        inventoryItems++;
        cout << "You pick up a scroll called " << item.getObjectType() << endl;
    }
    else {
        cout << "Your knapsack is full; you can't pick that up." << endl;
    }
}

//remove and destroy a scroll in the player's inventory when it is read
void Player::removeScrollFromInventory(char selection) {
    int i = 0;
    // Find the index of the selected scroll
    for (; i < 25; i++) {
        if (selection == alphabet[i]) {
            break;
        }
    }
    
    if (i < 25 && inventory[i] != nullptr) { // ensuring the item exists in the inventory
        delete inventory[i]; // delete the scroll object
        
        // Shift the remaining items in the inventory array
        for (int x = i; x < inventoryItems - 1; x++) {
            inventory[x] = inventory[x + 1];
        }
        inventory[inventoryItems - 1] = nullptr;
        inventoryItems--;
    }
}

//monster constructor
Monster::Monster(int specHitPoints, string specWeaponType, int specArmorPoints, int specStrengthPoints, int specDexterityPoints) : Actor( specHitPoints, specWeaponType, specArmorPoints, specStrengthPoints, specDexterityPoints) {
    oldType = EMPTY;
};

Monster::~Monster() {}

Coord Monster::move(Coord playerPosition, Level* level) {
    return getCurrentPos();
}

char Monster::getOldType() {
    return oldType;
}

void Monster::setOldType(char old) {
    oldType = old;
}

Bogeymen::Bogeymen() : Monster(randInt(5,10), "short sword", randInt(2, 3), randInt(2, 3), 2) {}
Bogeymen::~Bogeymen() {
}

Coord Bogeymen::move(Coord playerPosition, Level* level) {
    int changeInX = getCurrentPos().getX() - playerPosition.getX();
    int changeInY = getCurrentPos().getY() - playerPosition.getY();
    Coord newLocation = getCurrentPos();

    if (!withinNumber(playerPosition, getCurrentPos(), 5)) {
        return newLocation;
    }
    int x = getCurrentPos().getX();
    int y = getCurrentPos().getY();
    
    //player and bogeyman are in the same column
    if (changeInX == 0) {
        if (changeInY < 0) { // Player is below
            if (openSpot(level->getType(y+1, x))) {
                newLocation.setY(newLocation.getY() + 1);
            }
        }
        else { // Player is above or at the same level
            if (openSpot(level->getType(y-1, x))) {
                newLocation.setY(newLocation.getY() - 1);
            }
        }
        //p;ayer and bogeyman are in the same row
    } else if (changeInY == 0) {
        if (changeInX < 0) { // Player is to the right
            if (openSpot(level->getType(y, x+1))) {
                newLocation.setX(newLocation.getX() + 1);
            }
        }
        else { // Player is to the left or at the same level
            if (openSpot(level->getType(y, x-1))) {
                newLocation.setX(newLocation.getX() - 1);
            }
        }
        // not in the same row or column so no direct path
    } else {
        if (changeInX > 0) { // Bogeyman is to the right of the player
            if (changeInY > 0) { // Bogeyman is below the player
                if (openSpot(level->getType(y, x-1))) {
                    newLocation.setX(newLocation.getX() - 1);
                } else if (openSpot(level->getType(y-1, x))) {
                    newLocation.setY(newLocation.getY() - 1);
                }
            }
            else { // Bogeyman is above the player
                if (openSpot(level->getType(y, x-1))) {
                    newLocation.setX(newLocation.getX() - 1);
                } else if (openSpot(level->getType(y+1, x))) {
                    newLocation.setY(newLocation.getY() + 1);
                }
            }
        } else { // Bogeyman is to the left of the player
            if (changeInY > 0) { // Bogeyman is below the player or at the same level
                if (openSpot(level->getType(y, x+1))) {
                    newLocation.setX(newLocation.getX() + 1);
                } else if (openSpot(level->getType(y-1, x))) {
                    newLocation.setY(newLocation.getY() - 1);
                }
            }
            else { // Bogeyman is above the player
                if (openSpot(level->getType(y, x+1))) {
                    newLocation.setX(newLocation.getX() + 1);
                } else if (openSpot(level->getType(y+1, x))) {
                    newLocation.setY(newLocation.getY() + 1);
                }
            }
        }
    }

    if (openSpot(newLocation.getType())) {
        return newLocation;
    }
    return getCurrentPos();}


Snakewomen::Snakewomen() : Monster(randInt(3,6), "magic fangs", 2, 3, 3) {}
Snakewomen::~Snakewomen() {
}

Coord Snakewomen::move(Coord playerPosition, Level* level){
    int changeInX = getCurrentPos().getX() - playerPosition.getX();
    int changeInY = getCurrentPos().getY() - playerPosition.getY();
    Coord newLocation = getCurrentPos();

    if (!withinNumber(playerPosition, getCurrentPos(), 3)) {
        return newLocation;
    }
    int x = getCurrentPos().getX();
    int y = getCurrentPos().getY();
    
    //snakewomen and player are in the column
    if (changeInX == 0) {
        if (changeInY < 0) { // Player is below
            if (openSpot(level->getType(y+1, x))) {
                newLocation.setY(newLocation.getY() + 1);
            }
        }
        else { // Player is above or at the same level
            if (openSpot(level->getType(y-1, x))) {
                newLocation.setY(newLocation.getY() - 1);
            }
        }
        //same row
    } else if (changeInY == 0) {
        if (changeInX < 0) { // Player is to the right
            if (openSpot(level->getType(y, x+1))) {
                newLocation.setX(newLocation.getX() + 1);
            }
        }
        else { // Player is to the left or at the same level
            if (openSpot(level->getType(y, x-1))) {
                newLocation.setX(newLocation.getX() - 1);
            }
        }
        //cannot move in a direct path to player
    } else {
        if (changeInX > 0) { // Snakewoman is to the right of the player
            if (changeInY > 0) { // Snakewoman is below the player
                if (openSpot(level->getType(y, x-1))) {
                    newLocation.setX(newLocation.getX() - 1);
                } else if (openSpot(level->getType(y-1, x))) {
                    newLocation.setY(newLocation.getY() - 1);
                }
            }
            else { // Snakewoman is above the player
                if (openSpot(level->getType(y, x-1))) {
                    newLocation.setX(newLocation.getX() - 1);
                } else if (openSpot(level->getType(y+1, x))) {
                    newLocation.setY(newLocation.getY() + 1);
                }
            }
        } else { // Snakewoman is to the left of the player
            if (changeInY > 0) { // Snakewoman is below the player or at the same level
                if (openSpot(level->getType(y, x+1))) {
                    newLocation.setX(newLocation.getX() + 1);
                } else if (openSpot(level->getType(y-1, x))) {
                    newLocation.setY(newLocation.getY() - 1);
                }
            }
            else { // Snakewoman is above the player
                if (openSpot(level->getType(y, x+1))) {
                    newLocation.setX(newLocation.getX() + 1);
                } else if (openSpot(level->getType(y+1, x))) {
                    newLocation.setY(newLocation.getY() + 1);
                }
            }
        }
    }
    if (openSpot(newLocation.getType())) {
        return newLocation;
    }
    return getCurrentPos();
}

// dragon constructor
Dragons::Dragons() : Monster(randInt(20, 25), "long sword", 4, 4, 4) {}
Dragons::~Dragons() {
}

//does not move so only returns current position, but can get hit points back
Coord Dragons::move(Coord playerPosition, Level* level) {
    if (trueWithProbability(10) && getHitPoints() < 25) {
        setHitPoints(getHitPoints() + 1);
    }
    return getCurrentPos();
}

Goblins::Goblins(int sniff) : Monster(randInt(15, 20), "short sword", 3, 1, 1) {
    goblinSniff = sniff;
}
Goblins::~Goblins() {
}

Coord Goblins::move(Coord playerPosition, Level* level) {
    //creating a array of arrays of bools of whether or not a Coord location has been visited
    bool visited[18][70];
    for (int i = 0; i < 18; i++){
        for (int x = 0; x < 70; x++){
            visited[i][x] = true;
        }
    }
    
    int minSteps = goblinSniff + 1;
    // if a path is not found, stay in the same space
    if (withinNumber(playerPosition, getCurrentPos(), 1) || !level->bestPath(playerPosition, getCurrentPos(), visited, goblinSniff, 0, minSteps)) {
        return getCurrentPos();
    }

    int x = getCurrentPos().getX();
    int y = getCurrentPos().getY();
    Coord bestMove = getCurrentPos();
    vector<Coord> directions = {
        Coord(y, x + 1), // RIGHT
        Coord(y, x - 1), // LEFT
        Coord(y + 1, x), // DOWN
        Coord(y - 1, x)  // UP
    };
    //try each of the directions and whichever min steps is one less from that spot than the original spot that is the next ideal move
    for (Coord direction : directions) {
        int newMinSteps = minSteps;
        if (openSpot(level->getType(direction.getY(), direction.getX())) && level->bestPath(playerPosition, direction, visited, goblinSniff - 1, 0, newMinSteps) && newMinSteps == minSteps - 1) {
            bestMove = direction;
            break;
        }
    }

    return bestMove;
}

//check if the monster can make it to the player in a set number of steps
bool withinNumber(Coord position1, Coord position2, int number) {
    int x = position1.getX() - position2.getX();
    int y = position1.getY() - position2.getY();
    
    if (abs(x) + abs(y) <= number) {
        return true;
    }
    return false;
}

//if the actor can move to the next spot
bool openSpot(char character) {
    if (character != WALL && character != BOGEYMEN && character != SNAKEWOMEN && character != DRAGONS && character != DRAGONS) {
        return true;
    }
    return false;
}
