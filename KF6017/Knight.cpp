#include "Knight.h"
#include "Shapes.h"
#include "errorlogger.h"

#include "ServiceManager.h"
#include "GameObjectFactory.h"
#include "mydrawengine.h"
#include "myinputs.h"

#include "Weapon.h"


// PUBLIC

Knight::Knight(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	mObjectType = ObjectType::knight;
}

Knight::~Knight()
{
}

void Knight::Initialise(Vector2D position, float angle, float scale)
{
	scale *= mBaseSpriteScale;

	SetRenderSprite(mRenderSpritePath);
	SetShowHealthBar(true);

	const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<Rectangle2D>();
	mpBoundingShape->PlaceAt(position, (float)spriteHeight * scale, (float)spriteWidth * scale);

	SetMovementSpeed(8.f);
	SetTimeToFullSpeed(0.1f);
	SetTimeToStop(0.1f);

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
	if (pOtherObject->GetObjectType() == ObjectType::orc)
	{
		// TODO
		//Damage(1);
	}
}


// PROTECTED

void Knight::Attack()
{ 
	std::shared_ptr<Weapon> pWeapon = GetEquippedWeapon();
	pWeapon->Attack(shared_from_this(), mRotationAngle);
}


// PRIVATE

void Knight::HandleInputs(double deltaTime)
{
	std::shared_ptr<MyInputs> pInputs = mpServiceManager.lock()->GetInputs().lock();
	pInputs->SampleKeyboard();
	pInputs->SampleMouse();

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