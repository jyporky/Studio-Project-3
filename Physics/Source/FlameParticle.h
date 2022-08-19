#pragma once
#include "Entity.h"
class FlameParticle :
    public Entity
{
public:
    FlameParticle();
    ~FlameParticle();
    virtual bool Update(double dt);
    void SetBullet(float bulletspeed, unsigned damage, bool pen, float range, Vector3 direction);
    unsigned GetDamage();
    bool GetPenetrationValue();
    void HalfRange();
private:
    Vector3 direction;
    float bulletspeed;
    unsigned damage;
    bool penetrationValue;
    unsigned numOfEntitiesHit;
    float distanceTravelled;
    float range;
};

