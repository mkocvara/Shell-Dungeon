#include "Weapon.h"
#include "ErrorLogger.h"


Weapon::Weapon(const std::weak_ptr<ServiceManager> pServiceManager, PictureIndex attackSprite, int baseAttackDamage, double baseAttackDelay, float baseAttackReach)
{
	mpServiceManager = pServiceManager;
	mBaseAttackDamage = baseAttackDamage;
	mBaseAttackDelay = baseAttackDelay;
	mBaseAttackReach = baseAttackReach;
	mAttackSprite = attackSprite;
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