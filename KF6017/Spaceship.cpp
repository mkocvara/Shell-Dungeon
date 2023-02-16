#include "Spaceship.h"
#include "mydrawengine.h"

// PUBLIC

Spaceship::Spaceship() : Super()
{
}

void Spaceship::Initialise(Vector2D position, float angle)
{
	Super::Initialise(position, angle);

	MyDrawEngine* pDrawEngine = MyDrawEngine::GetInstance();

	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"Spaceship::Initialise(); MyDrawEngine instance is null");
		return;
	}

	sprite = pDrawEngine->LoadPicture(L"Assets/basic.bmp");

	if (!sprite)
	{
		ErrorLogger::Writeln(L"Spaceship::Initialise(); MyDrawEngine instance is null");
		return;
	}

	return;
}

void Spaceship::Update()
{
	if (!IsActive())
		return;

	Super::Update();

	Move();
}


// PROTECTED

void Spaceship::Move()
{
	position += velocity;
}

void Spaceship::Render()
{
	MyDrawEngine* pDrawEngine = MyDrawEngine::GetInstance();

	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"Spaceship::Render(); MyDrawEngine instance is null");
		return;
	}

	pDrawEngine->DrawAt(position, sprite);
}
