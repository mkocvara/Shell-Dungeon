#pragma once
#include "MovingGameObject.h"
#include <vector>

class SpaceRock : public MovingGameObject
{
public:
	SpaceRock();
	virtual ~SpaceRock();

	typedef MovingGameObject Super;

	virtual void Initialise();

protected:
	virtual void Move(float deltaTime) override;

private:
	std::vector<const wchar_t*> _renderSpritePaths = {
		L"Assets/rock1.bmp",
		L"Assets/rock2.bmp",
		L"Assets/rock3.bmp",
		L"Assets/rock4.bmp"
	};
};

