#pragma once
#include "Enemy.h"

class ShieldEnemy : public Enemy
{
public:
    enum SHEILDSTATES {
        IDLE,
        CHASE,
        ATTACK,
    };
    ShieldEnemy();
    ~ShieldEnemy();
    bool Update(double dt);
    void Init();
    void SetWeapon(Weapon* weapon);
    Weapon* GetWeapon();
    float GetAngle()
    {
        return angle;
    }
    
    bool getStunned();
    void makeEnemyStunned();
    void resetEnemyStunned();
    void turnEnemy();
    void resetEnemyTurned();
private:
    unsigned sCurrState;
    unsigned attackRange;
    float angle;
    float shieldturningrate;
    float switchtime;
    bool moveleft = true;
    float leftdt = 0;
};

