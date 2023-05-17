#pragma once
#include <string>
#include <vector>
#include <functional>
#include "errortype.h"

class ServiceManager;

typedef unsigned int Colour;

struct MenuItem
{
public:
	//MenuItem(std::wstring text, std::function<void()> callback);
	MenuItem(std::wstring text, std::function<void()> callback, Colour colour);
	~MenuItem();

	std::wstring text;
	Colour colour;
	std::function<void()> callback;
};

class Menu
{
public:
	Menu(std::shared_ptr<ServiceManager> pServiceManager, std::wstring heading);
	Menu(std::shared_ptr<ServiceManager> pServiceManager, std::wstring heading, Colour colour);
	~Menu();

	void AddItem(std::wstring menuText, std::function<void()> callback);
	void AddItem(std::wstring menuText, std::function<void()> callback, Colour colour);
	void RemoveItem(int index);
	void RemoveItem(std::wstring menuText);
	ErrorType Update();
	ErrorType HandleInput();
	ErrorType Render();

private:
	std::wstring mHeading;
	Colour mHeadingColour;

	std::vector<MenuItem> mItemsList;
	int mCurrentItem = 0;

	std::weak_ptr<ServiceManager> mpServiceManager;
};