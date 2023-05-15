#include "Enemy.h"

#include "ObjectType.h"
#include "Attack.h"


Enemy::Enemy(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
}


// PUBLIC

Enemy::~Enemy()
{
}

void Enemy::HandleCollision(const std::shared_ptr<GameObject> pOtherObject)
{
	if (pOtherObject->GetObjectType() == ObjectType::attack)
	{
		const std::shared_ptr<class Attack> pAttack = std::static_pointer_cast<class Attack>(pOtherObject);
		const std::shared_ptr<GameObject> pAttacker = pAttack->GetAttacker();
		const std::shared_ptr<GameObject> pThis = shared_from_this();

		if (pAttacker->GetObjectType() == ObjectType::knight && !pAttack->HasEntityBeenHit(pThis))
		{
			Damage(pAttack->GetDamage());
			pAttack->AddHitEntity(pThis);
		}
	}

	Super::HandleCollision(pOtherObject);
}

