/*
* Club.h
* 19002175
*
* Concrete subclass of a MeleeWeapon. Determines stats and the sprite of its attacks.
*/

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
	static const int mAttackDamage;
	static const double mAttackDelay;
	static const float mAttackReach;
	static const wchar_t* mAttackSpritePath;
	static const float mAttackSpriteScale;
};