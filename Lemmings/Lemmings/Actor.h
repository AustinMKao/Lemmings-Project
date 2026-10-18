#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
    public:
        Actor(StudentWorld* sw, int imageID, Coord startCoord, int dir = right);
        virtual ~Actor();
        virtual void doSomething() = 0;
        virtual bool obstruct() const;
        virtual bool climbable() const;
        virtual bool launchable() const;
        virtual bool attractable() const;
        void setDead();
        bool isDead();
        virtual int getBounce() const;
        virtual bool killable() const;
        virtual bool getLemming() const;
        virtual bool getIce() const;
    private:
        StudentWorld* world;
        bool dead;
    protected:
        StudentWorld* getWorld() const;
};
class PassiveActor : public Actor
{
    public:
        PassiveActor(StudentWorld* sw, int imageID, Coord startCoord);
        virtual ~PassiveActor();
        virtual void doSomething();
};

class FloorBrick : public PassiveActor
{
    public:
        FloorBrick(StudentWorld* sw, Coord startCoord);
        virtual ~FloorBrick();
        virtual bool obstruct() const override;
};

class Net : public PassiveActor
{
    public:
        Net(StudentWorld* sw, Coord startCoord);
        virtual ~Net();
        virtual bool climbable() const override;
};

class Pheromone : public PassiveActor
{
    public:
        Pheromone(StudentWorld* sw, Coord startCoord);
        virtual ~Pheromone();
        virtual bool attractable() const override;
};

class Bonfire : public PassiveActor
{
    public:
        Bonfire(StudentWorld* sw, Coord startCoord);
        virtual ~Bonfire();
        virtual bool killable() const override;
};

class Trampoline : public PassiveActor
{
    public:
        Trampoline(StudentWorld* sw, Coord startCoord);
        virtual ~Trampoline();
        virtual bool launchable() const override;
        virtual int getBounce() const override;
};

class Spring : public PassiveActor
{
    public:
        Spring(StudentWorld* sw, Coord startCoord);
        virtual ~Spring();
        virtual bool launchable() const override;
        virtual int getBounce() const override;
};

class OneWayDoor : public Actor
{
    public:
        OneWayDoor(StudentWorld* sw, Coord startCoord, int dir);
        virtual ~OneWayDoor();
};

class LeftDoor : public OneWayDoor
{
    public:
        LeftDoor(StudentWorld* sw, Coord startCoord);
        virtual ~LeftDoor();
        virtual void doSomething() override;
};

class RightDoor : public OneWayDoor
{
    public:
        RightDoor(StudentWorld* sw, Coord startCoord);
        virtual ~RightDoor();
        virtual void doSomething() override;
};

class Exit : public Actor
{
    public:
        Exit(StudentWorld* sw, Coord startCoord);
        virtual ~Exit();
        virtual void doSomething() override;
};

class Player : public Actor
{
    public:
        Player(StudentWorld* sw);
        virtual ~Player();
        virtual void doSomething() override;
        void movePlayer(int dir);
        bool validPlace(char key);
};

class WaitingActor : public Actor
{
    public:
        WaitingActor(StudentWorld* sw, int imageID, Coord startCoord, int i);
        virtual ~WaitingActor();
        int getIdle();
        void resetIdle();
        void increaseIdle();
    private:
        int idle;
};

class IceMonster : public WaitingActor
{
    public:
        IceMonster(StudentWorld* sw, Coord startCoord);
        virtual ~IceMonster();
        virtual void doSomething() override;
        virtual bool killable() const override;
        virtual bool getIce() const override;
    private:
        bool encounter(int dir);
};

class LemmingFactory : public WaitingActor
{
    public:
        LemmingFactory(StudentWorld* sw, Coord startCoord);
        virtual ~LemmingFactory();
        virtual void doSomething() override;
    private:
        int lemmings;
};

class Lemming : public WaitingActor
{
    public:
        Lemming(StudentWorld* sw, Coord startCoord);
        virtual ~Lemming();
        virtual void doSomething() override;
        void resetBounce();
        void addBounce(int b);
        void die();
        virtual bool getLemming() const override;
    private:
        std::string movingState;
        int fallDistance;
        int bounceUp;
        int maxBounce;
        bool decideMove();
        void attraction();
};
#endif // ACTOR_H_
