#include "Weapon.h"
#include "ErrorLogger.h"


Weapon::Weapon(const std::weak_ptr<ServiceManager> pServiceManager, int baseAttackDamage, double baseAttackDelay, float baseAttackReach, float attackSpriteScale, PictureIndex attackSprite)
{
	mpServiceManager = pServiceManager;
	mBaseAttackDamage = baseAttackDamage;
	mBaseAttackDelay = baseAttackDelay;
	mBaseAttackReach = baseAttackReach;
	mAttackSprite = attackSprite;
	mBaseAttackSpriteScale = attackSpriteScale;
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
	return mBaseAttackDamage;
}

double Weapon::GetBaseAttackDelay() const
{
	return mBaseAttackDelay;
}

float Weapon::GetBaseAttackReach() const
{
	return mBaseAttackReach;
}

float Weapon::GetBaseAttackSpriteScale() const
{
	return mBaseAttackSpriteScale;
}


// PROTECTED

void Weapon::SetAttackSprite(PictureIndex pictureIndex)
{
	mAttackSprite = pictureIndex;
}

void Weapon::SetBaseAttackDamage(int damage)
{
	mBaseAttackDamage = damage;
}

void Weapon::SetBaseAttackDelay(double delay)
{
	mBaseAttackDelay = delay;
}

void Weapon::SetBaseAttackReach(float reach)
{
	mBaseAttackReach = reach;
}

void Weapon::SetBaseAttackSpriteScale(float scale)
{
	mBaseAttackSpriteScale = scale;
}
