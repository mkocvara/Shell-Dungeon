#pragma once
#include "GameManager.h"

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

protected:
	virtual void Render() override;

private:
	float mTimer = 0.f;
	GameState mGameState = GameState::dead;
};

