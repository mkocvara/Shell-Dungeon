#pragma once
#include <memory>
#include "errortype.h"
#include "windows.h"

class ObjectManager;
class GameObjectFactory;
class MyDrawEngine;
class MySoundEngine;
class MyInputs;

class ServiceManager
{
public:
	virtual ~ServiceManager();

	virtual ErrorType StartServices(bool bFullScreen, HWND hwnd, HINSTANCE hinstance);

	std::weak_ptr<ObjectManager> GetObjectManager();
	std::weak_ptr<MyDrawEngine> GetDrawEngine();
	std::weak_ptr<MySoundEngine> GetSoundEngine();
	std::weak_ptr<MyInputs> GetInputs();

	// GameObjectFactor is abstract; this must be instantiated by a subclass
	std::weak_ptr<GameObjectFactory> GetObjectFactory();

protected:
	ServiceManager(); // abstract

	std::shared_ptr<ObjectManager> objectManager;
	std::shared_ptr<GameObjectFactory> objectFactory;
	std::shared_ptr<MyDrawEngine> drawEngine;
	std::shared_ptr<MySoundEngine> soundEngine;
	std::shared_ptr<MyInputs> inputs;
};

