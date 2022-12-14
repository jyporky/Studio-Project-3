#pragma once
#include "Entity.h"
#include "Vector3.h"
class Bullet :
    public Entity
{
public:
    Bullet();
    ~Bullet();
    virtual bool Update(double dt);
    void SetBullet(float bulletspeed, unsigned damage, bool pen, float range, Vector3 direction);
    void SetBullet(float bulletspeed, unsigned damage, bool pen, float range, Vector3 direction, bool isExplosive, float explosionRad);
    unsigned GetDamage();
    bool GetPenetrationValue();
    bool isExplosive;
    float explosionRadius;
private:
    Vector3 direction;
    float bulletspeed;
    unsigned damage;
    bool penetrationValue;
    unsigned numOfEntitiesHit;
    float distanceTravelled;
    float range;
};

