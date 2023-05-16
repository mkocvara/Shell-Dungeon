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

	virtual void Die() override;
};

