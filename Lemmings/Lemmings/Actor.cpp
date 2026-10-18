#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
#include <string>
using namespace std;

Actor::Actor(StudentWorld* sw, int imageID, Coord startCoord, int dir)
    :GraphObject(imageID, startCoord, dir), world(sw), dead(false)
{}

Actor::~Actor()
{}

void Actor::setDead()
{
    dead = true;
}

bool Actor::isDead()
{
    return dead;
}

StudentWorld* Actor::getWorld() const
{
    return world;
}

bool Actor::obstruct() const
{
    return false;
}

bool Actor::climbable() const
{
    return false;
}

bool Actor::launchable() const
{
    return false;
}

bool Actor::attractable() const
{
    return false;
}

int Actor::getBounce() const
{
    return -1;
}

bool Actor::killable() const
{
    return false;
}

bool Actor::getLemming() const
{
    return false;
}

bool Actor::getIce() const
{
    return false;
}

PassiveActor::PassiveActor(StudentWorld* sw, int imageID, Coord startCoord)
    :Actor(sw, imageID, startCoord)
{}

PassiveActor::~PassiveActor()
{}

void PassiveActor::doSomething()
{}

FloorBrick::FloorBrick(StudentWorld* sw, Coord startCoord)
    :PassiveActor(sw, IID_FLOOR, startCoord)
{}

FloorBrick::~FloorBrick()
{}

bool FloorBrick::obstruct() const
{
    return true;
}

Net::Net(StudentWorld* sw, Coord startCoord)
    :PassiveActor(sw, IID_NET, startCoord)
{}

Net::~Net()
{}

bool Net::climbable() const
{
    return true;
}

Pheromone::Pheromone(StudentWorld* sw, Coord startCoord)
    :PassiveActor(sw, IID_PHEROMONE, startCoord)
{}

Pheromone::~Pheromone()
{}

bool Pheromone::attractable() const
{
    return true;
}

Bonfire::Bonfire(StudentWorld* sw, Coord startCoord)
    :PassiveActor(sw, IID_BONFIRE, startCoord)
{}

Bonfire::~Bonfire()
{}

bool Bonfire::killable() const
{
    return true;
}

Trampoline::Trampoline(StudentWorld* sw, Coord startCoord)
    :PassiveActor(sw, IID_TRAMPOLINE, startCoord)
{}

Trampoline::~Trampoline()
{}

bool Trampoline::launchable() const
{
    return true;
}

int Trampoline::getBounce() const
{
    return 0;
}

Spring::Spring(StudentWorld* sw, Coord startCoord)
    :PassiveActor(sw, IID_SPRING, startCoord)
{}

Spring::~Spring()
{}

bool Spring::launchable() const
{
    return true;
}

int Spring::getBounce() const
{
    return 15;
}

OneWayDoor::OneWayDoor(StudentWorld* sw, Coord startCoord, int dir)
    :Actor(sw, IID_ONE_WAY_DOOR, startCoord, dir)
{}

OneWayDoor::~OneWayDoor()
{}

LeftDoor::LeftDoor(StudentWorld* sw, Coord startCoord)
    :OneWayDoor(sw, startCoord, left)
{}

LeftDoor::~LeftDoor()
{}

void LeftDoor::doSomething()
{
    if(getWorld()->getLemming(getCoord()) || getWorld()->findIceMonster(getCoord()))
    {
        getWorld()->turnLeft(getCoord());
    }
}

RightDoor::RightDoor(StudentWorld* sw, Coord startCoord)
    :OneWayDoor(sw, startCoord, right)
{}

RightDoor::~RightDoor()
{}

void RightDoor::doSomething()
{
    if(getWorld()->getLemming(getCoord()) || getWorld()->findIceMonster(getCoord()))
    {
        getWorld()->turnRight(getCoord());
    }
}

Exit::Exit(StudentWorld* sw, Coord startCoord)
    :Actor(sw, IID_EXIT, startCoord)
{}

Exit::~Exit()
{}

void Exit::doSomething()
{
    if(getWorld()->getLemming(getCoord()))
    {
        getWorld()->lemmingSave(getCoord());
    }
}

Player::Player(StudentWorld* sw)
    :Actor(sw, IID_PLAYER, Coord(VIEW_WIDTH/2, VIEW_HEIGHT/2))
{}

Player::~Player()
{}

void Player::doSomething()
{
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                movePlayer(left);
                break;
            case KEY_PRESS_RIGHT:
                movePlayer(right);
                break;
            case KEY_PRESS_UP:
                movePlayer(up);
                break;
            case KEY_PRESS_DOWN:
                movePlayer(down);
                break;
            case 'T':
            case 't':
                if(validPlace('T'))
                {
                    getWorld()->placeActor('T', getCoord());
                }
                break;
            case 'N':
            case 'n':
                if(validPlace('N'))
                {
                    getWorld()->placeActor('N', getCoord());
                }
                break;
            case 'P':
            case 'p':
                if(validPlace('P'))
                {
                    getWorld()->placeActor('P', getCoord());
                }
                break;
            case 'S':
            case 's':
                if(validPlace('S'))
                {
                    getWorld()->placeActor('S', getCoord());
                }
                break;
            case '<':
                if(validPlace('<'))
                {
                    getWorld()->placeActor('<', getCoord());
                }
                break;
            case '>':
                if(validPlace('>'))
                {
                    getWorld()->placeActor('>', getCoord());
                }
                break;
        }
    }
}

void Player::movePlayer(int dir)
{
    if(dir == left && getCoord().x > 1)
    {
        moveTo(left);
    }
    else if(dir == right && getCoord().x < VIEW_WIDTH-2)
    {
        moveTo(right);
    }
    else if(dir == up && getCoord().y < VIEW_HEIGHT-2)
    {
        moveTo(up);
    }
    else if(dir == down && getCoord().y > 1)
    {
        moveTo(down);
    }
}

bool Player::validPlace(char key)
{
    string t = "";
    bool occur = false;
    bool v = getWorld()->empty(getCoord());
    if(v == false)
    {
        return false;
    }
    for(int i = 0; i < getWorld()->getTools().size(); i++)
    {
        if(getWorld()->getTools().at(i) == key && occur == false)
        {
            occur = true;
            continue;
        }
        t += getWorld()->getTools().at(i);
    }
    if(t.size() == getWorld()->getTools().size())
    {
        return false;
    }
    else
    {
        getWorld()->setTools(t);
    }
    return true;
}

WaitingActor::WaitingActor(StudentWorld* sw, int imageID, Coord startCoord, int i)
    :Actor(sw, imageID, startCoord), idle(i)
{}

WaitingActor::~WaitingActor()
{}

int WaitingActor::getIdle()
{
    return idle;
}

void WaitingActor::resetIdle()
{
    idle = 0;
}

void WaitingActor::increaseIdle()
{
    idle++;
}

IceMonster::IceMonster(StudentWorld* sw, Coord startCoord)
    :WaitingActor(sw, IID_ICE_MONSTER, startCoord, 9)
{}

IceMonster::~IceMonster()
{}

void IceMonster::doSomething()
{
    increaseIdle();
    if(getIdle() < 10)
    {
        return;
    }
    resetIdle();
    if(getWorld()->getLemming(getCoord()))
    {
        return;
    }
    if(encounter(right) && getDirection() == right)
    {
        setDirection(left);
        return;
    }
    if(encounter(left) && getDirection() == left)
    {
        setDirection(right);
        return;
    }
    moveTo(getDirection());
}

bool IceMonster::killable() const
{
    return true;
}

bool IceMonster::getIce() const
{
    return true;
}

bool IceMonster::encounter(int dir)
{
    //walls
    if(getWorld()->notMove(getTargetCoord(dir)) && getDirection() == dir)
    {
        return true;
    }
    //floor
    if(getWorld()->notMove(getTargetCoord(getTargetCoord(down), dir)) == false && getDirection() == dir)
    {
        return true;
    }
    return false;
}

LemmingFactory::LemmingFactory(StudentWorld* sw, Coord startCoord)
    :WaitingActor(sw, IID_LEMMING_FACTORY, startCoord, 0), lemmings(10)
{}

LemmingFactory::~LemmingFactory()
{}

void LemmingFactory::doSomething()
{
    increaseIdle();
    if(getIdle() < 100)
    {
        return;
    }
    resetIdle();
    if(lemmings <= 0)
    {
        return;
    }
    getWorld()->spawnLemming(getCoord());
    lemmings--;
}

Lemming::Lemming(StudentWorld* sw, Coord startCoord)
 :WaitingActor(sw, IID_LEMMING, startCoord, 1), movingState("walking"), fallDistance(0), bounceUp(0), maxBounce(0)
{}

Lemming::~Lemming()
{}

void Lemming::doSomething()
{
    if(isDead())
    {
        return;
    }
    if(movingState != "bouncing")
    {
        maxBounce = getWorld()->findTrampOrSpring(getCoord());
        if(maxBounce != -1)
        {
            movingState = "bouncing";
            bounceUp = 0;
            if(maxBounce == 0)
            {
                maxBounce = fallDistance - 1;
            }
            getWorld()->playSound(SOUND_BOUNCE);
        }
    }
    if(getWorld()->findFireOrIceMonster(getCoord()))
    {
        die();
        return;
    }
    if(decideMove() == false)
    {
        return;
    }
    attraction();
    if(movingState == "walking")
    {
        if(getWorld()->findNet(getCoord()))
        {
            movingState = "climbing";
            return;
        }
        if(getWorld()->findWall(getTargetCoord(getDirection())))
        {
            if(getDirection() == left)
            {
                setDirection(right);
            }
            else if(getDirection() == right)
            {
                setDirection(left);
            }
            return;
        }
        if(getWorld()->findWall(getTargetCoord(getTargetCoord(down), getDirection())))
        {
            moveTo(getDirection());
            return;
        }
        movingState = "falling";
        fallDistance = 0;
        moveTo(getDirection());
        return;
    }
    if(movingState == "falling")
    {
        if(getWorld()->findNet(getCoord()))
        {
            movingState = "climbing";
            return;
        }
        if(getWorld()->notMove(getTargetCoord(down)))
        {
            if(fallDistance > 5)
            {
                die();
            }
            else
            {
                movingState = "walking";
            }
            return;
        }
        fallDistance += 1;
        moveTo(down);
        return;
    }
    if(movingState == "climbing")
    {
        if(getWorld()->findNet(getCoord()) == false)
        {
            movingState = "walking";
            return;
        }
        if(getWorld()->notMove(getTargetCoord(up)))
        {
            return;
        }
        moveTo(up);
        return;
    }
    if(movingState == "bouncing")
    {
        if(getWorld()->findNet(getCoord()))
        {
            movingState = "climbing";
            return;
        }
        if(bounceUp < maxBounce)
        {
            if(getWorld()->notMove(getTargetCoord(up)) == false)
            {
                moveTo(up);
                bounceUp++;
            }
            else
            {
                bounceUp = maxBounce;
            }
        }
        if(bounceUp == maxBounce)
        {
            if(getWorld()->findWall(getTargetCoord(getDirection())))
            {
                if(getDirection() == right)
                {
                    setDirection(left);
                }
                else if(getDirection() == left)
                {
                    setDirection(right);
                }
            }
            else
            {
                moveTo(getDirection());
            }
            movingState = "falling";
            fallDistance = 0;
        }
    }
}

bool Lemming::getLemming() const
{
    return true;
}

void Lemming::resetBounce()
{
    bounceUp = 0;
}

void Lemming::addBounce(int b)
{
    bounceUp += b;
}

bool Lemming::decideMove()
{
    increaseIdle();
    if(movingState == "walking")
    {
        if(getIdle() < 4)
        {
            return false;
        }
        resetIdle();
    }
    else
    {
        if(getIdle() < 2)
        {
            return false;
        }
        resetIdle();
    }
    return true;
}

void Lemming::attraction()
{
    int x = getWorld()->isAttractable(getCoord());
    if(x < getCoord().x && x != -1)
    {
        setDirection(left);
    }
    if(x > getCoord().x)
    {
        setDirection(right);
    }
}

void Lemming::die()
{
    if(isDead())
    {
        return;
    }
    getWorld()->playSound(SOUND_LEMMING_DIE);
    getWorld()->lemmingDied();
    setDead();
}

