#pragma once
#include "GameManager.h"
#include "vector2D.h"

enum class GameState {
	alive,
	dead
};

class AsteroidsGameManager final : public GameManager 
{
	typedef GameManager Super;
	typedef int LevelId;

public:
	AsteroidsGameManager(std::shared_ptr<ServiceManager> pServiceManager);
	virtual ~AsteroidsGameManager();

	virtual void Update(double deltaTime) override;

	virtual void StartLevel(LevelId level) override;
	virtual void RestartLevel() override;
	virtual void EndLevel() override;

	virtual void HandleEvent(const Event& rEvent) override;

protected:
	virtual void Render() override;

private:
	int mAsteroidsLeft = 0;
	float mTimer = 0.f;
	GameState mGameState = GameState::dead;
	Vector2D mLastClickLocation;
};

