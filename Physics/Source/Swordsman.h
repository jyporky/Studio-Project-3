#pragma once
#include "Enemy.h"
class Swordsman :
    public Enemy
{
public:
    enum SWORDSMAN_STATES {
        IDLE,
        CHASE,
        ATTACK,
        NUM_SM_STATES,
    };
    Swordsman();
    ~Swordsman();
    bool Update(double dt);
    void Init();


private:
    unsigned sCurrState;

};

