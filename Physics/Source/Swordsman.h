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
        BACK_OFF,
        NUM_SM_STATES,
    };
    Swordsman();
    ~Swordsman();
    bool Update(double dt);
    void Init();
    void SetWeapon(Weapon* weapon);
    Weapon* GetWeapon();


private:
    unsigned sCurrState;
    unsigned attackRange;
};