/*
* Enemy.cpp
* 19002175
*
* An abstract subclass of Creature specifically for creatures hostile to the player.
* Can be hit by the Knight. It's death is transmitted as an event.
*/

#include "Enemy.h"
#include "ObjectType.h"
#include "RenderLayers.h"
#include "Attack.h"

#include "ServiceManager.h"
#include "EventManager.h"


Enemy::Enemy(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	SetZIndex(RenderLayer::EnemyLayer);
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
			ProcessHit(pAttack);
			pAttack->AddHitEntity(pThis);
		}
	}

	Super::HandleCollision(pOtherObject);
}


// PROTECTED

void Enemy::ProcessHit(const std::shared_ptr<class Attack>& pAttack)
{
	Damage(pAttack->GetDamage());
}

void Enemy::Die()
{
	Super::Die();

	std::shared_ptr<EventManager> pEventManager = mpServiceManager.lock()->GetEventManager().lock();
	Event e(EventType::enemyKilled, mPosition, this);
	pEventManager->DispatchEvent(e);
}