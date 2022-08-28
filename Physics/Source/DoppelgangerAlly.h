#pragma once
#include "Enemy.h"
#include <vector>
class DoppelgangerAlly :
    public Entity
{
public:
    enum DOPPELGANGER_STATES {
        IDLE,
        CHASE,
        ATTACK,
        NUM_SM_STATES,
    };
    DoppelgangerAlly();
    ~DoppelgangerAlly();
    bool Update(double dt);
    void Init();
    void SetWeapon(Weapon* weapon);
    void SetEnemyVector(std::vector<Entity*> m_enemyList);
    Weapon* GetWeapon();
    bool ChangeHealth(int changeInHealth);
    virtual unsigned GetMaxHealth()
    {
        return maxhealth;
    }
    virtual unsigned GetHealth()
    {
        return health;
    }

private:
    std::vector<Entity*> m_enemyList;
    unsigned sCurrState;
    unsigned attackRange;
    int health;
    float redTimer;
    int movementSpeed;
    int attackDamage;
    float attackSpeed;
    float iFrameTimer;
    
protected:
    unsigned maxhealth;
};