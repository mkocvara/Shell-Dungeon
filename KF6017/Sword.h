#pragma once
#include "MeleeWeapon.h"
#include <memory>

class Sword : public MeleeWeapon
{
	typedef MeleeWeapon Super;

public:
	Sword(const std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Sword();

private:
	static const int mBaseAttackDamage;
	static const double mBaseAttackDelay;
	static const float mBaseAttackReach;
	static const wchar_t* mAttackSpritePath;
	static const float mAttackSpriteScale;
};