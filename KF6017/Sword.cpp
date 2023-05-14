#include "Sword.h"
#include "ErrorLogger.h"

#include "ServiceManager.h"
#include "mydrawengine.h"
#include "GameObjectFactory.h"

#include "Attack.h"

const int Sword::mBaseAttackDamage = 5;
const double Sword::mBaseAttackDelay = 0.5f;
const float Sword::mBaseAttackReach = 200.f;
const wchar_t* Sword::mAttackSpritePath = L"Assets/Attack_Effects/Small/small_0022.png";
const float Sword::mAttackSpriteScale = 0.9f;


// PUBLIC

Sword::Sword(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager, mBaseAttackDamage, mBaseAttackDelay, mBaseAttackReach, mAttackSpriteScale)
{
	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManager.lock()->GetDrawEngine().lock();

	const PictureIndex attackSprite = pDrawEngine->LoadPicture(mAttackSpritePath);
	SetAttackSprite(attackSprite);
}

Sword::~Sword()
{
}