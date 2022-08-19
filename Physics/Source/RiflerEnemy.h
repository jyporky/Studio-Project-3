#pragma once
#include "Enemy.h"


class Rifler :
    public Enemy
{
public:
    enum RIFLER_STATES {
        IDLE,
        CHASE,
        ATTACK,
        NUM_SM_STATES,
    };
    Rifler();
    ~Rifler();
    bool Update(double dt);
    void Init();
    bool IsSpawningBullet();

private:
    bool isSpawningBullet;
    unsigned sCurrState;
    unsigned attackRange;
};