/*
* Weapon.cpp
* 19002175
*
* Abstract class representing a weapon usable by creatures. Has to be constructed with stats.
* Specifics of how it creates Attacks are individual to types of weapons.
*/

#include "Weapon.h"
#include "ErrorLogger.h"


Weapon::Weapon(const std::weak_ptr<ServiceManager> pServiceManager, int baseAttackDamage, double baseAttackDelay, float baseAttackReach, float attackSpriteScale, PictureIndex attackSprite)
{
	mpServiceManager = pServiceManager;
	mAttackDamage = baseAttackDamage;
	mAttackDelay = baseAttackDelay;
	mAttackReach = baseAttackReach;
	mAttackSprite = attackSprite;
	mAttackSpriteScale = attackSpriteScale;
}


// PUBLIC

Weapon::~Weapon()
{
}

PictureIndex Weapon::GetAttackSprite() const
{
	return mAttackSprite;
}

int Weapon::GetBaseAttackDamage() const
{
	return mAttackDamage;
}

double Weapon::GetBaseAttackDelay() const
{
	return mAttackDelay;
}

float Weapon::GetBaseAttackReach() const
{
	return mAttackReach;
}

float Weapon::GetBaseAttackSpriteScale() const
{
	return mAttackSpriteScale;
}


// PROTECTED

void Weapon::SetAttackSprite(PictureIndex pictureIndex)
{
	mAttackSprite = pictureIndex;
}

void Weapon::SetBaseAttackDamage(int damage)
{
	mAttackDamage = damage;
}

void Weapon::SetBaseAttackDelay(double delay)
{
	mAttackDelay = delay;
}

void Weapon::SetBaseAttackReach(float reach)
{
	mAttackReach = reach;
}

void Weapon::SetBaseAttackSpriteScale(float scale)
{
	mAttackSpriteScale = scale;
}
