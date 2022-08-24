#include "ShieldEnemy.h"



ShieldEnemy::ShieldEnemy()
{
    health = 69;
    redTimer = 0;
    movementSpeed = 15;
    energyDropped = 2;
    moneyDropped = 15;
    attackDamage = 5;
    affectedByKnockback = true;
    gameobject = nullptr;
    PlayerPointer = nullptr;
    CurrWeapon = nullptr;
    sCurrState = CHASE;
    enemytype = SHIELDMAN;
    attackRange = 10;
    attackSpeed = 1.5;
    angle = 0;
    iFrameTimer = 0;
    shieldturningrate = 90;
    isStunned = false;
}

ShieldEnemy::~ShieldEnemy()
{
}

Vector3 RotateVector2(const Vector3& vec, float radian)
{
    return Vector3(vec.x * cos(radian) + vec.y * -sin(radian),
        vec.x * sin(radian) + vec.y * cos(radian), 0.f);
}

bool ShieldEnemy::Update(double dt)
{
    if (angle > 360)
        angle -= 360;
    else if (angle < 0)
        angle += 360;
    if (iFrameTimer > 0)
    {
        iFrameTimer -= dt;
    }
    //check if the enemy is dead
    if (health <= 0)
    {
        cSoundController->StopPlayByID(4);
        cSoundController->PlaySoundByID(4);
        return true;
    }

    if (redTimer > 0)
    {
        gameobject->color.Set(1, 0, 0);
        redTimer -= dt;
    }
    else
        gameobject->color.Set(1, 1, 1);

    if (kbTimer > 0)
    {
        gameobject->direction -= kbEffect;
        kbEffect -= dt;
    }

    if (greenTimer > 0)
    {
        gameobject->color.Set(0, 1, 0);
        greenTimer -= dt;
    }
    else if (redTimer <= 0)
        gameobject->color.Set(1, 1, 1);

    if (isStunned)
        sCurrState = IDLE;

    Entity* Target = PlayerPointer;
    if (turned)
    {
        //find an enemy to target
    }
    Vector3 enemy2player = (PlayerPointer->GetGameObject()->pos - gameobject->pos).Normalize();
    Vector3 enemy2shield = (CurrWeapon->GetGameObject()->pos - gameobject->pos).Normalize();
    enemy2player.z = 0;
    enemy2shield.z = 0;
    if (enemy2player == Vector3(0, 0, 0))
    {
        enemy2player = Vector3(1, 0, 0);
    }
    float tempangle;
    tempangle = Math::RadianToDegree(atan2(enemy2player.y, enemy2player.x) - atan2(enemy2shield.y, enemy2shield.x));

    if (tempangle > 180)
        tempangle -= 360;
    else if (tempangle < -180)
        tempangle += 360;
    if (tempangle < 3 && tempangle > -3)
    {
        tempangle = 0;
    }
    else
    {
        if (tempangle > 0)
            tempangle = 1;
        else if (tempangle < 0)
            tempangle = -1;
        else
            tempangle = 0;
    }
    angle += tempangle * dt * shieldturningrate;

    if ((angle >= 0 && angle <= 90) || (angle >= 270 && angle <= 360))
    {
        gameobject->angle = 0;
    }
    else
    {
        gameobject->angle = 180;
    }

    //ai of the enemy
    switch (sCurrState)
    {
    case IDLE:
        break;
    case CHASE:
        //chase the player
        gameobject->pos += (Target->GetGameObject()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
        if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
        {
            sCurrState = ATTACK;
        }
        break;
    case ATTACK:
        if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() > attackRange * attackRange)
            sCurrState = CHASE;
        //check if the enemy is facing the player
        if (enemy2player.Dot(enemy2shield) < 0)
            break;


        //Attack the player
        if (CurrWeapon->attack())
        {
            //deal damage to the player
            if (PlayerPointer->iFrame == false)
            {
                PlayerPointer->ChangeHealth(-attackDamage);
            }
        }
        break;
    }

    if (kbTimer > 0)
    {
        gameobject->pos -= kbEffect;
        kbTimer -= dt;
    }
    gameobject->pos.z = 0;
    Vector3 direction = Vector3(1, 0, 0);
    direction = RotateVector2(direction, Math::DegreeToRadian(angle));
    //move the shield
    CurrWeapon->Update(dt, direction, Vector3(0,0,0), gameobject);
    
	return false;
}

void ShieldEnemy::Init()
{
    PlayerPointer = Player::GetInstance();
}

void ShieldEnemy::SetWeapon(Weapon* weapon)
{
    CurrWeapon = weapon;
}

Weapon* ShieldEnemy::GetWeapon()
{
	return CurrWeapon;
}
bool ShieldEnemy::getStunned() {
    return isStunned;
}
void ShieldEnemy::makeEnemyStunned() {
    isStunned = true;
}

