/*
* MeleeWeapon.h
* 19002175
*
* Abstract class for a melee weapons. Implements specifics of creating Attacks. Stats must be given it.
*/

#pragma once
#include "Weapon.h"

class MeleeWeapon : public Weapon
{
	typedef Weapon Super;

public:
	virtual ~MeleeWeapon();

	virtual void Attack(const std::shared_ptr<Creature> pAttacker, float attackAngle) override;

protected:
	MeleeWeapon(const std::weak_ptr<ServiceManager> pServiceManager, int baseAttackDamage, double baseAttackDelay, float baseAttackReach, float attackSpriteScale = 1.f, PictureIndex attackSprite = -1);
};

