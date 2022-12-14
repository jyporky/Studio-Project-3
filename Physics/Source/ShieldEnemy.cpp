#include "ShieldEnemy.h"



ShieldEnemy::ShieldEnemy()
{
    health = 69;
    maxhealth = health;
    redTimer = 0;
    movementSpeed = 15;
    energyDropped = 5;
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
    switchtime = 0;
    moveleft = true;
    leftdt = 0;
    turned = false;
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

    Target = nullptr;
    if (turned)
    {
        for (unsigned idx = 0; idx < m_enemyList.size(); idx++) {
            if (m_enemyList[idx] == this)
                continue;
            if (Target == nullptr) {
                Target = m_enemyList[idx];
                continue;
            }
            if ((m_enemyList[idx]->GetGameObject()->pos - gameobject->pos).LengthSquared() < (Target->GetGameObject()->pos - gameobject->pos).LengthSquared()) {
                Target = m_enemyList[idx];
            }
        }
    }
    else {
        Target = PlayerPointer;
    }
    if (Target) {
        Vector3 enemy2player = (Target->GetGameObject()->pos - gameobject->pos).Normalize();
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
            if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() >= attackRange * attackRange)
                sCurrState = CHASE;
            else if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
            {
                sCurrState = ATTACK;
            }
            break;
        case CHASE:
        {
            //chase the player
            leftdt += dt;
            Vector3 direction;
            direction.SetZero();
            direction = (Target->GetGameObject()->pos - gameobject->pos).Normalize();
            if (moveleft)
                direction += Vector3(-direction.y, direction.x, 0);
            else
                direction -= Vector3(-direction.y, direction.x, 0);

            if (leftdt > switchtime)
            {
                leftdt = 0;
                moveleft = !moveleft;
            }

            gameobject->pos += direction * dt * movementSpeed;
            if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
            {
                sCurrState = ATTACK;
            }
            break; 
        }
        case ATTACK:
            if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() > attackRange * attackRange)
                sCurrState = CHASE;
            //check if the enemy is facing the player
            if (enemy2player.Dot(enemy2shield) < 0)
                break;


            //Attack the player
            if (CurrWeapon->attack())
            {
                if (turned) {
                    Target->ChangeHealth(-attackDamage);
                }
                else {
                    if (PlayerPointer->iFrame == false)
                    {
                        if(cGameManager->isImmortal)
                           PlayerPointer->ChangeHealth(attackDamage);
                        else if(cGameManager->isImmortal != true)
                           PlayerPointer->ChangeHealth(-attackDamage);
                    }
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
        //move the shield
        Vector3 direction = Vector3(1, 0, 0);
        direction = RotateVector2(direction, Math::DegreeToRadian(angle));
        //move the shield
        CurrWeapon->Update(dt, direction, Vector3(0, 0, 0), gameobject);
    }
	return false;
}

void ShieldEnemy::Init()
{
    switchtime = Math::RandFloatMinMax(0.5f, 8.0f);
    //get the revelant pointer
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
void ShieldEnemy::resetEnemyStunned() {
    isStunned = false;
}
void ShieldEnemy::turnEnemy() {
    turned = true;
}

void ShieldEnemy::resetEnemyTurned()
{
    if(turned)
      turned = false;
}
