#pragma once
#include "MovingGameObject.h"
#include <vector>

class SpaceRock : public MovingGameObject
{
public:
	SpaceRock(std::weak_ptr<ServiceManager> serviceManager);
	virtual ~SpaceRock();

	typedef MovingGameObject Super;

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0, float scale = 1) override;

protected:
	//virtual void Move(double deltaTime) override;

private:
	std::vector<const wchar_t*> _renderSpritePaths = {
		L"Assets/rock1.bmp",
		L"Assets/rock2.bmp",
		L"Assets/rock3.bmp",
		L"Assets/rock4.bmp"
	};
};

