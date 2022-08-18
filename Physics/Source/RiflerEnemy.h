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
    void SetWeapon(Weapon* weapon);
    Weapon* GetWeapon();


private:
    unsigned sCurrState;
    unsigned attackRange;
};