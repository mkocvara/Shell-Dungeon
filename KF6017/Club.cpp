#include "Club.h"
#include "ErrorLogger.h"

#include "ServiceManager.h"
#include "mydrawengine.h"
#include "GameObjectFactory.h"

#include "Attack.h"


const int Club::mBaseAttackDamage = 3;
const double Club::mBaseAttackDelay = 1.8f;
const float Club::mBaseAttackReach = 90.f;
const wchar_t* Club::mAttackSpritePath = L"Assets/Attack_Effects/Small/small_0031.png";
const float Club::mAttackSpriteScale = 0.8f;

// PUBLIC

Club::Club(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager, mBaseAttackDamage, mBaseAttackDelay, mBaseAttackReach, mAttackSpriteScale)
{
	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManager.lock()->GetDrawEngine().lock();
	const PictureIndex attackSprite = pDrawEngine->LoadPicture(mAttackSpritePath);
	SetAttackSprite(attackSprite);
}

Club::~Club()
{
}