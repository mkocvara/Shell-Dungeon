#include "Spaceship.h"
#include "winerror.h"
#include "myinputs.h"
#include "errorlogger.h"

// PUBLIC

Spaceship::Spaceship() : Super()
{
}

Spaceship::~Spaceship()
{
}

void Spaceship::Initialise(Vector2D position, float angle, float scale)
{
	SetRenderSprite(renderSpritePath);
	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Spaceship::Update(float deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	Move(deltaTime);

	return SUCCESS;
}


// PROTECTED

void Spaceship::Move(float deltaTime)
{
	MyInputs* pInputs = MyInputs::GetInstance();
	pInputs->SampleKeyboard();

	// Rotation
	short rotDir = 0;
	if (pInputs->KeyPressed(DIK_LEFT) || pInputs->KeyPressed(DIK_A))
		rotDir = -1;
	else if (pInputs->KeyPressed(DIK_RIGHT) || pInputs->KeyPressed(DIK_D))
		rotDir = +1;

	rotationAngle += rotDir * rotationSpeed * deltaTime;

	// Movement
	if (pInputs->KeyPressed(DIK_UP) || pInputs->KeyPressed(DIK_W))
	{
		// accelerate
		Vector2D a;
		a.setBearing(rotationAngle, acceleration);
		velocity += a * deltaTime;
	}

	// apply friction
	Vector2D f = friction * velocity;
	velocity += f * deltaTime;

	position += velocity;
}
