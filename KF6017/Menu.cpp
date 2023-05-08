#include "Menu.h"
#include "mydrawengine.h"
#include "myinputs.h"
#include "ServiceManager.h"


// PUBLIC

Menu::Menu(std::shared_ptr<ServiceManager> pServiceManager, std::wstring heading)
	: mpServiceManager(pServiceManager), mHeading(heading), mHeadingColour(MyDrawEngine::WHITE)
{
}

Menu::Menu(std::shared_ptr<ServiceManager> pServiceManager, std::wstring heading, Colour colour)
	: mpServiceManager(pServiceManager), mHeading(heading), mHeadingColour(colour)
{
}

Menu::~Menu()
{
}

void Menu::AddItem(std::wstring menuText, std::function<void()> callback)
{
	AddItem(menuText, callback, MyDrawEngine::GREY);
}

void Menu::AddItem(std::wstring menuText, std::function<void()> callback, Colour colour)
{
	mItemsList.push_back(MenuItem(menuText, callback, colour));
}

void Menu::RemoveItem(int index)
{
	mItemsList.erase(mItemsList.begin() + index);
}

void Menu::RemoveItem(std::wstring menuText)
{
	mItemsList.erase(std::remove_if(
		mItemsList.begin(),
		mItemsList.end(),
		[menuText](MenuItem item) { return item.text == menuText; }
	));
}

ErrorType Menu::Update()
{
	if (FAILED(HandleInput()))
		return FAILURE;
	
	if (FAILED(Render()))
		return FAILURE;

	return SUCCESS;
}

ErrorType Menu::HandleInput()
{
	std::shared_ptr<ServiceManager> mpServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<MyInputs> pInputs = mpServiceManagerLocked->GetInputs().lock();
	if (!pInputs)
		return FAILURE;

	// Get keyboard input
	if (pInputs->NewKeyPressed(DIK_UP))
	{
		mCurrentItem--;
	}
	if (pInputs->NewKeyPressed(DIK_DOWN))
	{
		mCurrentItem++;
	}
	if (mCurrentItem < 0)
	{
		mCurrentItem = 0;
	}
	else if (mCurrentItem >= (int)mItemsList.size())
	{
		mCurrentItem = mItemsList.size() - 1;
	}

	// User selects an option
	if (pInputs->NewKeyPressed(DIK_RETURN))
	{
		mItemsList[mCurrentItem].callback();
		mCurrentItem = 0; // ensures the top option is always selected by default when we reopen a menu
	}

	return SUCCESS;
}

ErrorType Menu::Render()
{
	std::shared_ptr<ServiceManager> mpServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManagerLocked->GetDrawEngine().lock();
	if (!pDrawEngine)
		return FAILURE;

	pDrawEngine->WriteText(450, 220, mHeading.c_str(), mHeadingColour);

	// Display the options
	for (unsigned int i = 0; i < mItemsList.size(); i++)
	{
		int colour = mItemsList[i].colour;
		if (i == mCurrentItem)
		{
			colour = MyDrawEngine::WHITE;
		}
		pDrawEngine->WriteText(450, 300 + 50 * i, mItemsList[i].text.c_str(), colour);
	}

	return SUCCESS;
}


//** MenuItem **//

// PUBLIC

MenuItem::MenuItem(std::wstring text, std::function<void()> callback, Colour colour)
	: text(text), callback(callback), colour(colour)
{
}

MenuItem::~MenuItem()
{
}
