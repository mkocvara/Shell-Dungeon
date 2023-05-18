/*
* Enemy.h
* 19002175
*
* An abstract subclass of Creature specifically for creatures hostile to the player.
* Can be hit by the Knight. It's death is transmitted as an event.
*/

#pragma once
#include "Creature.h"

class Enemy : public Creature
{
	typedef Creature Super;

public:
	virtual ~Enemy();

	virtual void HandleCollision(const std::shared_ptr<GameObject> pOtherObject) override;

protected:
	Enemy(const std::weak_ptr<ServiceManager> pServiceManager);

	virtual void ProcessHit(const std::shared_ptr<class Attack>& rpAttack);

	virtual void Die() override;
};

