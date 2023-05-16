#include "Knight.h"
#include "Shapes.h"
#include "errorlogger.h"
#include "RenderLayers.h"

#include "ServiceManager.h"
#include "GameObjectFactory.h"
#include "mydrawengine.h"
#include "myinputs.h"
#include "MousePointer.h"
#include "EventManager.h"

#include "Weapon.h"
#include "Attack.h"


// PUBLIC

Knight::Knight(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	mObjectType = ObjectType::knight;
	SetZIndex(RenderLayer::PlayerLayer);
}

Knight::~Knight()
{
}

void Knight::Initialise(Vector2D position, float angle, float scale)
{
	scale *= mBaseSpriteScale;

	SetRenderSprite(mRenderSpritePath);

	SetMaxHealth(mBaseMaxHealth);
	Heal(mBaseMaxHealth);
	SetShowHealthBar(true);

	const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<Rectangle2D>();
	mpBoundingShape->PlaceAt(position, (float)spriteHeight * scale, (float)spriteWidth * scale);

	SetMovementSpeed(8.f);
	SetTimeToFullSpeed(0.03f);
	SetTimeToStop(0.03f);

	Super::Initialise(position, angle, scale);
}

ErrorType Knight::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	mpBoundingShape->SetCentre(mPosition);

	HandleInputs(deltaTime);

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	return SUCCESS;
}

std::weak_ptr<IShape2D> Knight::GetShape() const
{
	return mpBoundingShape;
}

void Knight::HandleCollision(const std::shared_ptr<GameObject> pOtherObject)
{
	if (pOtherObject->GetObjectType() == ObjectType::attack)
	{
		const std::shared_ptr<class Attack> pAttack = std::static_pointer_cast<class Attack>(pOtherObject);
		const std::shared_ptr<GameObject> pAttacker = pAttack->GetAttacker();
		const std::shared_ptr<GameObject> pThis = shared_from_this();

		if (pAttacker->GetObjectType() == ObjectType::orc && !pAttack->HasEntityBeenHit(pThis))
		{
			Damage(pAttack->GetDamage());
			pAttack->AddHitEntity(pThis);
		}
	}

	Super::HandleCollision(pOtherObject);
}

std::weak_ptr<Rectangle2D> Knight::GetBounds() const
{
	return mpBoundingShape;
}


// PROTECTED

void Knight::Attack()
{ 
	if (IsAttackOnCooldown())
		return;

	const std::shared_ptr<MousePointer> pMousePointer = mpServiceManager.lock()->GetMousePointer().lock();
	Vector2D attackDirection = pMousePointer->GetPosition() - mPosition;

	const std::shared_ptr<Weapon> pWeapon = GetEquippedWeapon();
	pWeapon->Attack(shared_from_this(), attackDirection.angle());

	Super::Attack();
}

void Knight::Die()
{
	SetActive(false);

	std::shared_ptr<EventManager> pEventManager = mpServiceManager.lock()->GetEventManager().lock();
	Event e(EventType::playerDied, mPosition, this);
	pEventManager->DispatchEvent(e);
}


// PRIVATE

void Knight::HandleInputs(double deltaTime)
{
	std::shared_ptr<MyInputs> pInputs = mpServiceManager.lock()->GetInputs().lock();

	// Movement
	Vector2D walkDirection;

	if (pInputs->KeyPressed(DIK_UP) || pInputs->KeyPressed(DIK_W))
		walkDirection += Vector2D(0, 1);
	if (pInputs->KeyPressed(DIK_DOWN) || pInputs->KeyPressed(DIK_S))
		walkDirection += Vector2D(0, -1);
	if (pInputs->KeyPressed(DIK_LEFT) || pInputs->KeyPressed(DIK_A))
		walkDirection += Vector2D(-1, 0);
	if (pInputs->KeyPressed(DIK_RIGHT) || pInputs->KeyPressed(DIK_D))
		walkDirection += Vector2D(1, 0);

	mMoveDirection = walkDirection;

	// Attacking
	if (pInputs->IfMouseNewLeftDown())
	{
		Attack();
	}
}