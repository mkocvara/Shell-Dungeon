#pragma once
#include "ServiceManager.h"

class AsteroidsServiceManager final : public ServiceManager
{
public:
	virtual ~AsteroidsServiceManager();

	// Create new instance using this method;
	// Can only be created as a shared_ptr, enforces ownership,
	// prevents nullptr return from ServiceManager::GetSharedPtrFromThis()
	// without having a std::shared_ptr owning the caller 
	// (i.e. from a non-pointer variable)
	[[nodiscard]] static std::shared_ptr<AsteroidsServiceManager> Create();

	typedef ServiceManager Super;

	virtual ErrorType StartServices(bool fullScreen, HWND hwnd, HINSTANCE hinstance) override;

private:
	AsteroidsServiceManager();
};

