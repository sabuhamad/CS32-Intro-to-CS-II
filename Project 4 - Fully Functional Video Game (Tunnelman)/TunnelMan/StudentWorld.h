#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    void initializeEarth();
    void initializeBoulders();
    void generateGoodie();
    ~StudentWorld();
    void generateRandomCoordinates(int&x, int&y); //generates random coordinates for the oil and gold to be spawned
    virtual int move();
    void createProtester();
    bool moveIsValid(int x, int y);
    bool newMoveOverlap(int x, int y, Actor* b);
    bool overlapEarth(int x, int y, Actor::Direction d);
    void boulderEarthOverlap(int x, int y); //removes earth around the boulder for initializing
    bool boulderShouldFall(int x, int y); //determines if there is earth under the boulder
    void sonarUsed(int x, int y, int radius);
    void foundOil();
    void display();
    bool goodieOverlap(int x, int y, Actor* b);
    bool goodieOverlap(int x, int y);
    Tunnelman* getPlayer();
    Protester* protesterInRadius(Actor * actor, int radius);
    bool isThereBoulder(int x, int y);
    bool isThereEarth(int x, int y); //checks if there is a piece of active earth assuming a 4x4 object, given the coordinates provided
    bool checkIfOutOfBoundsOrEarth(int x, int y);
    bool withinRadius(int x1, int y1, int x2, int y2, int radius);
    void decProtester();
    GraphObject::Direction leaveGame(Protester * pr);
    bool ableToMove(int x, int y, GraphObject::Direction direction);
    bool isPlayerInRadius(Actor *actor, int radius);
    virtual void cleanUp();
    GraphObject::Direction hardcoreProtesterPath(Protester * pr, int M);
    void newObject(Actor* a);
private:
    Tunnelman* player; //keeps track of player object
    bool m_isFirstTick; //checks if it is the first tick of playing
    Earth* v[64][64]; //keeps track of earth objects
    int protesterPath[64][64]; //creates a path for the protester through a search
    std::vector<Actor*> objects; //holds all the actors except tunnelman and earth
    int ticks = 0; //counts the ticks
    int m_tickSinceLast; //tracks ticks since the last protester was added
    int numOil, numGold, numBoulders, maxProtesters, probHardcore, probGoodie, currProtesters; //stores number of different variables
    struct Coordinates //creates a structure to hold both an x and y component in order to be able to calculate path for protester
    {
            int x;
            int y;
            Coordinates(int a, int b): x(a),y(b){}
    };
};



#endif // STUDENTWORLD_H_
