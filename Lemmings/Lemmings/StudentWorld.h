#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        virtual ~StudentWorld();
        virtual int init();
        virtual int move();
        virtual void cleanUp();
        bool notMove(Coord c);
        std::string createGameText();
        void placeActor(char key, Coord c);
        std::string getTools();
        void setTools(std::string t);
        bool empty(Coord c);
        void spawnLemming(Coord c);
        int isAttractable(Coord c);
        bool findNet(Coord c);
        bool findWall(Coord c);
        int findTrampOrSpring(Coord c);
        void lemmingDied();
        void lemmingSave(Coord c);
        bool findFireOrIceMonster(Coord c);
        bool getLemming(Coord c);
        void turnRight(Coord c);
        void turnLeft(Coord c);
        bool findIceMonster(Coord c);
    private:
        std::vector<Actor*> a;
        std::vector<Actor*> pending;
        Actor* builder;
        int time;
        std::string tools;
        int lemmingsDied;
        int lemmingsSave;
};

#endif // STUDENTWORLD_H_
