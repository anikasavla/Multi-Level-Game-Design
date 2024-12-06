#ifndef GameObjects_hpp
#define GameObjects_hpp

#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

class GameObjects {
public:
    GameObjects(string type);
    virtual ~GameObjects();
    void setObjectType(string specType);
    string getObjectType();
    void setActionString(string specActionString);
    string getActionString();
    void setDexterityBonus (int specDexterityBonus);
    int getDexterityBonus ();
    void setDamageAmount (int specDamageAmount);
    int getDamageAmount ();
private:
    string objectType, actionString;
    int dexterityBonus, damageAmount;
};

class Weapons: public GameObjects {
public:
    Weapons(string type);
    ~Weapons();
    bool weaponAttack(int attackerDexterity, int defenderDexterity, int defenderArmorPoints);
    int damageAmount(int attackerStrength);
};

class Scrolls: public GameObjects {
public:
    Scrolls(string type);
    ~Scrolls();
};


#endif /* GameObjects_hpp */

