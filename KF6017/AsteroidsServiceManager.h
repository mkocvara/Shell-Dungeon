#pragma once
#include "ServiceManager.h"

class AsteroidsServiceManager : public ServiceManager
{
public:
	AsteroidsServiceManager();
	virtual ~AsteroidsServiceManager();

	typedef ServiceManager Super;

	virtual ErrorType StartServices(bool bFullScreen, HWND hwnd, HINSTANCE hinstance) override;
};

