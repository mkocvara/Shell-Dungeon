/*
* DungeonServiceManager.h
* 19002175
*
* Game-specific ServiceManager. Keeps a reference to all services and IO engines.
* Used to access all services by game objects via dependency injection.
* Creates game-specific services.
*/

#pragma once
#include "ServiceManager.h"

class DungeonServiceManager final : public ServiceManager
{
public:
	virtual ~DungeonServiceManager();

	// Create new instance using this method;
	// Can only be created as a shared_ptr, enforces ownership,
	// prevents nullptr return from ServiceManager::GetSharedPtrFromThis()
	// without having a std::shared_ptr owning the caller 
	// (i.e. from a non-pointer variable)
	[[nodiscard]] static std::shared_ptr<DungeonServiceManager> Create();

	typedef ServiceManager Super;

	virtual ErrorType StartServices(bool fullScreen, HWND hwnd, HINSTANCE hinstance) override;

private:
	DungeonServiceManager();
};

