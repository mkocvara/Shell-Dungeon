#pragma once
#include "ServiceManager.h"

class AsteroidsServiceManager : public ServiceManager
{
	typedef ServiceManager Super;

	virtual ErrorType StartServices(bool bFullScreen, HWND hwnd, HINSTANCE hinstance) override;
};

