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
    bool getStunned();
    void makeEnemyStunned();
    void resetEnemyStunned();
    void turnEnemy();
    void resetEnemyTurned();
    void SetUndead();
private:
    unsigned sCurrState;
    unsigned attackRange;
    float switchtime;
    bool moveleft = true;
    float leftdt = 0;
    bool undead;
};