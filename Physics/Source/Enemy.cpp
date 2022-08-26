#include "Enemy.h"

Player* Enemy::PlayerPointer = Player::GetInstance();
GameManger* Enemy::cGameManager = GameManger::GetInstance();
std::vector<Entity*> Enemy::m_enemyList;

Enemy::Enemy()
{
    health = 0;
    turned = false;
    redTimer = 0;
    movementSpeed = 0;
    energyDropped = 0;
    moneyDropped = 0;
    attackDamage = 0;
    affectedByKnockback = true;
    gameobject = nullptr;
    PlayerPointer = nullptr;
    attackSpeed = 0;
    iFrameTimer = 0;
    greenTimer = 0;
    kbTimer = 0;
    kbEffect.SetZero();
    isSpawningBullet = false;
    isStunned = false;
}

Enemy::~Enemy()
{
    
}

bool Enemy::Update(double dt)
{

    return false;
}

void Enemy::Init()
{
}

void Enemy::SetGameObject(GameObject* GO)
{
    gameobject = GO;
}

GameObject* Enemy::GetGameObject()
{
    return gameobject;
}

void Enemy::SetGO(GameObject* newGameObject)
{
    gameobject = newGameObject;
}

bool Enemy::IsSpawningBullet()
{
    return isSpawningBullet;
}

bool Enemy::ChangeHealth(int changeInHealth)
{
    if (changeInHealth < 0)
    {
        if (iFrameTimer > 0)
        {
            return false;
        }
        cSoundController->PlaySoundByID(3);
        redTimer = 0.5;
        iFrameTimer = 0.15;
    }
    else if (changeInHealth > 0)
        greenTimer = 0.5;

    health += changeInHealth;
    if (health <= 0)
    {
        cSoundController->StopPlayByID(4);
        cSoundController->PlaySoundByID(4);
        return true;
    }
    return true;
}

unsigned Enemy::GetMoneyDrop()
{
    return moneyDropped;
}

unsigned Enemy::GetEnergyDrop()
{
    return energyDropped;
}

unsigned Enemy::GetEnemyType()
{
    return enemytype;
}

float Enemy::GetAngle()
{
    return 0.0f;
}

void Enemy::SetWeapon(Weapon* weapon)
{
    CurrWeapon = weapon;
}

Weapon* Enemy::GetWeapon()
{
    return CurrWeapon;
}
bool Enemy::getStunned() {
    return false;
}
void Enemy::makeEnemyStunned() {
    return;
}
void Enemy::resetEnemyStunned() {
    return;
}
bool Enemy::getTurnedState() {
    return false;
}
void Enemy::turnEnemy() {
    return;
}
void Enemy::resetEnemyTurned()
{
    return;
}
void Enemy::SetEnemyVector(std::vector<Entity*> m_enemyList) {
    Enemy::m_enemyList = m_enemyList;
}
Entity* Enemy::getTarget() {
    return Target;
}
bool Enemy::IsSpawningSwordsman() {
    return false;
}