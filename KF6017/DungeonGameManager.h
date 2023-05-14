#pragma once
#include "GameManager.h"
#include "vector2D.h"

class Knight;
class Rectangle2D;

enum class GameState {
	alive,
	dead
};

class DungeonGameManager final : public GameManager 
{
	typedef GameManager Super;
	typedef int LevelId;

public:
	DungeonGameManager(std::shared_ptr<ServiceManager> pServiceManager);
	virtual ~DungeonGameManager();

	virtual void Update(double deltaTime) override;

	virtual void StartLevel(LevelId level) override;
	virtual void RestartLevel() override;
	virtual void EndLevel() override;

	virtual void HandleEvent(const Event& rEvent) override;

	Vector2D GetPlayerLocation() const;
	std::weak_ptr<Rectangle2D> GetPlayerBounds() const;

protected:
	virtual void Render() override;

private:
	float mTimer = 0.f;
	GameState mGameState = GameState::dead;
	Vector2D mLastClickLocation;

	std::weak_ptr<Knight> mpPlayerKnight;
};

