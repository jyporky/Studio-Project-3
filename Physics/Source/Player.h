#pragma once
#include "Entity.h"
class Player : public Entity ,public CSingletonTemplate<Player>
{
public:
	Player();
	~Player();
private:

};

