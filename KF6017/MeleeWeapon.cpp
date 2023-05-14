#include "MeleeWeapon.h"

#include "ServiceManager.h"
#include "GameObjectFactory.h"
#include "mydrawengine.h"

#include "Attack.h"


MeleeWeapon::MeleeWeapon(const std::weak_ptr<ServiceManager> pServiceManager, int baseAttackDamage, double baseAttackDelay, float baseAttackReach, float attackSpriteScale, PictureIndex attackSprite)
	: Super(pServiceManager, baseAttackDamage, baseAttackDelay, baseAttackReach, attackSpriteScale, attackSprite)
{
}


// PUBLIC

MeleeWeapon::~MeleeWeapon()
{
}

void MeleeWeapon::Attack(const std::shared_ptr<GameObject> pAttacker, float attackAngle)
{
	const std::shared_ptr<ServiceManager> pServiceManager = mpServiceManager.lock();
	const std::shared_ptr<GameObjectFactory> pObjectFactory = pServiceManager->GetObjectFactory().lock();
	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManager->GetDrawEngine().lock();

	int height, width;
	pDrawEngine->GetDimensions(GetAttackSprite(), height, width);

	const std::shared_ptr<GameObject> pAttack = pObjectFactory->Create(ObjectType::attack, mpServiceManager, false).lock();
	const std::shared_ptr<class Attack> pAsAttack = std::static_pointer_cast<class Attack>(pAttack);
	Vector2D reachOffset;
	reachOffset.setBearing(attackAngle, GetBaseAttackReach() - height/2); // position is in the centre of the image, so this means the far edge is as far as range demands
	const Vector2D spawnPosition = pAttacker->GetPosition() + reachOffset;
	pAsAttack->Initialise(spawnPosition, attackAngle, GetBaseAttackSpriteScale(), GetAttackSprite(), shared_from_this(), pAttacker, 0.f);
}

