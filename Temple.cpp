#include "Temple.h"
#include "Actor.h"
#include "GameObjects.h"
#include "utilities.h"
#include <string>
#include <iostream>
using namespace std;

//Coord class stores the x and y coordinates on the level and the type of
Coord::Coord(int specY, int specX){
    y = specY;
    x = specX;
    type = EMPTY;
}

//type of Coord within the level
char Coord::getType() {
    return type;
}

//changing the Coord type
void Coord::setType(char specType) {
    type = specType;  
}

//get y coordinate
int Coord::getY(){
    return y;
}

//set y coordinate
void Coord::setY(int specY){
    y = specY;
}

// get x coordinate
int Coord::getX(){
    return x;
}

//set x coordinate
void Coord::setX(int specX){
    x = specX;
}

//constructor for a level: initializes level to all wall, creates killedMonsters vector, creates droppedWeapons vector, sets the goblin sniff distance and sets the player's old type to Empty
Level::Level(int goblinSniffDistance) {
    for (int y = 0; y < 18; y++) {
        for (int x = 0; x < 70; x++) {
            level[y][x] = new Coord(y,x);
            level[y][x]->setType(WALL);
        }
    }
    killedMonsters = new vector<Coord>;
    droppedWeapons = new vector<string>;
    goblinSniff = goblinSniffDistance;
    oldType = EMPTY;
}

//desctructor for the level object
Level::~Level() {
    // Deallocate memory for each Coord object in the level array
    for (int y = 0; y < 18; y++) {
        for (int x = 0; x < 70; x++) {
            delete level[y][x];
        }
    }

    // Deallocate memory for each Monster object in the monsters vector
    for (Monster* monster : monsters) {
        delete monster;
    }

    // Deallocate memory for the killedMonsters and droppedWeapons vectors
    delete killedMonsters;
    delete droppedWeapons;
}

//cleans up old level when new one is created by reseting vectors and level Coords
void Level::cleanUpLevel() {
    // Set all tiles to WALL
    for (int y = 0; y < 18; y++) {
        for (int x = 0; x < 70; x++) {
            level[y][x]->setType(WALL);
        }
    }

    // Delete all monsters and clear the container
    for (Monster* monster : monsters) {
        delete monster;
    }
    monsters.clear(); // Clear the container after deletion

    // Delete other dynamic containers and set pointers to null
    if (killedMonsters) {
        delete killedMonsters;
        killedMonsters = new vector<Coord>;
    }

    if (droppedWeapons) {
        delete droppedWeapons;
        droppedWeapons = new vector<string>;
    }
}

//generates the level
void Level::generateLevel(int levelNumber){
    setOldType(EMPTY);
    vector<Coord> roomCenters;
    //creates random number of rooms with random starting Coords and random lengths/widths
    int numberOfRooms = randInt(4, 6);
    for (int i = 0; i < numberOfRooms; i++) {
        int width, length, startLength, startWidth;
        do {
            length = randInt(3, 8);
            width = randInt(9, 15);
            startWidth = randInt(1, 69 - width);
            startLength = randInt(1, 17 - length);
            //when the area of the room is not already occupied by another room, sets that to empty and officially makes a new room
        } while (areaIsTaken(width, startWidth, length, startLength));
            setAreaToEmpty(width, startWidth, length, startLength);
        
        //find the centers of the rooms
            int centerX = startWidth + width / 2;
            int centerY = startLength + length / 2;
            roomCenters.push_back(Coord(centerY, centerX));
    }

    //connects the rooms with hallways between the room centers
    connectRooms(roomCenters);
    
        
    //ensures that the outer border of the temple is walls
    for (int i = 0; i < 70; i++) {
        level[0][i]->setType(WALL);
        level[17][i]->setType(WALL);
    }
        for (int i = 0; i < 18; i++) {
        level[i][0]->setType(WALL);
        level[i][69]->setType(WALL);
        }
    
    //adding weapons, scrolls, and idol
    int numberOfItems = randInt(2, 3);
    while (numberOfItems > 0) {
        int ScrollOrWeapon = randInt(2);
        if (ScrollOrWeapon == 1) {
            changeCoordType(findRandomEmpty(), WEAPON);
        }
        else {
            changeCoordType(findRandomEmpty(), SCROLL);
        }
        numberOfItems--;
    }
        
    if (levelNumber == 4) {
        changeCoordType(findRandomEmpty(), IDOL);
    }
    else {
        changeCoordType(findRandomEmpty(), STAIRWAY);
    }
            
        
    //create a random number of monsters and randomly assign which one depending on the level
    int numberOfMonsters = randInt(2, 5*(levelNumber+1)+1);
    if (levelNumber < 1) {
        while (numberOfMonsters > 0) {
            int whichMonster = randInt(2);
            if (whichMonster == 1) {
                Coord location = findRandomEmpty();
                Snakewomen* snakewoman = new Snakewomen();
                snakewoman->setCurrentPos(location.getY(), location.getX());
                addMonster(snakewoman);
                changeCoordType(location, SNAKEWOMEN);  }
            else {
                Coord location = findRandomEmpty();
                Goblins* goblin = new Goblins(goblinSniff);
                goblin->setCurrentPos(location.getY(), location.getX());
                addMonster(goblin);
                changeCoordType(location, GOBLINS);            }
                numberOfMonsters--;
        }
    }
    else if (levelNumber == 1) {
        while (numberOfMonsters > 0) {
            int whichMonster = randInt(3);
            if (whichMonster == 1) {
                Coord location = findRandomEmpty();
                Bogeymen* bogeyman = new Bogeymen();
                bogeyman->setCurrentPos(location.getY(), location.getX());
                addMonster(bogeyman);
                changeCoordType(location, BOGEYMEN);
            }
            else if (whichMonster == 2){
                Coord location = findRandomEmpty();
                Goblins* goblin = new Goblins(goblinSniff);
                goblin->setCurrentPos(location.getY(), location.getX());
                addMonster(goblin);
                changeCoordType(location, GOBLINS);
            }
            else {
                Coord location = findRandomEmpty();
                Snakewomen* snakewoman = new Snakewomen();
                snakewoman->setCurrentPos(location.getY(), location.getX());
                addMonster(snakewoman);
                changeCoordType(location, SNAKEWOMEN);
            }
            numberOfMonsters--;
        }
    }
    else {
        while (numberOfMonsters > 0) {
            int whichMonster = randInt(4);
                if (whichMonster == 1) {
                    Coord location = findRandomEmpty();
                    Bogeymen* bogeyman = new Bogeymen();
                    bogeyman->setCurrentPos(location.getY(), location.getX());
                    addMonster(bogeyman);
                    changeCoordType(location, BOGEYMEN);
                }
                else if (whichMonster == 2){
                    Coord location = findRandomEmpty();
                    Goblins* goblin = new Goblins(goblinSniff);
                    goblin->setCurrentPos(location.getY(), location.getX());
                        addMonster(goblin);
                    changeCoordType(location, GOBLINS);
                }
                else if (whichMonster == 3){
                    Coord location = findRandomEmpty();
                    Dragons* dragon = new Dragons();
                    dragon->setCurrentPos(location.getY(), location.getX());
                    addMonster(dragon);
                    changeCoordType(location, DRAGONS);
                }
                else {
                    Coord location = findRandomEmpty();
                    Snakewomen* snakewoman = new Snakewomen();
                    snakewoman->setCurrentPos(location.getY(), location.getX());
                    addMonster(snakewoman);
                    changeCoordType(location, SNAKEWOMEN);
                }
            numberOfMonsters--;
        }
    }
            
}

//check that the area for the room is not already empty because of another room or directly next to it by using a buffer

bool Level::areaIsTaken(int width, int startWidth, int length, int startLength) {
    int buffer = 1;
    for (int y = startLength - buffer; y < startLength + length + buffer; y++) {
        for (int x = startWidth - buffer; x < startWidth + width + buffer; x++) {
            if (level[y][x]->getType() == EMPTY) {
                return true;
            }
        }
    }
    return false;
}

//set the Area for the room to empty

void Level::setAreaToEmpty(int width, int startWidth, int length, int startLength) {
    for (int y = startLength; y < startLength + length; ++y) {
        for (int x = startWidth; x < startWidth + width; ++x) {
            level[y][x]->setType(EMPTY);
        }
    }
}

//connect the rooms with hallways that either go up/ down first or horisontally
void Level::connectRooms(vector<Coord> roomCenters) {
    for (int i = 1; i < roomCenters.size(); ++i) {
        int x1 = roomCenters[i-1].getX();
        int y1 = roomCenters[i-1].getY();
        int x2 = roomCenters[i].getX();
        int y2 = roomCenters[i].getY();

        if (trueWithProbability(.5)) { // 50% chance to do horizontal first
            for (int x = min(x1, x2); x <= max(x1, x2); x++) {
                level[y1][x]->setType(EMPTY);
            }
            for (int y = min(y1, y2); y <= max(y1, y2); y++) {
                level[y][x2]->setType(EMPTY);
            }
        }
        else { // 50% chance to do vertical first
            for (int y = min(y1, y2); y <= max(y1, y2); y++) {
                level[y][x1]->setType(EMPTY);
            }
            for (int x = min(x1, x2); x <= max(x1, x2); x++) {
                level[y2][x]->setType(EMPTY);
            }
        }
    }
}


//set the player's old location to what they are standing on
void Level::setOldType(char specType){
    oldType = specType;
}

//get the player's old position type
char Level::getOldType(){
    return oldType;
}

//find the type of a level Coord with the x and y locations
char Level::getType(int specY, int specX) {
    return (level[specY][specX]->getType());
}

//find the type of a level Coord with the Coord
char Level::getType(Coord position) {
    return (level[position.getY()][position.getX()]->getType());
}

//find the type of monster at location
Monster* Level::whichMonster(int positionY, int positionX) {
    for (int i = 0; i < monsters.size(); i++) {
        if (monsters[i]->getCurrentPos().getY() == positionY && monsters[i]->getCurrentPos().getX() == positionX) {
            return monsters[i];
        }
    }
    return monsters[0];
}

//changing the type of a coord to specific type
void Level::changeCoordType(Coord change, char specType) {
    level[change.getY()][change.getX()]->setType(specType);
}

//find a random spot that has no item on it or isn't occupied by a actor or wall
Coord Level::findRandomEmpty() {
    int specY, specX;
    do {
        specY = randInt(18);
        specX = randInt(70);
    } while (level[specY][specX]->getType() != EMPTY);

    return Coord(specY, specX);
}

//prints out the level
void Level::printLevel() {
    for (int l = 0; l < 18; l++) {
        for (int w = 0; w < 70; w ++) {
            cout << level[l][w]->getType();
        }
        cout<< endl;
    }
}

//add a monster to the vector of the monsters on the level
void Level::addMonster(Monster* monster) {
    monsters.push_back(monster);
}

//destroy the monster object
void Level::destroyMonster(Monster* monster) {
    for (auto it = monsters.begin(); it != monsters.end(); ) {
        if (*it == monster) {
            it = monsters.erase(it);
            delete monster;
            return;
        } else {
            ++it;
        }
    }
}

//add a monster to the vector of monsters that have been killed
void Level::addKilledMonster(Coord newMonster) {
    killedMonsters->push_back(newMonster);
}

//remove a monster from the killed monster vector which contains the spots where monsters have died
void Level::removeKilledMonster(Coord monster) {
    for (auto it = killedMonsters->begin(); it != killedMonsters->end(); ++it) {
        if (it->getX() == monster.getX() && it->getY() == monster.getY()) {
            it = killedMonsters->erase(it);
            return;
        }
    }
}

//return the vector of the killed monsters on the level
vector<Coord>* Level::getKilledMonsters() {
    return killedMonsters;
}

//if a monster has dropped a weapon it is died, it records that spot
void Level::addDroppedWeapon (string weaponType) {
    droppedWeapons->push_back(weaponType);
}

//if the dropped object has been picked up removes it from the vector
void Level::removeDroppedWeapon(string weapon) {
    for (auto it = droppedWeapons->begin(); it != droppedWeapons->end(); ++it) {
        if (*it == weapon) {
            it = droppedWeapons->erase(it);
            return;
        }
    }
}

//return the vector of dropped weapons
vector<string>* Level::getDroppedWeapons() {
    return droppedWeapons;
}

//move function for the monsters that takes in the player object as a parameter
void Level::moveMonsters(Player& myPlayer) {
    // moves each of the monsters in the vector of monsters on the floor
    for (int i = 0; i< monsters.size(); i++) {
        // records what type of monster is being moved for the couts
        string monster;
        char character = getType(monsters[i]->getCurrentPos());
        if (character == BOGEYMEN) {monster = "Bogeyman";}
        else if (character == GOBLINS) { monster = "Goblin";}
        else if (character == SNAKEWOMEN) {monster = "Snakewoman";}
        else {monster = "Dragon";}
        //calls that specific monster's location setting the player's position and level object as parameters
        Coord newLocation = monsters[i]->move(myPlayer.getCurrentPos(), this);
        //if the player and monster are one spot away from each other
        if (withinNumber(myPlayer.getCurrentPos(), monsters[i]->getCurrentPos(), 1)) {
            //calls the attack function to see if they hit or miss the player
            if (monsters[i]->getWeapon().weaponAttack(monsters[i]->getDexterityPoints(), myPlayer.getDexterityPoints(), myPlayer.getArmorPoints())) {
                int damage = monsters[i]->getWeapon().damageAmount(monsters[i]->getStrengthPoints());
                myPlayer.setHitPoints(myPlayer.getHitPoints()-damage);
                
                // if the player's hit points are 0 or below, they have lost, so prints message
                if (myPlayer.getHitPoints() <= 0) {
                    cout << "the " << monster << " " << monsters[i]->getWeapon().getActionString() << "Player dealing a final blow." << endl;
                }
                
                //otherwise checks if the player is put to sleep or just hit
                else {
                    if (monster == "Snakewoman" && trueWithProbability(.2)) {
                            cout << "the Snakewoman " << monsters[i]->getWeapon().getActionString() << "Player and hits, putting the Player to sleep." << endl;
                            myPlayer.setSleepTime(myPlayer.getSleepTime() + randInt(2,6));
                    }
                    else {
                        cout << "the " << monster << " " << monsters[i]->getWeapon().getActionString() << "Player and hits." << endl;
                    }
                }
            }
            //monster does not hit the player
            else {
                cout << "the " << monster << " " << monsters[i]->getWeapon().getActionString() << "Player and misses." << endl;
            }
        }
        //monster is moving into an open spot so the old type fills the old spot and is then adjusted
        else if (openSpot(newLocation.getType())){
            changeCoordType(monsters[i]->getCurrentPos(), monsters[i]->getOldType());
            monsters[i]->setOldType(getType(newLocation));
            changeCoordType(newLocation, character);
            monsters[i]->setCurrentPos(newLocation.getY(), newLocation.getX());
        }
    }
}

//Goblin recurisve function which finds the shortest path to the player if one exits and returns false if it doesnt have path in range

bool Level::bestPath(Coord playerPosition, Coord monsterPosition, bool visited[18][70], int maxSteps, int currentSteps, int &minSteps) {
    
    //player is out of range
    if (currentSteps > maxSteps) {
        return false;
    }

    int x = monsterPosition.getX();
    int y = monsterPosition.getY();
    
    // if player is reached

    if (y == playerPosition.getY() && x == playerPosition.getX()) {
        minSteps = min(minSteps, currentSteps);
        return true;
    }

    bool foundPath = false;
    vector<Coord> directions = {
        Coord(y, x + 1), // RIGHT
        Coord(y, x - 1), // LEFT
        Coord(y + 1, x), // DOWN
        Coord(y - 1, x)  // UP
    };

    //checking all four directions
    for (Coord direction : directions) {
        int newX = direction.getX();
        int newY = direction.getY();

        if (openSpot(level[newY][newX]->getType()) && visited[newY][newX]) {
            visited[newY][newX] = false;
            if (bestPath(playerPosition, direction, visited, maxSteps, currentSteps + 1, minSteps)) {
                foundPath = true;
            }
            visited[newY][newX] = true;
        }
    }

    return foundPath;
}
