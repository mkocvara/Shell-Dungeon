#pragma once
#include "Service.h"
#include <memory>

class ServiceManager;

class GameManager : public Service
{
	typedef Service Super;
	typedef int LevelId;

public:
	GameManager(std::shared_ptr<ServiceManager> pServiceManager);
	virtual ~GameManager();

	virtual void Update(double deltaTime);
	
	virtual void StartLevel(LevelId level) = 0;
	virtual void RestartLevel() = 0;
	virtual void EndLevel() = 0;

protected:
	std::weak_ptr<ServiceManager> mpServiceManager;
	LevelId mActiveLevelId = -1;

	virtual void Render() = 0;
};