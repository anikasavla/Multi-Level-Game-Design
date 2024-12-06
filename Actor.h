#ifndef Actor_hpp
#define Actor_hpp

#include <stdio.h>
#include "GameObjects.h"
#include "Temple.h"
#include <string>
#include <iostream>
using namespace std;

class Actor {
private:
    Coord currentPos;
    Weapons weapon;
    int hitPoints, armorPoints, strengthPoints, dexterityPoints, sleepTime;
    
public:
    Actor(int specHitPoints, string specWeaponType, int specArmorPoints, int specStrengthPoints, int specDexterityPoints);
    virtual ~Actor();
    Coord getCurrentPos();
    void setCurrentPos(int specY, int specX);
    int getHitPoints();
    virtual void setHitPoints(int specHitPoints);
    int getArmorPoints();
    void setArmorPoints(int specArmorPoints);
    int getStrengthPoints();
    void setStrengthPoints(int specStrengthPoints);
    int getDexterityPoints();
    void setDexterityPoints(int specDexterityPoints);
    void setWeapon(Weapons specWeapon);
    Weapons getWeapon();
    void setSleepTime(int specSleepTime);
    int getSleepTime();
};

class Player: public Actor {
private:
    GameObjects* inventory[25];
    char alphabet[25] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y'};
    int inventoryItems;
    int level;
    int maxHitPoints;
public:
    Player();
    ~Player();
    void setMaxHitPoints(int specMaxHitPoints);
    int getMaxHitPoints();
    void setHitPoints(int specHitPoints);
    int getLevel();
    void setLevel(int newLevel);
    void displayStats();
    bool isWeapon(char character, Weapons &whichWeapon);
    bool isScroll(char character, Scrolls &whichScroll);
    void displayInventory();
    void addWeaponToInventory(Weapons item);
    void addScrollToInventory(Scrolls item);
    void removeScrollFromInventory(char selection);
};


class Monster: public Actor {
private:
    char oldType;
public:
    Monster(int specHitPoints, string specWeaponType, int specArmorPoints, int specStrengthPoints, int specDexterityPoints);
    virtual ~Monster(); 
    virtual Coord move(Coord playerPosition, Level* level);
    char getOldType();
    void setOldType(char old);
};

class Bogeymen: public Monster {
public:
    Bogeymen();
    ~Bogeymen();
    Coord move(Coord playerPosition, Level* level);
};

class Snakewomen: public Monster {
public:
    Snakewomen();
    ~Snakewomen();
    Coord move(Coord playerPosition, Level* level);
};
    
class Dragons: public Monster {
public:
    Dragons();
    ~Dragons();
    Coord move(Coord playerPosition, Level* level);
};

class Goblins: public Monster {
private:
    int goblinSniff;
public:
    Goblins(int sniff);
    ~Goblins();
    Coord move(Coord playerPosition, Level* level);
};

bool withinNumber (Coord position1, Coord position2, int number);

bool openSpot(char character);

#endif /* Actor_hpp */
