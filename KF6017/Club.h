#pragma once
#include "MeleeWeapon.h"
#include <memory>

class Club : public MeleeWeapon
{
	typedef MeleeWeapon Super;

public:
	Club(const std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Club();

private:
	static const int mBaseAttackDamage;
	static const double mBaseAttackDelay;
	static const float mBaseAttackReach;
	static const wchar_t* mAttackSpritePath;
	static const float mAttackSpriteScale;
};