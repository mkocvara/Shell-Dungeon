#pragma once
#include "Weapon.h"

class MeleeWeapon : public Weapon
{
	typedef Weapon Super;

public:
	virtual ~MeleeWeapon();

	virtual void Attack(const std::shared_ptr<GameObject> pAttacker, float attackAngle) override;

protected:
	MeleeWeapon(const std::weak_ptr<ServiceManager> pServiceManager, int baseAttackDamage, double baseAttackDelay, float baseAttackReach, float attackSpriteScale = 1.f, PictureIndex attackSprite = -1);
};

