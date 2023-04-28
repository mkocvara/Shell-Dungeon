#include "Sword.h"
#include "ErrorLogger.h"

#include "ServiceManager.h"
#include "mydrawengine.h"
#include "GameObjectFactory.h"

#include "Attack.h"


// PUBLIC

Sword::Sword(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManager.lock()->GetDrawEngine().lock();

	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"Sword(); Failed to fetch draw engine.");
		return;
	}

	const PictureIndex attackSprite = pDrawEngine->LoadPicture(mAttackSpritePath);

	SetAttackSprite(attackSprite);
	SetBaseAttackDamage(mBaseAttackDamage);
	SetBaseAttackDelay(mBaseAttackDelay);
	SetBaseAttackReach(mBaseAttackReach);
}

Sword::~Sword()
{
}

void Sword::Attack(const std::shared_ptr<GameObject> pAttacker, float attackAngle)
{
	const std::shared_ptr<GameObjectFactory> pObjectFactory = mpServiceManager.lock()->GetObjectFactory().lock();
	if (!pObjectFactory)
	{
		ErrorLogger::Writeln(L"Sword::Attack(); Failed to fetch object factory.");
		return;
	}

	std::shared_ptr<GameObject> pAttack = pObjectFactory->Create(ObjectType::attack, mpServiceManager, false).lock();
	std::shared_ptr<class Attack> pAsAttack = std::static_pointer_cast<class Attack>(pAttack);
	Vector2D reachOffset;
	reachOffset.setBearing(attackAngle, GetBaseAttackReach());
	const Vector2D spawnPosition = pAttacker->GetPosition() + reachOffset;
	pAsAttack->Initialise(spawnPosition, attackAngle, 1.f, GetAttackSprite(), shared_from_this(), pAttacker, 0.f);
}