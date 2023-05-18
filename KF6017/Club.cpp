/*
* Club.cpp
* 19002175
*
* Concrete subclass of a MeleeWeapon. Determines stats and the sprite of its attacks.
*/

#include "Club.h"

#include "ServiceManager.h"
#include "mydrawengine.h"


const int Club::mAttackDamage = 3;
const double Club::mAttackDelay = 1.8f;
const float Club::mAttackReach = 90.f;
const wchar_t* Club::mAttackSpritePath = L"Assets/Attack_Effects/Small/small_0031.png";
const float Club::mAttackSpriteScale = 0.8f;

// PUBLIC

Club::Club(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager, mAttackDamage, mAttackDelay, mAttackReach, mAttackSpriteScale)
{
	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManager.lock()->GetDrawEngine().lock();
	const PictureIndex attackSprite = pDrawEngine->LoadPicture(mAttackSpritePath);
	SetAttackSprite(attackSprite);
}

Club::~Club()
{
}