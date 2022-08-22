#pragma once
#include "Entity.h"
#include "Vector3.h"
#include <stdio.h>			
#include <stdlib.h>			
#include <time.h>	
class Arrow :
    public Entity
{
public:
    Arrow();
    ~Arrow();
    virtual bool Update(double dt);
    void SetArrow(float arrowspeed, unsigned damage, bool pen, float range, Vector3 direction);
    unsigned GetDamage();
    bool GetPenetrationValue();
    int getCrit();
    void setCrit(int chance);
private:
    Vector3 direction;
    float arrowspeed;
    unsigned damage;
    bool penetrationValue;
    unsigned numOfEntitiesHit;
    float distanceTravelled;
    float range;
    int critChance;
};

