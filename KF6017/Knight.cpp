#include "Knight.h"
#include "Shapes.h"
#include "errorlogger.h"

#include "ServiceManager.h"
#include "GameObjectFactory.h"
#include "mydrawengine.h"
#include "myinputs.h"

// PUBLIC

Knight::Knight(std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager, -0.95f)
{
}

Knight::~Knight()
{
}

void Knight::Initialise(Vector2D position, float angle, float scale)
{
	SetRenderSprite(mRenderSpritePath);

	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<Rectangle2D>();
	mpBoundingShape->PlaceAt(position, (float)spriteHeight, (float)spriteWidth);

	scale *= mBaseSpriteScale;

	Super::Initialise(position, angle, scale);
	return;
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

void Knight::HandleCollision(std::shared_ptr<GameObject> pOtherObject)
{
	/*if (pOtherObject->GetObjectType() == ObjectType::spacerock)
	{
		Die();
	}*/
}


// PROTECTED

void Knight::HandleInputs(double deltaTime)
{
	std::shared_ptr<MyInputs> pInputs = mpServiceManager.lock()->GetInputs().lock();
	pInputs->SampleKeyboard();
	pInputs->SampleMouse();

	// MOVEMENT
	// Rotation
	/*short rotDir = 0;
	if (pInputs->KeyPressed(DIK_LEFT) || pInputs->KeyPressed(DIK_A))
		rotDir = -1;
	else if (pInputs->KeyPressed(DIK_RIGHT) || pInputs->KeyPressed(DIK_D))
		rotDir = +1;*/

	// Movement
	//if (pInputs->KeyPressed(DIK_UP) || pInputs->KeyPressed(DIK_W))
	//{
	//	// accelerate
	//	Vector2D a;
	//	a.setBearing(mRotationAngle, mAcceleration);
	//	mVelocity += a;
	//}

	// Attacking
	if (pInputs->IfMouseNewLeftDown())
	{
		Attack();
	}
}

void Knight::Attack()
{
	
}

void Knight::Die()
{
	Remove();
}
