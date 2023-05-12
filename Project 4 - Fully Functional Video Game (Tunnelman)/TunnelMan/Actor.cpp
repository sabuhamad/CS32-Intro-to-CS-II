#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Tunnelman::isAnnoyed(int hP) //removes HP from tunnelman and checks if he is dead
{
    decHitPts(hP);
    if (getHitPts() <= 0) {
        die();
    }
}

void Tunnelman::add(int id) //adds values of specific object type to tunnelman inventory
{
    if (id == TID_GOLD)
        nuggets++;
    if (id == TID_WATER_POOL)
        water+=5;
    if (id == TID_SONAR)
        sonar++;
}

void Tunnelman::ableToMove(int ch, int finalX, int finalY) { //checks if the tunnelman is facing the direction seeking to move in, if not changes direction to that direction

    bool correctDirection = false;

    if (ch == KEY_PRESS_LEFT) {
        if (!(getDirection() == left)) {
            setDirection(left);
        } else
        {
            correctDirection = true;
        }
    }
    else if (ch == KEY_PRESS_UP) {
        if (!(getDirection() == up)) {
            setDirection(up);
        }
        else
            correctDirection = true;
    }
    else if (ch == KEY_PRESS_DOWN) {
        if (!(getDirection() == down)) {
            setDirection(down);
        }
        else
            correctDirection = true;
    }
    else if (ch == KEY_PRESS_RIGHT) {
        if (!(getDirection() == right)) {
            setDirection(right);
        }
        else
            correctDirection = true;
    }

    if (correctDirection && getWorld()->moveIsValid(finalX, finalY))
    {
        moveTo(finalX, finalY);
        if(getWorld()->overlapEarth(getX(), getY(), getDirection()))
            getWorld()->playSound(SOUND_DIG);
    }
}

void Tunnelman::doSomething() { //gets the users key input every tick
    if (isAlive() == false) {
        return;
    } else {
        int ch;
        if (getWorld()->getKey(ch) == true)
        {
            switch (ch)
            {
            case KEY_PRESS_LEFT:
                ableToMove(ch, getX() - 1, getY());
                break;
            case KEY_PRESS_RIGHT:
                ableToMove(ch, getX() + 1, getY());
                break;
            case KEY_PRESS_UP:
                ableToMove(ch, getX(), getY() + 1);
                break;
            case KEY_PRESS_DOWN:
                ableToMove(ch, getX(), getY() - 1);
                break;
            case KEY_PRESS_ESCAPE:
                getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
                die();
                break;
            case KEY_PRESS_SPACE:
                if (water > 0) {
                    water--;
                    shoot();
                }
			break;
                break;
            case KEY_PRESS_TAB:
                if (nuggets > 0) {
				getWorld()->newObject(new GoldNugget(getWorld(), getX(), getY(), false, true));
				nuggets--;
                }
			break;
                break;
            case 'z':
            case 'Z':
                if (sonar > 0)
                {
                    sonar--;
                    getWorld()->sonarUsed(getX(), getY(), 12);
                    getWorld()->playSound(SOUND_SONAR);
                }
			break;
                break;
            default:
                break;
            }
        }
    }
}

bool Boulder::needsToFall() //checks if the boulder should continue to fall or disappear
{
    Protester * p= getWorld()->protesterInRadius(this,3);
    if (p != nullptr && !p->isStunned()) {
        p->isAnnoyed(21);
        return true;
    }
    if(getWorld()->isPlayerInRadius(this, 3))
    {
        getWorld()->getPlayer()->die();
        return false;
    }
    return (getWorld()->boulderShouldFall(getX(), getY()));
}
void Tunnelman::shoot() //shoots a spurt if space was pressed
{
    switch (getDirection())
        {
        case left:
            if(!getWorld()->checkIfOutOfBoundsOrEarth(getX()-4,getY())&& !getWorld()->isThereBoulder(getX()-4, getY()))
                getWorld()->newObject(new Squirt(getWorld(), getX() - 4, getY(), getDirection()));
            break;
        case right:
            if (!getWorld()->checkIfOutOfBoundsOrEarth(getX() + 4, getY()) && !getWorld()->isThereBoulder(getX()+4, getY()))
                getWorld()->newObject(new Squirt(getWorld(), getX() + 4, getY(), getDirection()));
            break;
        case down:
            if (!getWorld()->checkIfOutOfBoundsOrEarth(getX() , getY()-4) && !getWorld()->isThereBoulder(getX(), getY()-4))
                getWorld()->newObject(new Squirt(getWorld(), getX(), getY() - 4, getDirection()));
            break;
        case up:
            if (!getWorld()->checkIfOutOfBoundsOrEarth(getX(), getY() + 4) && !getWorld()->isThereBoulder(getX(), getY()+4 ))
            getWorld()->newObject(new Squirt(getWorld(), getX(), getY() + 4, getDirection()));
            break;
        case none:return;
        }
        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}
void Squirt::doSomething() { //checks if squirt comes into contact with a protester at every tick
    if (!isAlive()) return;
    if (annoyProtester() || distance == 4 ) {
        die();
        return;
    }
    
    switch (getDirection()) {
    case left:
        if (getWorld()->isThereEarth(getX() - 1, getY()) || !getWorld()->moveIsValid(getX() - 1, getY())) {
            die(); return;
        }
        else   moveTo(getX() - 1, getY());
        break;
    case right:
        if (getWorld()->isThereEarth(getX() + 1, getY()) || !getWorld()->moveIsValid(getX() + 1, getY())) {
           die(); return;
        }
        else   moveTo(getX() + 1, getY());
        break;
    case up:
        if (getWorld()->isThereEarth(getX(), getY() + 1) || !getWorld()->moveIsValid(getX(), getY() + 1)) {
            die(); return;
        }
        else moveTo(getX(), getY() + 1);
        break;
    case down:
        if (getWorld()->isThereEarth(getX(), getY() - 1) || !getWorld()->moveIsValid(getX(), getY() - 1)) {
            die(); return;
        }
        else moveTo(getX(), getY() - 1);
        break;
    case none : return;
    }
    distance++;
}

bool Squirt::annoyProtester() //decrement protester hp by 2if it is within radius of the water spurt
{
    Protester * p= getWorld()->protesterInRadius(this,3);
    if (p != nullptr && !p->isStunned()) {
        p->isAnnoyed(2);
        return true;
    }
    return false;

}

void Boulder:: doSomething() //checks boulder's state every tick
{
    if(!isAlive())
    {
        falling = false;
        stable = false;
        waiting = false;
        return;
    }
    if(stable && needsToFall())
    {
        stable = false;
        waiting = true;
    }
    else if(waiting)
    {
        if(ticks >= 30)
        {
            waiting = false;
            falling = true;
        }
        ticks++;
    }
    else if((falling && !needsToFall()) || getY() == 0)
    {
        stable = true;
        falling = false;
        die();
    }
    else if(falling)
    {
        moveTo(getX(), getY() - 1);
    }
}

Protester::Protester(StudentWorld* world, int imageID, int hP, int startX = 60, int startY = 60, Direction startDirection = left, int size = 1, int depth = 0) : Actor(world, imageID, startX, startY, startDirection, size, depth), tickSinceLastTurn(200), tickNoYell(15), stunned(false), leave(false), hitpoints(hP) //constructs protester
{
    randomizeNumToMove();
    tickRest = std::max(0, 3 - (int)getWorld()->getLevel() / 4);
}
void Protester::randomizeNumToMove() //sets the random num to move value
{
    numToMove = rand() % 53 + 8;
}
void Protester::doSomething() //updates the protester's actions every tick
{
    if (!isAlive()) //if the protester is dead, do not check the rest of the cases
        return;

    if (leave) { //if the protester is exiting, do not check the rest of the cases
        if (getX() == 60 && getX() == 60) {
            die();
            getWorld()->decProtester();
            return;
        }
        if (tickRest > 0) {
            tickRest--;
            return;
        }
        else
        {
            tickRest = std::max(0, 3 - (int)getWorld()->getLevel() / 4); //rest
        }
        Direction exit = getWorld()->leaveGame(this);
        if(exit != none){
            moveInDirection(exit);
            return;
        }
        return;
    }
    
    if (tickRest > 0) { //causes protester to be slower than player by resting a certain amount of ticks
        tickRest--;
        return;
    }
    else {
        tickRest = std::max(0, 3 - (int)getWorld()->getLevel() / 4);
        stunned = false;
        tickSinceLastTurn++;
        tickNoYell++;
    }

    if (getWorld()->isPlayerInRadius(this, 4) && isFacingPlayer()) { //yell if within range
        if (tickNoYell > 15) {
            getWorld()->getPlayer()->isAnnoyed(2);
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            tickNoYell = 0;
            return;
        }
        return;
    }
  
    if(getID()==TID_HARD_CORE_PROTESTER){   //hardcore protester tracks the player
        int M =16 + int(getWorld()->getLevel());
        Direction s= getWorld()->hardcoreProtesterPath(this, M);
        if(s != none){
            moveInDirection(s);
            return;
        }
    }
    
    Direction d = directionToPlayer(); //if the player is within line of sight, go towards them
    if (d != none && directPath(d) && (!getWorld()->isPlayerInRadius(this, 3))) {
        setDirection(d);
        moveInDirection(d);
        numToMove = 0;
        return;
    }
    
    numToMove--; //decrease the num to move and keep walking in a random direction if possible
    if (numToMove <= 0) {
        Direction k = none;
        k = randomDirection();
        while (true) {
            k = randomDirection();
            if (getWorld()->ableToMove(getX(),getY(), k)) break;
        }
        setDirection(k);
        randomizeNumToMove();
    }
   
    else if (atIntersection() && tickSinceLastTurn > 200) {  // check if protester is at an intersection and turn if it has been more than 200 ticks since the last one
        turn();
        tickSinceLastTurn = 0;
        randomizeNumToMove();
    }
    
    moveInDirection(getDirection()); //move
    if (!getWorld()->ableToMove(getX(),getY(),getDirection())) { //if you can't move in that direction set numToMove to 0 so a new one can be picked
        numToMove = 0;
    }
}

void Protester::moveInDirection(Direction direction) //moves the protester in a specified direction
{
    switch (direction) {
    case left:
        if (getDirection() == left) {
            if (getX() == 0) setDirection(right);
                moveTo(getX() - 1, getY());
        }
        else setDirection(left);
        break;
    case right:
        if (getDirection() == right) {
            if (getX() == 60) setDirection(left);
                moveTo(getX() + 1, getY());
        }
        else setDirection(right);
        break;
    case up:
        if (getDirection() == up) {
            if (getY() == 60) setDirection(down);
            moveTo(getX(), getY() + 1);
        }
        else setDirection(up);
        break;
    case down:
        if (getDirection() == down) {
            if (getY() == 0) setDirection(up);
            moveTo(getX(), getY() - 1);
        }
        else setDirection(down);
        break;
    case none: return;
    }
}



GraphObject:: Direction Protester::directionToPlayer() //finds the direction of the player if in line of sight
{
    int playerX = getWorld()->getPlayer()->getX();
    int playerY = getWorld()->getPlayer()->getY();
    if (getY() == playerY && getX() == playerX)
    {
        return getDirection();
    }
    if (getX() == playerX) {
        if (getY() < playerY)
            return up;
        if (getY() > playerY)
            return down;
    }
    if (getY() == playerY) {
        if (getX() > playerX)
            return left;
        if (getX() < playerX)
            return right;
    }
    
    return none;
}

bool Protester::directPath(Direction direction) //plots a path towards the player noting the line of sight
{
    int playerX = getWorld()->getPlayer()->getX();
    int playerY = getWorld()->getPlayer()->getY();
    switch (direction) {
    case left:
        for (int i = getX(); i >= playerX; i--) {
            if (getWorld()->isThereEarth(i, getY()) || (getWorld()->isThereBoulder(i, getY())))
                return false;
        }
        return true;
        break;
    case right:
        for (int i = getX(); i <= playerX; i++) {
            if (getWorld()->isThereEarth(i, getY()) || (getWorld()->isThereBoulder(i, getY())))
                return false;
        }
        return true;
        break;
    case up:
        for (int j = getY(); j <= playerY; j++) {
            if (getWorld()->isThereEarth(getX(), j) || (getWorld()->isThereBoulder(getX(), j)))
                return false;
        }
        return true;
        break;
    case down:
        for (int j = getY(); j >= playerY; j--) {
            if (getWorld()->isThereEarth(getX(), j) || (getWorld()->isThereBoulder(getX(), j)))
                return false;
        }
        return true;
        break;
    case none:
        return false;
    }
}
GraphObject::Direction Protester::randomDirection() //picks a random direction for the protester
{
    int r;
    r = rand() % 4;
    switch (r) {
        case 0: return left;
        case 1: return  right;
        case 2: return up;
        case 3: return down;
    }
    return none;
}
bool Protester::atIntersection() //determines logic for protester if they are at an intersection
{
    if (getDirection() == up || getDirection() == down) {
        return (getWorld()->ableToMove(getX(),getY(), left) || getWorld()->ableToMove(getX(),getY(), right));
    }
    else
        return (getWorld()->ableToMove(getX(),getY(), up) || getWorld()->ableToMove(getX(),getY(), down));
}
void Protester::turn() //pick a direction to turn that is possible to turn to
{
    if (getDirection() == up || getDirection() == down) {
        if (!getWorld()->ableToMove(getX(),getY(), left)) setDirection(right);
        else if (!getWorld()->ableToMove(getX(),getY(), right)) setDirection(left);
        else {
            switch (rand() % 2) {
            case 0: setDirection(left);
            case 1: setDirection(right);
            }
        }
    }
    else {
        if (!getWorld()->ableToMove(getX(),getY(), up)) setDirection(down);
        else if (!getWorld()->ableToMove(getX(),getY(), down)) setDirection(up);
        else {
            switch (rand() % 2) {
            case 0: setDirection(up);
            case 1: setDirection(down);
            }
        }
    }

}
void Protester::isAnnoyed(int hP) //reduces protester hP as necessary and checks if it was hit by a boulder or dead
{
    if(leave) return;
    decHP(hP);
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    getStunned();
    if (getHP() <= 0) {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        leave = true;
        tickRest=0;
        if(hP==21) getWorld()->increaseScore(500);
        else if(getID()==TID_PROTESTER) getWorld()->increaseScore(100);
        else getWorld()->increaseScore(250);
    }
}
void Protester::getStunned() //stuns the protester and makes it not move for a longer period of time
{
    tickRest = std::max(50, 100 - (int)getWorld()->getLevel()*10);
    stunned = true;
}
void Protester::bribe() //bribes the protester if it is a regular one and picks up the gold
{
    if(getID()==TID_PROTESTER) {
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        getWorld()->increaseScore(25);
        leave = true;
    }
    else {
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        getWorld()->increaseScore(50);
        tickRest = std::max(50,100- int(getWorld()->getLevel())*10);
    }
}
bool Protester::isFacingPlayer() //checks if the protester is facing the tunnelman
{
    switch (getDirection()) {
        case left:
            return getWorld()->getPlayer()->getX() <= getX();
        case right:
            return getWorld()->getPlayer()->getX() >= getX();
        case up:
            return getWorld()->getPlayer()->getY() >= getY();
        case down:
            return getWorld()->getPlayer()->getY() <= getY();
        case none:
            return false;
    }
    return false;
}

RegularProtester::RegularProtester(StudentWorld* world)
: Protester(world, TID_PROTESTER, 5) {setVisible(true);}

HardcoreProtester::HardcoreProtester(StudentWorld* world)
: Protester(world, TID_HARD_CORE_PROTESTER, 20) {setVisible(true);}

int Protester::getHP() //returns hp of the protester
{
    return hitpoints;
}

void Protester::decHP(int points) //decrements the protesters hp
{
    hitpoints -= points;
}

void GoldNugget::doSomething() //gold nugget's actions every tick
{
    if (!isAlive()) return;
        if(!placed && protestPickup == true && getWorld()->goodieOverlap(getX(), getY(), this)) //does not allow gold to be placed on top of another piece of gold
        {
            getWorld()->getPlayer()->add(TID_GOLD);
            die();
        }
        if (!isVisible() && getWorld()->isPlayerInRadius(this, 4)) { //if undiscovered piece of gold, set visible
            setVisible(true);
            return;
        }
        if (tunnelPickup && getWorld()->isPlayerInRadius(this, 3)) { //if undiscovered piece of gold, pick it up
            die();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->getPlayer()->add(getID());
            getWorld()->increaseScore(10);
            return;
        }
        if (protestPickup) //if the tunnelman cannot pick it up and a protester can, check if protester is within radius
        {
            placed = true;
            Protester* p = (getWorld()->protesterInRadius(this, 3));
            if (p != nullptr) {
                die();
                p->bribe();
            };
            disappearIn(100);
        }
}
void GoldNugget::disappearIn(int ticks) //time until gold falls into oil
{
    if (currTicks == ticks) die();
    else currTicks++;
}

void Goodie::disappearIn(int ticks) //time until goodie disappears into the oil
{
    if (currTicks == ticks) die();
    else currTicks++;
}

void OilBarrel::doSomething() //checks what the oil barrel should do every tick
{
    if (!isAlive()) return;
    if (!isVisible() && getWorld()->isPlayerInRadius(this, 4)) { //check if it should be visible
        setVisible(true);
        return;
    }
    if (getWorld()->isPlayerInRadius(this, 3)) { //check if player picked it up
        die();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->foundOil();
        return;
    }
}

void Goodie::doSomething(){ //checks what a goodie should do every tick (sonar, waterpool)
    if (!isAlive()) return;
    if (getWorld()->isPlayerInRadius(this, 3)) { //check if picked up by player
        die();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->add(getID());
        getWorld()->increaseScore(75);
        return;
    }
    disappearIn(std::max(100, 300 - 10 * (int)getWorld()->getLevel())); //find time when it should disappear
}
