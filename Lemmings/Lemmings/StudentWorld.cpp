#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), time(2000), tools(""), builder(nullptr), lemmingsDied(0), lemmingsSave(0)
{}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    int level = getLevel();
    ostringstream oss;
    oss << "level" << setfill('0') << setw(2) << level << ".txt";
    std::string curLevel = oss.str();
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found)
    {
        return GWSTATUS_PLAYER_WON;
    }
    if(result == Level::load_fail_bad_format)
    {
        return GWSTATUS_LEVEL_ERROR; // something bad happened!
    }
    // otherwise the load was successful and you can access the
    // contents of the level
    time = 2000;
    tools = lev.getTools();
    for(int i = 0; i < VIEW_WIDTH; i++)
    {
        for(int j = 0; j < VIEW_HEIGHT; j++)
        {
            Coord p(i, j);
            Level::MazeEntry item = lev.getContentsOf(p);
            if(item == Level::floor)
            {
                FloorBrick* f = new FloorBrick(this, p);
                a.push_back(f);
            }
            else if(item == Level::ice_monster)
            {
                IceMonster* m = new IceMonster(this, p);
                a.push_back(m);
            }
            else if(item == Level::lemming_factory)
            {
                LemmingFactory* lf = new LemmingFactory(this, p);
                a.push_back(lf);
            }
            else if(item == Level::bonfire)
            {
                Bonfire* b = new Bonfire(this, p);
                a.push_back(b);
            }
            else if(item == Level::trampoline)
            {
                Trampoline* t = new Trampoline(this, p);
                a.push_back(t);
            }
            else if(item == Level::net)
            {
                Net* n = new Net(this, p);
                a.push_back(n);
            }
            else if(item == Level::pheromone)
            {
                Pheromone* ph = new Pheromone(this, p);
                a.push_back(ph);
            }
            else if(item == Level::spring)
            {
                Spring* s = new Spring(this, p);
                a.push_back(s);
            }
            else if(item == Level::lemming_exit)
            {
                Exit* e = new Exit(this, p);
                a.push_back(e);
            }
        }
    }
    builder = new Player(this);
    setGameStatText(createGameText());
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    time -= ANIMATION_POSITIONS_PER_TICK;
    builder->doSomething();
    for(Actor* p : pending)
    {
        a.push_back(p);
    }
    pending.clear();
    for(int i = 0; i < a.size(); i++)
    {
        a[i]->doSomething();
    }
    setGameStatText(createGameText());
    for (vector<Actor*>::iterator it = a.begin(); it != a.end();)
    {
        if ((*it)->isDead())
        {
            delete *it;
            it = a.erase(it);
        }
        else
        {
            ++it;
        }
    }
    if(lemmingsDied >= 6)
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if(lemmingsSave + lemmingsDied >= 10)
    {
        increaseScore(time);
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if(time <= 0 && lemmingsSave < 5)
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if(time <= 0 && lemmingsSave >= 5)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i < a.size(); i++)
    {
        delete a[i];
    }
    a.clear();
    for(Actor* p : pending)
    {
        delete p;
    }
    pending.clear();
    delete builder;
    builder = nullptr;
    lemmingsDied = 0;
    lemmingsSave = 0;
}

bool StudentWorld::notMove(Coord c)
{
    if(c.x < 0 || c.x >= VIEW_WIDTH || c.y < 0 || c.y >= VIEW_HEIGHT)
    {
        return true;
    }
    for(int i = 0; i < a.size(); i++)
    {
        if(a[i]->getCoord() == c && a[i]->obstruct())
        {
            return true;
        }
    }
    return false;
}

string StudentWorld::createGameText()
{
    ostringstream oss;
    oss << "Score: " << setfill('0') << setw(5) << getScore();
    oss << "  Level: " << setfill('0') << setw(2) << getLevel();
    oss << "  Lives: " << setfill('0') << setw(2) << getLives();
    oss << "  Saved: " << setfill('0') << setw(2) << lemmingsSave;
    string displayTools = tools;
    if(displayTools == "")
    {
        displayTools = "None";
    }
    oss << "  Tools: " << displayTools;
    oss << "  Time Left: " << setfill('0') << setw(4) << time;
    return oss.str();
}

void StudentWorld::placeActor(char key, Coord c)
{
    if(key == 'T')
    {
        Trampoline* t = new Trampoline(this, c);
        pending.push_back(t);
    }
    else if(key == 'N')
    {
        Net* n = new Net(this, c);
        pending.push_back(n);
    }
    else if(key == 'P')
    {
        Pheromone* p = new Pheromone(this, c);
        pending.push_back(p);
    }
    else if(key == 'S')
    {
        Spring* s = new Spring(this, c);
        pending.push_back(s);
    }
    else if(key == '<')
    {
        LeftDoor* ld = new LeftDoor(this, c);
        pending.push_back(ld);
    }
    else if(key == '>')
    {
        RightDoor* rd = new RightDoor(this, c);
        pending.push_back(rd);
    }
}

string StudentWorld::getTools()
{
    return tools;
}

void StudentWorld::setTools(string t)
{
    tools = t;
}

bool StudentWorld::empty(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(c == a[i]->getCoord())
        {
            return false;
        }
    }
    return true;
}

void StudentWorld::spawnLemming(Coord c)
{
    pending.push_back(new Lemming(this, c));
}

int StudentWorld::isAttractable(Coord c)
{
    int closest = -1;
    int distance = 6;
    for(int i = 0; i < a.size(); i++)
    {
        if(a[i]->attractable())
        {
            if(c.y == a[i]->getCoord().y)
            {
                if(abs(c.x - a[i]->getCoord().x) <= 5)
                {
                    if(c.x != a[i]->getCoord().x)
                    {
                        if(abs(c.x - a[i]->getCoord().x) <= distance)
                        {
                            distance = abs(c.x - a[i]->getCoord().x);
                            closest = a[i]->getCoord().x;
                        }
                    }
                }
            }
        }
    }
    return closest;
}

bool StudentWorld::findNet(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(c == a[i]->getCoord() && a[i]->climbable())
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::findWall(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(c == a[i]->getCoord() && a[i]->obstruct())
        {
            return true;
        }
    }
    return false;
}

int StudentWorld::findTrampOrSpring(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(c == a[i]->getCoord() && a[i]->launchable())
        {
            return a[i]->getBounce();
        }
    }
    return -1;
}

bool StudentWorld::findFireOrIceMonster(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(c == a[i]->getCoord() && a[i]->killable())
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::findIceMonster(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(c == a[i]->getCoord() && a[i]->getIce())
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::getLemming(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(c == a[i]->getCoord() && a[i]->getLemming())
        {
            return true;
        }
    }
    return false;
}

void StudentWorld::turnRight(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(getLemming(c) && c == a[i]->getCoord())
        {
            a[i]->setDirection(GraphObject::right);
        }
        if(findIceMonster(c) && c == a[i]->getCoord())
        {
            a[i]->setDirection(GraphObject::right);
        }
    }
}

void StudentWorld::turnLeft(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(getLemming(c) && c == a[i]->getCoord())
        {
            a[i]->setDirection(GraphObject::left);
        }
        if(findIceMonster(c) && c == a[i]->getCoord())
        {
            a[i]->setDirection(GraphObject::left);
        }
    }
}

void StudentWorld::lemmingDied()
{
    lemmingsDied++;
}

void StudentWorld::lemmingSave(Coord c)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(c == a[i]->getCoord() && a[i]->getLemming())
        {
            a[i]->setDead();
            break;
        }
    }
    playSound(SOUND_LEMMING_SAVED);
    increaseScore(SCORE_SAVED_LEMMING);
    lemmingsSave++;
}
