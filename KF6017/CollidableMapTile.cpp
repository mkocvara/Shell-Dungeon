#include "CollidableMapTile.h"
#include "Shapes.h"

#include "ServiceManager.h"
#include "MyDrawEngine.h"


// PUBLIC

CollidableMapTile::CollidableMapTile(std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	mObjectType = ObjectType::mapTileCollidable;
}

CollidableMapTile::~CollidableMapTile()
{
}

std::weak_ptr<IShape2D> CollidableMapTile::GetShape() const
{
	return mpBoundingShape;
}

void CollidableMapTile::HandleCollision(std::shared_ptr<GameObject> pOtherObject)
{
	// No need to do anything here
}

PictureIndex CollidableMapTile::SetRenderSprite(const wchar_t* imagePath)
{
	Super::SetRenderSprite(imagePath);
	const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<Rectangle2D>();
	mpBoundingShape->PlaceAt(mPosition, (float)spriteHeight * mRenderScale, (float)spriteWidth * mRenderScale);
	// Note: Shape doesn't need to be placed on Update, as a map tile does not move.
	
	return mRenderSprite;
}
