#pragma once
#include "GameObject.h"
#include <vector>

class Explosion final : public GameObject
{
public:
	Explosion(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Explosion();

	typedef GameObject Super;

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0, float scale = 1) override;
	virtual ErrorType Update(double deltaTime) override;

private:
	std::vector<const wchar_t*> mRenderSpritePaths = {
		L"Assets/explosion1.bmp",
		L"Assets/explosion2.bmp",
		L"Assets/explosion3.bmp",
		L"Assets/explosion4.bmp",
		L"Assets/explosion5.bmp",
		L"Assets/explosion6.bmp",
		L"Assets/explosion7.bmp",
		L"Assets/explosion8.bmp"
	};

	const float mAnimationLength = 1.f;
	double mElapsedTime = 0.f;
};

