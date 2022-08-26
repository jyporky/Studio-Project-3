#pragma once
#include "Enemy.h"
#include "Skill.h"
#include "EMPSkill.h"

class Rifler :
    public Enemy
{
public:
    enum RIFLER_STATES {
        IDLE,
        CHASE,
        ATTACK,
        RUN,
        CAMP,
        NUM_SM_STATES,
    };
    Rifler();
    ~Rifler();
    bool Update(double dt);
    void Init();
    bool IsSpawningBullet();
    bool getStunned();
    void makeEnemyStunned();
    void resetEnemyStunned();
    bool getTurnedState();
    void turnEnemy();
    void resetEnemyTurned();
    Entity* getTarget();
protected:
   
private:
    bool isSpawningBullet;
    unsigned sCurrState;
    unsigned attackRange;
    unsigned runRange;
    bool isCamper;
};