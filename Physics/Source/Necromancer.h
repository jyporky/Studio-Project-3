#pragma once
#include "Enemy.h"


class Necromancer :
    public Enemy
{
public:
    enum NECROMANCER_STATES {
        IDLE,
        CHASE,
        SPAWNENEMY,
        NUM_SM_STATES,
    };
    Necromancer();
    ~Necromancer();
    bool Update(double dt);
    void Init();
    void SetWeapon(Weapon* weapon);
    Weapon* GetWeapon();
    bool getStunned();
    void makeEnemyStunned();
    void turnEnemy();
private:
    unsigned sCurrState;
    unsigned attackRange;
    float switchtime;
    bool moveleft = true;
    float leftdt = 0;
};