#include "GameObjects.h"
#include "utilities.h"

GameObjects::GameObjects(string type) {
    objectType = type;
}

GameObjects::~GameObjects(){
}

//sets the weapon's traits depending on what type it is initialized as
void GameObjects:: setObjectType(string type) {
    objectType = type;
    if (type == "mace") {
        actionString = "swings mace at the ";
        dexterityBonus = 0;
        damageAmount = 2;
    }
    if (type == "short sword") {
        actionString = "slashes short sword at the ";
        dexterityBonus = 0;
        damageAmount = 2;
    }
    if (type == "long sword") {
        actionString = "swings long sword at the ";
        dexterityBonus = 2;
        damageAmount = 4;
    }
    if (type == "magic axe") {
        actionString = "chops magic axe at the ";
        dexterityBonus = 5;
        damageAmount = 5;
    }
    if (type == "magic fangs") {
        actionString = "strikes magic fangs at the ";
        dexterityBonus = 3;
        damageAmount = 2;
    }
}

//returns the object type
string GameObjects:: getObjectType() {
    return objectType;
}

// weapon constructor
Weapons:: Weapons(string type) : GameObjects(type) {
    setObjectType(type);
}

Weapons::~Weapons(){}

// set the action string returned when the weapon is used
void GameObjects::setActionString(string specActionString){
    actionString = specActionString;
}

// gets the action string returned when the weapon is used
string GameObjects::getActionString(){
    return actionString;
}

void GameObjects::setDexterityBonus (int specDexterityBonus){
    dexterityBonus = specDexterityBonus;
}
int GameObjects::getDexterityBonus () {
    return dexterityBonus;
}
void GameObjects::setDamageAmount (int specDamageAmount){
    damageAmount = specDamageAmount;
}
int GameObjects::getDamageAmount (){
    return damageAmount;
}

//calculates whether the attacker is able to hit the defender taking the defender's armor and both's dexterities into account
bool Weapons::weaponAttack(int attackerDexterity, int defenderDexterity, int defenderArmorPoints) {
    int attackerPoints = attackerDexterity + getDexterityBonus();
    int defenderPoints = defenderDexterity + defenderArmorPoints;
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)) {
        return true;
    }
    return false;
}

//determines how much damage each weapon does depending on who uses it
int Weapons::damageAmount(int attackerStrength) {
    int damagePoints = randInt(0, attackerStrength + getDamageAmount() - 1);
    return damagePoints;
}


//Scroll constructor
Scrolls:: Scrolls(string type) : GameObjects(type) { }
Scrolls::~Scrolls(){}

