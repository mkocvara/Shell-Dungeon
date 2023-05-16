#pragma once
#include "MapTile.h"
#include "ICollidableObject.h"

class Rectangle2D;

class CollidableMapTile : public MapTile, public ICollidableObject
{
	typedef MapTile Super;
	typedef int PictureIndex;

public:
	CollidableMapTile(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~CollidableMapTile();

	virtual std::weak_ptr<IShape2D> GetShape() const override;
	virtual void HandleCollision(std::shared_ptr<GameObject> pOtherObject) override;
	virtual PictureIndex SetRenderSprite(const wchar_t* imagePath) override;

private:
	std::shared_ptr<Rectangle2D> mpBoundingShape;
};