#include "Sword.h"

#include "ServiceManager.h"
#include "mydrawengine.h"

const int Sword::mAttackDamage = 5;
const double Sword::mAttackDelay = 0.5f;
const float Sword::mAttackReach = 200.f;
const wchar_t* Sword::mAttackSpritePath = L"Assets/Attack_Effects/Small/small_0022.png";
const float Sword::mAttackSpriteScale = 0.9f;


// PUBLIC

Sword::Sword(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager, mAttackDamage, mAttackDelay, mAttackReach, mAttackSpriteScale)
{
	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManager.lock()->GetDrawEngine().lock();

	const PictureIndex attackSprite = pDrawEngine->LoadPicture(mAttackSpritePath);
	SetAttackSprite(attackSprite);
}

Sword::~Sword()
{
}