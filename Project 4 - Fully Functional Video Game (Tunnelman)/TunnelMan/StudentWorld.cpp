#include "StudentWorld.h"
#include <string>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

GameWorld* createStudentWorld(string assetDir) //creates studentworld
{
	return new StudentWorld(assetDir);
}
StudentWorld::StudentWorld(std::string assetDir) //constructor for studentworld
: GameWorld(assetDir),m_isFirstTick(true), m_tickSinceLast(0), currProtesters(0)
{
    for(int i = 0; i < 64; i++)
        for(int j = 0; j < 64; j++)
            v[i][j] = nullptr;
    player = nullptr;
}

int StudentWorld::init() //initializes student world
{
    
    m_tickSinceLast = 0;
    currProtesters = 0;
    srand(static_cast<unsigned int>(time(0))); //ensures new random values are generated when program is ran again
    probGoodie = getLevel() * 25 + 300;
    numBoulders = std::min((int)(getLevel()/ 2 + 2), 9);
    numGold = std::max((int)(5 - getLevel() / 2), 2);
    numOil = std::min((int)(2 + getLevel()), 21);
    m_isFirstTick = true;
    player = new Tunnelman(this);
    initializeEarth();
    initializeBoulders();
    int x, y;
    for(int i = 0; i < numGold; i++)
    {
        generateRandomCoordinates(x, y);
        GoldNugget *g = new GoldNugget(this, x, y, true, false);
        objects.push_back(g);
    }
    for(int i = 0; i < numOil; i++)
    {
        generateRandomCoordinates(x, y);
        OilBarrel *o = new OilBarrel(this, x, y);
        objects.push_back(o);
    }
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::initializeBoulders() //initializes the boulders in the game setting and removes the earth around it by calling boulderEarthOverlap
{
    int x, y;
    double xSquared, ySquared, euDistance;
    for(int i = 0; i < numBoulders; i++)
    {
        bool surrounded;
        do
        {
            surrounded = false;
            do{
            x = rand() % 61;
            y = rand() % 37 + 20;
            }while(!((x >= 0 && x <= 26) || (x >= 34 && x <= 60)));
                
            for(int i = 0; i < objects.size(); i++)
            {
                xSquared = x - objects[i]->getX();
                ySquared = y - objects[i]->getY();
                euDistance = sqrt((xSquared * xSquared) + (ySquared * ySquared));
                if(euDistance <= 6.0)
                    surrounded = true;
            }
        } while(surrounded);
        Boulder *b = new Boulder(this, x, y);
        objects.push_back(b);
        boulderEarthOverlap(x, y);
    }
}
void StudentWorld::initializeEarth() //initializes the array holding the oil field and picks which to set visible and which not to
{
    for (int i = 0; i <= 63; i++)
    {
        for(int j = 0; j <= 63; j++)
        {
                Earth *e = new Earth(this, i, j);
                v[i][j] = e;
            if(!(j < 60 && (j < 4 || !(i <= 33 && i >= 30))))
            {
                v[i][j]->setVisible(false);
            }
        }
    }
}
StudentWorld::~StudentWorld() //destructor
{
    cleanUp();
}
void StudentWorld::generateRandomCoordinates(int&x, int&y) //generates random coordinates for the oil and gold to be spawned
{
    double xSquared, ySquared, euDistance;
    bool surrounded;
    do
    {
        surrounded = false;
        do{
        x = rand() % 61;
        y = rand() % 57;
        }while((!((x >= 0 && x <= 26) || (x >= 34 && x <= 60))) || y == 0);
        
        for(int i = 0; i < objects.size(); i++)
        {
            xSquared = x - objects[i]->getX();
            ySquared = y - objects[i]->getY();
            euDistance = sqrt((xSquared * xSquared) + (ySquared * ySquared));
            if(euDistance <= 6.0)
                surrounded = true;
        }
    } while(surrounded);
}
int StudentWorld::move() //world actions every tick
{
    ticks++;
    display();
    player->doSomething(); //get player input
    for (int i = 0; i < objects.size(); i++) //get object actions and decrement life if player is dead
    {
            objects[i]->doSomething();
            if((!player->isAlive()))
            {
                decLives();
                playSound(SOUND_PLAYER_GIVE_UP);
                return GWSTATUS_PLAYER_DIED;
            }
            
    }
    createProtester(); //adds protesters with correct probability and abides by max number
    generateGoodie(); //calculates probablity of generating a goodie and generates it if it falls within probability during this tick
    if(numOil == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    std::vector<Actor*>::iterator it = objects.begin();

        while (it != objects.end()) {
            if (!(*it)->isAlive()) {
                delete *it;
                it = objects.erase(it);
            }
            else {
                it++;
            }
        }
    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::goodieOverlap(int x, int y)
{
    for(int i = 0; i < objects.size();i++)
    {
        if(newMoveOverlap(x, y, objects[i]))
            return true;
    }
    return false;
}

bool StudentWorld::goodieOverlap(int x, int y, Actor*b)
{
    for(int i = 0; i < objects.size();i++)
    {
        if(b != objects[i] && newMoveOverlap(x, y, objects[i]))
            return true;
    }
    return false;
       
}
void StudentWorld::generateGoodie() //calculates probablity of generating a goodie and generates it if it falls within probability during this tick
{
    if(((rand() % probGoodie) == 1))
    {
        if((rand()%5 + 1) == 1)
        {
            if((!goodieOverlap(0, 60)))
            {
                SonarKit* sonar = new SonarKit(this, 0, 60);
                if(isPlayerInRadius(sonar, 3))
                    delete sonar;
                else
                    objects.push_back(sonar);
            }
        }
        else
        {
            bool surrounded;
            int x, y;
            double xSquared, ySquared, euDistance;
            do{
                surrounded = false;
                do {
                    x = rand() % 61;
                    y = rand() % 61;
                } while (isThereEarth(x, y) || goodieOverlap(x, y));
                for(int i = 0; i < objects.size(); i++)
                {
                    if(newMoveOverlap(x, y, objects[i])) //check if location will cause overlap with other actors
                        surrounded = true;
                    xSquared = x - objects[i]->getX();
                    ySquared = y - objects[i]->getY();
                    euDistance = sqrt((xSquared * xSquared) + (ySquared * ySquared));
                    if(euDistance <= 6.0)
                        surrounded = true;
                }
            }while(surrounded);
            WaterPool* w = new WaterPool(this,x,y);
            if(isPlayerInRadius(w, 3))
                delete w;
            else
                objects.push_back(w);
        }
    }
}
string formatText(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int scr) //formats the game text so it is evenly spaced and matches the correct formatting
{
    stringstream s;
        s.fill('0');
        s << "Scr: " << setw(6) << scr;
        s.fill(' ');
        s << " Lvl: " << setw(2) << level << " Lives: " << setw(1) << lives << "  Hlth: " << setw(3) << health << '%' << "  Wtr: " << setw(2) << squirts << "  Gld: " << setw(2) << gold << "  Sonar: " << setw(2) << sonar << "  Oil Left: " << setw(2) << barrelsLeft;

        return s.str();
}

void StudentWorld::display() //displays the game text
{
    
    int level = getLevel();
    int lives = getLives();
    int health = (player->getHitPts() / 10.0) * 100;
    int squirts = player->getWater();
    int gold = player->getNuggets();
    int barrelsLeft = numOil;
    int sonar = player->getSonar();
    int scr = getScore();
    // Next, create a string from your statistics, of the form:
    // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
    string s = formatText(level, lives, health, squirts, gold, barrelsLeft, sonar, scr);
    // Finally, update the display text at the top of the screen with your
    // newly created stats
    setGameStatText(s);
}

void StudentWorld::createProtester() //adds protesters depending on time since last addition and probability of one being a hardcore protester
{
    int T = std::max(25, 200 - (int)getLevel());
    int P = std::min(15,(int)( 2 + getLevel() * 1.5));
    int probabilityOfHardcore = min(90, (int)getLevel() * 10 + 30);
    ;
    if (m_isFirstTick || (m_tickSinceLast > T && currProtesters < P)) {
        if (rand() % 100 + 1 < probabilityOfHardcore)
            objects.push_back(new HardcoreProtester(this));
        else
            objects.push_back(new RegularProtester(this));
        m_tickSinceLast = 0;
        currProtesters++;
        m_isFirstTick = false;
    }
    m_tickSinceLast++;
}
void StudentWorld::cleanUp() //releases dynamically allocated memory
{
    ticks = 0;
    if(player != nullptr)
    {
        delete player;
        player = nullptr;
    }
    for(int i = 0; i <64; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            if(v[i][j] != nullptr)
            {
                delete v[i][j];
                v[i][j] = nullptr;
            }
        }
    }
    std::vector<Actor*>::iterator it = objects.begin();

        while (it != objects.end()) {
                delete *it;
                it = objects.erase(it);
        }
}

bool StudentWorld::moveIsValid(int x, int y) //checks if the proposed coordinate move is valid on the coordinate plane and does not run into a boulder
{
    bool noBoulders = true;
    if(x <= 60 && x >= 0 && y <= 60 && y >= 0)
    {
        for(int i = 0; i < objects.size(); i++)
        {
            if(objects[i]->getID() == TID_BOULDER)
                if(newMoveOverlap(x,y, objects[i]))
                    noBoulders = false;
        }
    }
    else
    {
        return false;
    }
    return noBoulders;
}


bool StudentWorld::newMoveOverlap(int x, int y, Actor* b) //checks if the coordinates of this new move will overlap the passed actor
{
    if (x > (b->getX() + 3) || (b->getX()) > (x + 3))
         return false;
      if ((y + 3) < b->getY() || (b->getY() + 3) < y)
         return false;
      return true;
}
bool StudentWorld::overlapEarth(int x, int y, Actor::Direction d) //checks if there is earth in the direction pointed to by the tunnelman and makes it invisible if that is the case
{
    bool worked = false;
    switch(d)
    {
        case Actor::up:
            for(int i = 0; i < 4; i++)
            {
                if(v[x + i][y + 3]->isVisible())
                {
                    v[x + i][y + 3]->setVisible(false);
                    worked = true;
                }
            }
            if(worked)
                return true;
        break;
        case Actor::down:
            for(int i = 0; i < 4; i++)
            {
                if(v[x + i][y]->isVisible())
                {
                    v[x + i][y]->setVisible(false);
                    worked = true;
                }
            }
            if(worked)
                return true;
        break;
        case Actor::left:
            for(int i = 0; i < 4; i++)
            {
                if(v[x][y + i]->isVisible())
                {
                    v[x][y + i]->setVisible(false);
                    worked = true;
                }
            }
            if(worked)
                return true;
        break;
        case Actor::right:
            for(int i = 0; i < 4; i++)
            {
                if(v[x + 3][y + i]->isVisible())
                {
                    v[x + 3][y + i]->setVisible(false);
                    worked = true;
                }
            }
            if(worked)
                return true;
        break;
        default:
        break;
    }
    return false;
}
void StudentWorld::boulderEarthOverlap(int x, int y) //removes earth around the boulder for initializing
{
    for(int i = x; i < x + 4; i++)
        for(int j = y; j < y + 4; j++)
            v[i][j]->setVisible(false);
}
bool StudentWorld::boulderShouldFall(int x, int y) //determines if there is earth under the boulder
{
    bool itShould = true;
    for(int i = 0; i < 4; i++)
    {
        if(v[x + i][y-1]->isVisible())
        {
            itShould = false;
        }
    }
    return itShould;
}

void StudentWorld::sonarUsed(int x, int y, int radius) //finds hidden barrels and gold nearby for the sonar
{
	int a, b;
	vector<Actor*>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it)->getID() == TID_BARREL || (*it)->getID() == TID_GOLD)
		{
			a = (*it)->getX(); b = (*it)->getY();
			if ((x - a)*(x - a) + (y - b)*(y - b) <= radius * radius)
				(*it)->setVisible(true);
		}
	}
}

Protester* StudentWorld::protesterInRadius(Actor * actor, int radius) //checks if a protester is within the radius of the object passed
{
    vector<Actor*>::iterator it;
    for (it = objects.begin(); it != objects.end(); it++) {
        if (((*it)->getID() == TID_PROTESTER || (*it)->getID() == TID_HARD_CORE_PROTESTER))
            if (withinRadius(actor->getX(), actor->getY(), (*it)->getX(), (*it)->getY(), radius))
            {
                Protester* p = dynamic_cast<Protester*>((*it));
                if(p->isLeaving() || p->isStunned())
                {
                    
                }
                else
                    return dynamic_cast<Protester*>(*it);
            }
    }
    return nullptr;
}

Tunnelman* StudentWorld::getPlayer() //return the tunnelman object
{
    return player;
}

bool StudentWorld::isThereEarth(int x, int y) //checks if there is a piece of active earth assuming a 4x4 object, given the coordinates provided
{
    for (int i = x; i < x + 4; i++) {

        for (int j = y; j <y + 4; j++) {

            if (v[i][j]->isVisible())
                return true;
        }
    }
    return false;
}

bool StudentWorld::checkIfOutOfBoundsOrEarth(int x, int y) //used for squirt, checks if either the blast will hit earth or go out of bounds (so that a new Squirt object will not be created
{
    for (int i = x; i < x + 4; i++) {
        for (int j = y; j <y + 4; j++) {
            if (  (i < 0 || i > 63 || j < 0 || j > 63) || v[i][j]->isVisible())
                return true;
        }
    }
    return false;
}

bool StudentWorld::isThereBoulder(int x, int y) //checks if there is a boulder in a radius given coordinates
{
    vector<Actor*>::iterator it;
    for (it = objects.begin(); it != objects.end(); it++) {
        if ((*it)->getID() == TID_BOULDER && withinRadius(x, y, (*it)->getX(), (*it)->getY(), 3))
            return true;
    }
    return false;
}

bool StudentWorld::withinRadius(int x1, int y1, int x2, int y2, int radius) //checks if an object is within the radius of the other given x and y coordinates and radius
{
    if ((((x1 - x2)*(x1 - x2)) + ((y1 - y2)*(y1 - y2))) <= (radius * radius))
        return true;
    else return false;
}
void StudentWorld::decProtester(){
    currProtesters--;
}
GraphObject::Direction StudentWorld::leaveGame(Protester * pr) //moves the protester to the exit once it has been killed through a queue maze system
{
    for (int i = 0; i < 63; i++){
        for (int j = 0; j < 63; j++){
            protesterPath[i][j]=0;
        }
    }
    int a = pr->getX();
    int b = pr->getY();
    std::queue<Coordinates> q;
    q.push(Coordinates(60,60));
    protesterPath[60][60]=1;
    while (!q.empty()) {
        Coordinates c = q.front();
        q.pop();
        int x=c.x;
        int y=c.y;
        

        if(ableToMove(x,y, GraphObject::left)&& protesterPath[x-1][y]==0){
            q.push(Coordinates(x-1,y));
            protesterPath[x-1][y] =protesterPath[x][y]+1;
        }

        if(ableToMove(x,y, GraphObject::right)&& protesterPath[x+1][y]==0){
            q.push(Coordinates(x+1,y));
            protesterPath[x+1][y] =protesterPath[x][y]+1;
        }

        if(ableToMove(x,y, GraphObject::up)&& protesterPath[x][y+1]==0){
            q.push(Coordinates(x,y+1));
            protesterPath[x][y+1] =protesterPath[x][y]+1;
        }
        
        if(ableToMove(x,y, GraphObject::down)&& protesterPath[x][y-1]==0){
            q.push(Coordinates(x,y-1));
            protesterPath[x][y-1] =protesterPath[x][y]+1;
        }
    }
    if(ableToMove(a,b, GraphObject::left)&& protesterPath[a-1][b]<protesterPath[a][b])
    {
        return GraphObject::left;
    }
    if(ableToMove(a,b, GraphObject::right)&& protesterPath[a+1][b]<protesterPath[a][b])
    {
        return GraphObject::right;
    }
    if(ableToMove(a,b, GraphObject::up)&& protesterPath[a][b+1]<protesterPath[a][b])
    {
        return GraphObject::up;
    }
    if(ableToMove(a,b, GraphObject::down)&&protesterPath[a][b-1]<protesterPath[a][b])
    {
        return GraphObject::down;
    }
    return GraphObject::none;
}

bool StudentWorld::ableToMove(int x, int y, GraphObject::Direction direction) //checks if it is possible to move in the given direction
{
    switch (direction) {
    case GraphObject::left:
        return (x!=0&&!isThereEarth(x - 1, y) && !isThereBoulder(x, y));
    case GraphObject::right:
        return (x!=60&& !isThereEarth(x + 1, y) && !isThereBoulder(x + 1, y));
    case GraphObject::up:
        return (y!=60&&!isThereEarth(x, y + 1) && !isThereBoulder(x, y + 1));
    case GraphObject::down:
        return (y!=0&& !isThereEarth(x, y - 1) && !isThereBoulder(x, y - 1));
    case GraphObject::none:
            return false;
    }
    return false;
}

bool StudentWorld::isPlayerInRadius(Actor *a, int radius) //check if a passed actor is within the radius of the tunnelman
{
    if(withinRadius(a->getX(), a->getY(), player->getX(), player->getY(), radius))
        return true;
    return false;
}

GraphObject::Direction StudentWorld::hardcoreProtesterPath(Protester * pr, int M) //find the direction the hardcore protester should go to by plotting a path towards the player
{
    for (int i = 0; i < 63; i++){
        for (int j = 0; j < 63; j++){
            protesterPath[i][j]=0;
        }
    }
    int a = pr->getX();
    int b = pr->getY();
    std::queue<Coordinates> q;
    q.push(Coordinates(getPlayer()->getX(),getPlayer()->getY()));
    protesterPath[getPlayer()->getX()][getPlayer()->getY()]=1;
    while (!q.empty()) {
        Coordinates c = q.front();
        q.pop();
        int x=c.x;
        int y=c.y;
        
        //left
        if(ableToMove(x,y, GraphObject::left)&& protesterPath[x-1][y]==0){
            q.push(Coordinates(x-1,y));
            protesterPath[x-1][y] =protesterPath[x][y]+1;
        }
        //right
        if(ableToMove(x,y, GraphObject::right)&& protesterPath[x+1][y]==0){
            q.push(Coordinates(x+1,y));
            protesterPath[x+1][y] =protesterPath[x][y]+1;
        }
        //up
        if(ableToMove(x,y, GraphObject::up)&& protesterPath[x][y+1]==0){
            q.push(Coordinates(x,y+1));
            protesterPath[x][y+1] =protesterPath[x][y]+1;
        }
        // down
        if(ableToMove(x,y, GraphObject::down)&& protesterPath[x][y-1]==0){
            q.push(Coordinates(x,y-1));
            protesterPath[x][y-1] =protesterPath[x][y]+1;
        }
    }
    if(protesterPath[a][b] <= M+1){
        if(ableToMove(a,b, GraphObject::left)&& protesterPath[a-1][b]<protesterPath[a][b])
            return GraphObject::left;
        if(ableToMove(a,b, GraphObject::right)&& protesterPath[a+1][b]<protesterPath[a][b])
            return GraphObject::right;
        if(ableToMove(a,b, GraphObject::up)&& protesterPath[a][b+1]<protesterPath[a][b])
            return GraphObject::up;
        if(ableToMove(a,b, GraphObject::down)&&protesterPath[a][b-1]<protesterPath[a][b])
            return GraphObject::down;
    }
    return GraphObject::none;
}
void StudentWorld::newObject(Actor* a) //adds a new object to the objects vector
{
    objects.push_back(a);
}
void StudentWorld::foundOil() //decrements oil needed to find by 1
{
    numOil--;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
