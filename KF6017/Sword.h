#pragma once
#include "Weapon.h"
#include <memory>

class Sword : public Weapon
{
	typedef Weapon Super;

public:
	Sword(const std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Sword();

	virtual void Attack(const std::shared_ptr<GameObject> pAttacker, float attackAngle) override;

private:
	const int mBaseAttackDamage = 5;
	const double mBaseAttackDelay = 1.0;
	const float mBaseAttackReach = 100.f;
	
	const wchar_t* mAttackSpritePath = L"Assets/Attack_Effects/Small/small_0022.png";
	const float mAttackSpriteScale = 0.8f;
};