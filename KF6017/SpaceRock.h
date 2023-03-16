#pragma once
#include "MovingGameObject.h"
#include "ICollidableObject.h"
#include <vector>
#include <memory>

class Circle2D;

class SpaceRock : public MovingGameObject, public ICollidableObject
{
public:
	SpaceRock(std::weak_ptr<ServiceManager> serviceManager);
	virtual ~SpaceRock();

	typedef MovingGameObject Super;

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0, float scale = 1) override;
	virtual ErrorType Update(double deltaTime) override;

	virtual std::weak_ptr<IShape2D> GetShape() const override;
	virtual void HandleCollision(std::shared_ptr<GameObject> otherObject) override;

protected:
	//virtual void Move(double deltaTime) override;
	std::shared_ptr<Circle2D> boundingShape;

private:
	std::vector<const wchar_t*> renderSpritePaths = {
		L"Assets/rock1.bmp",
		L"Assets/rock2.bmp",
		L"Assets/rock3.bmp",
		L"Assets/rock4.bmp"
	};
};

