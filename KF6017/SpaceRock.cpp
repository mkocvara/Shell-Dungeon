#include "SpaceRock.h"
#include "errorlogger.h"
#include "RandUtil.h"
#include "mydrawengine.h"


// PUBLIC

SpaceRock::SpaceRock() : Super()
{ 
}

SpaceRock::~SpaceRock()
{
}

void SpaceRock::Initialise()
{
	int randSprite = RandUtil::randRangeInt(0, _renderSpritePaths.size()-1);	
	float randAngle = RandUtil::randRangeDouble(0, 360);
	float randScale = RandUtil::randRangeDouble(0.6f, 1.f);
	Vector2D moveDir(RandUtil::randRangeDouble(0.f, 1.f), RandUtil::randRangeDouble(0.f, 1.f));
	float randSpeed = RandUtil::randRangeDouble(0.5f, 1.5f);
	velocity = moveDir * randSpeed;

	MyDrawEngine* pDrawEngine = MyDrawEngine::GetInstance();
	Vector2D randPos = pDrawEngine->theCamera.ReverseTransform(Vector2D(
		RandUtil::randRangeDouble(0.f, pDrawEngine->GetScreenWidth()), 
		RandUtil::randRangeDouble(0.f, pDrawEngine->GetScreenHeight())
	));

	SetRenderSprite(_renderSpritePaths[randSprite]);
	Super::Initialise(randPos, randAngle, randScale);
	return;
}


// PROTECTED

void SpaceRock::Move(double deltaTime)
{
	position += velocity;
}
