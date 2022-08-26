#include "Entity.h"

float Entity::shieldblockingangle = 50;
CSoundController* Entity::cSoundController = CSoundController::GetInstance();

Entity::Entity()
{
	gameobject = nullptr;
	CurrWeapon = nullptr;
	SideWeapon = nullptr;
}

Entity::~Entity()
{
	if (CurrWeapon)
	{
		delete CurrWeapon;
		CurrWeapon = nullptr;
	}

	if (SideWeapon)
	{
		delete SideWeapon;
		SideWeapon = nullptr;
	}
}

bool Entity::Update(double dt)
{
	return false;
}

void Entity::Init()
{
}

void Entity::SetGameObject(GameObject* GO)
{
	gameobject = GO;
}

GameObject* Entity::GetGameObject()
{
	return gameobject;
}

bool Entity::ChangeHealth(int changeInHealth)
{
	return CurrWeapon;
}

bool Entity::ChangeHealth(int changeInHealth, Vector3 attackvec)
{
    ChangeHealth(changeInHealth);
    Vector3 direction;
    direction = attackvec - gameobject->pos;
    direction.Normalize();
    kbTimer = 0.5;
    kbEffect = direction;

    return true;
}

Weapon* Entity::GetWeapon()
{
	return CurrWeapon;
}

bool Entity::IsSpawningBullet()
{
	return false;
}

void Entity::SetWeapon(Weapon* weapon)
{
	CurrWeapon = weapon;
}

unsigned Entity::GetMoneyDrop()
{
	return 0;
}

unsigned Entity::GetEnergyDrop()
{
	return 0;
}

unsigned Entity::GetEnemyType()
{
	return 0;
}

float Entity::GetAngle()
{
	return 0.0f;
}

unsigned Entity::GetMaxHealth()
{
	return 0;
}

unsigned Entity::GetHealth()
{
	return 0;
}


bool Entity::CheckShieldCollision(Entity* projectile, Entity* shieldman)
{
    //check if the bullet is close enough
    if (projectile->GetGameObject()->pos.DistanceSquared(shieldman->GetGameObject()->pos) > (shieldman->GetWeapon()->GetRange() + shieldman->GetWeapon()->GetGameObject()->scale.x * 0.5f) * (shieldman->GetWeapon()->GetRange() + shieldman->GetWeapon()->GetGameObject()->scale.x * 0.5f))
        return false;

    Vector3 projectilevector = (projectile->GetGameObject()->pos - shieldman->GetGameObject()->pos).Normalize();
    Vector3 shieldman2shield = (shieldman->GetWeapon()->GetGameObject()->pos - shieldman->GetGameObject()->pos).Normalize();
    //check if the projectile is being blocked
    float angle, angle2;
    angle = atan2f(projectilevector.y, projectilevector.x);
    angle2 = atan2f(shieldman2shield.y, shieldman2shield.x);
    if (angle < 0)
        angle += Math::TWO_PI;

    if (angle2 < 0)
        angle2 += Math::TWO_PI;
    angle = Math::RadianToDegree(angle - angle2);
    if (angle <= shieldblockingangle * 0.5 && angle >= -shieldblockingangle * 0.5)
    {
        cSoundController->StopPlayByID(11);
        cSoundController->PlaySoundByID(11);
        return true;
    }
    return false;
}

bool Entity::getStunned() {
	return false;
}
void Entity::makeEnemyStunned() {
	return;
}
bool Entity::getTurnedState() {
	return false;
}
void Entity::turnEnemy() {
	return;
}
Entity* Entity::getTarget() {
	return Target;
}
bool Entity::IsSpawningSwordsman() {
	return false;
}