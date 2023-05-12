#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject { // abstract base class
    
    StudentWorld* m_world;
    bool alive;
    public:

        Actor(StudentWorld* world, int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth)
            : GraphObject(imageID, startX, startY, startDirection, size, depth), m_world(world), alive(true)
            {
        
            }
        
        virtual void doSomething() = 0;
        
        virtual ~Actor()
        {
            die();
        }
        bool isAlive(){return alive;}
        void die(){alive = false;}
        
        StudentWorld* getWorld() const
        {
            return m_world;
        }
    
};

class Tunnelman : public Actor {
    
    int hit_pts;
    int water;
    int sonar;
    int nuggets;
    
public:

    Tunnelman(StudentWorld* world, int imageID = TID_PLAYER, int startX = 30, int startY = 60, Direction startDirection = right, float size = 1.0, unsigned int depth = 0)
        : Actor(world, imageID, startX, startY, startDirection, size, depth), hit_pts(10), water(5), sonar(1), nuggets(0) //construct a tunnelman with starting hp of 10, water of 5, sonar of 1, and nuggets of 0
    {
        setVisible(true);
    }
    void isAnnoyed(int hP); //removes HP from tunnelman and checks if he is dead
    virtual ~Tunnelman() //destructor
    {
        setVisible(false);
    }
    void ableToMove(int ch, int dest_x, int dest_y); //checks if the tunnelman is facing the direction seeking to move in, if not changes direction to that direction
    virtual void doSomething(); //gets the users key input every tick
    void add(int ID); //adds values of specific object type to tunnelman inventory
    void shoot(); //shoots a spurt if space was pressed
    void decHitPts(int decrement) //removes hitpoints from tunnelman corresponding to the number passed to the function
    {
        hit_pts -= decrement;
    }

    int getHitPts(){ return hit_pts;}
    int getWater(){return water;}
    int getSonar(){return sonar;}
    int getNuggets(){return nuggets;}
};

class Earth : public Actor {
    public:
        Earth(StudentWorld* world, int startX, int startY, int imageID = TID_EARTH, Direction startDirection = right, float size = .25, unsigned int depth = 3)
            : Actor(world, imageID, startX, startY, startDirection, size, depth) {
                setVisible(true);
            }
        ~Earth()
        {
            setVisible(false);
        }
        virtual void doSomething()
        {}
};


class GoldNugget : public Actor {
    bool tunnelPickup, protestPickup, placed;
    public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool pickupableByT, bool pickupableByP, int imageID = TID_GOLD, Direction startDirection = right, float size = 1, unsigned int depth = 2)
        : Actor(world, imageID, startX, startY, startDirection, size, depth), tunnelPickup(pickupableByT), protestPickup(pickupableByP) {
            if(protestPickup)
            {
                setVisible(true);
                placed = false;
                currTicks = 0;
            }
            else
                setVisible(false);
        }
    virtual void doSomething();
    void disappearIn(int ticks);
    virtual ~GoldNugget()
    {
        tunnelPickup = false;
        protestPickup = false;
        setVisible(false);
    }
    private:
    int currTicks;
};

class OilBarrel : public Actor {
    bool pickupable, alive;
    public:
    OilBarrel(StudentWorld* world, int startX, int startY, int imageID = TID_BARREL, Direction startDirection = right, float size = 1, unsigned int depth = 2)
        : Actor(world, imageID, startX, startY, startDirection, size, depth), pickupable(true) {
            setVisible(false);
        }
    virtual void doSomething();
    virtual bool isAlive()
        {
            return pickupable;
        }
    
    virtual ~OilBarrel()
    {
        pickupable = false;
        setVisible(false);
        alive = false;
    }
    private:
};

class Boulder : public Actor {
    public:
    Boulder(StudentWorld* world, int startX, int startY, int imageID = TID_BOULDER, Direction startDirection = down, float size = 1, unsigned int depth = 1)
    : Actor(world, imageID, startX, startY, startDirection, size, depth), stable(true), ticks(0), waiting(false), falling(false) {
            setVisible(true);
        }
    virtual void doSomething();
    virtual ~Boulder()
    {
        setVisible(false);
        stable = true;
        waiting = false;
        falling = false;
        ticks = 0;
    }
    bool needsToFall();
    private:
    bool stable;
    bool waiting;
    bool falling;
    int ticks;
};
class Goodie : public Actor
{
    int currTicks;
    public:
    virtual void doSomething();
    Goodie(StudentWorld* world, int imageID, int startX, int startY, Direction startDirection = right, float size = 1, unsigned int depth = 2) : Actor(world, imageID, startX, startY, startDirection, size, depth), currTicks(0)
    {
        setVisible(true);
        
    }
    void disappearIn(int ticks);
    virtual ~Goodie(){}
};

class SonarKit : public Goodie {
    bool alive;
    public:
    SonarKit(StudentWorld* world, int startX, int startY, int imageID = TID_SONAR, Direction startDirection = right, float size = 1, unsigned int depth = 2)
        : Goodie(world, imageID, startX, startY, startDirection, size, depth) {
            setVisible(true);
        }
    virtual ~SonarKit()
    {
        setVisible(false);
    }
    private:
};

class WaterPool : public Goodie {
    bool alive;
    public:
    WaterPool(StudentWorld* world, int startX, int startY, int imageID = TID_WATER_POOL, Direction startDirection = right, float size = 1, unsigned int depth = 2)
        : Goodie(world, imageID, startX, startY, startDirection, size, depth) {
            setVisible(true);
        }
    virtual ~WaterPool()
    {
        setVisible(false);
    }
    private:
};

class Protester : public Actor{
public:
    Protester(StudentWorld* world, int imageID, int hP, int startX, int startY, Direction startDirection, int size, int depth);
    int getHP();
    void decHP(int points);
    virtual void doSomething();
    bool isStunned(){return stunned;} //checks if protester is stunned
    Direction directionToPlayer();
    Direction randomDirection();
    bool directPath(Direction direction);
    bool atIntersection();
    void turn();
    virtual void bribe();
    void moveInDirection(Direction direction);
    void randomizeNumToMove();
    void isAnnoyed(int hP);
    bool isLeaving(){return leave;}
    void getStunned();
    bool isFacingPlayer();
    virtual ~Protester()
    {
        die();
        setVisible(false);
    }

private:
    bool stunned;
    int hitpoints;
    bool leave;
    int numToMove;
    int tickSinceLastTurn;
    int tickNoYell;
    int tickRest;
};
class RegularProtester : public Protester { //type of protester
public:
    RegularProtester(StudentWorld* world);
    virtual ~RegularProtester(){}
};

class HardcoreProtester : public Protester { //type of protester
public:
    HardcoreProtester(StudentWorld* world);
    virtual ~HardcoreProtester(){}
};


class Squirt : public Actor //class for water spurt that the tunnelman shoots
{
    bool alive;
    public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDirection, int imageID = TID_WATER_SPURT, float size = 1, unsigned int depth = 1)
        : Actor(world, imageID, startX, startY, startDirection, size, depth), distance(0) {
            setVisible(true);
        }
    virtual ~Squirt()
    {
        die();
    }
    virtual void doSomething();
    bool annoyProtester();
    private:
    int distance;
};






#endif // ACTOR_H_
