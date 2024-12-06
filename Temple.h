#ifndef Temple_hpp
#define Temple_hpp

#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

const char BOGEYMEN = 'B';
const char DRAGONS = 'D';
const char GOBLINS = 'G';
const char SNAKEWOMEN = 'S';
const char STAIRWAY = '>';
const char EMPTY = ' ';
const char IDOL = '&';
const char WEAPON = ')';
const char WALL = '#';
const char SCROLL = '?';
const char PLAYER = '@';

class Coord {
private:
    char type;
    int y, x;
public:
    Coord(int specY, int specX);
    char getType();
    void setType(char specType);
    int getY();
    void setY(int specY);
    int getX();
    void setX(int specX);
    

};

class GameObjects;
class Monster;
class Player;
class Weapons;
struct Room;

class Level {
private:
    int goblinSniff;
    Coord *level[18][70];
    char oldType;
    vector<Monster*> monsters;
    vector<Coord>* killedMonsters;
    vector<string>* droppedWeapons;
public:
    Level(int goblinSniffDistance);
    ~Level();
    void cleanUpLevel();
    void generateLevel(int levelNumber);
    bool areaIsTaken(int width, int startWidth, int length, int startLength);
    void setAreaToEmpty(int width, int startWidth, int length, int startLength);
    void connectRooms(vector<Coord> roomCenters);
    void changeCoordType(Coord change, char specType);
    void setOldType(char type);
    char getOldType();
    char getType(int specY, int specX);
    Monster* whichMonster(int positionY, int positionX);
    char getType(Coord position);
    Coord findRandomEmpty();
    void printLevel();
    void addMonster(Monster* monster);
    void destroyMonster(Monster* monster);
    void addKilledMonster(Coord newMonster);
    void removeKilledMonster(Coord monster);
    vector<Coord>* getKilledMonsters();
    void addDroppedWeapon (string weaponType);
    void removeDroppedWeapon(string weapon);
    vector<string>* getDroppedWeapons();
    void moveMonsters(Player& myPlayer);
    bool bestPath(Coord playerPosition, Coord monsterPosition, bool visited[18][70], int maxSteps, int currentSteps, int &minSteps);
};

#endif /* Temple_hpp */
