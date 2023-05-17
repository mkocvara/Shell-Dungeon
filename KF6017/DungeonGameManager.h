#pragma once
#include "GameManager.h"
#include "vector2D.h"
#include <memory>
#include <list>

class Knight;
class Rectangle2D;
class Level;
class Boon;

enum class GameState {
	playerAlive,
	playerDead,
	levelCleared,
	victory
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

	void RestartGame();

	GameState GetGameState() const;

	Vector2D GetPlayerLocation() const;
	std::weak_ptr<Rectangle2D> GetPlayerBounds() const;

protected:
	virtual void Render() override;
	bool ContinuePressed();
	bool HandleBoonSelection();
	bool BoonSelected();

	void ApplyBoons(const std::weak_ptr<Knight>& pPlayerKnight);

private:
	GameState mGameState = GameState::playerAlive;
	Vector2D mLastClickLocation;

	int mEnemiesRemaining = 0;

	double mTimer = 0.f;
	double mTimeAtLevelStart = 0.f;

	const int mNumLevels = 4; // TODO could be determined automatically from the Levels folder, but there're more important things to do

	std::shared_ptr<Level> mActiveLevel;
	std::weak_ptr<Knight> mpPlayerKnight;

	std::list<std::unique_ptr<Boon>> mSelectedBoons;
};

