/*
* HealthBoon.cpp
* 19002175
*
* Health boon; increases Knight's health when applied.
*/

#include "HealthBoon.h"
#include "Knight.h"
#include "ErrorLogger.h"


// PUBLIC

HealthBoon::HealthBoon(const std::weak_ptr<Knight> pPlayerKnight, const int healthAmount)
	: Super(pPlayerKnight), mHealthAmount(healthAmount)
{
}

HealthBoon::~HealthBoon()
{
	Remove();
}

void HealthBoon::Apply()
{
	const std::shared_ptr<Knight> pPlayerKnight = mpPlayerKnight.lock();
	if (pPlayerKnight)
	{
		pPlayerKnight->SetMaxHealth(pPlayerKnight->GetMaxHealth() + mHealthAmount);
		pPlayerKnight->SetCurrentHealth(pPlayerKnight->GetCurrentHealth() + mHealthAmount);
	}
	else
	{
		ErrorLogger::Writeln(L"Bad reference to Knight in a Boon.");
	}
}

void HealthBoon::Remove()
{
	const std::shared_ptr<Knight> pPlayerKnight = mpPlayerKnight.lock();
	if (pPlayerKnight)
	{
		pPlayerKnight->SetMaxHealth(pPlayerKnight->GetMaxHealth() - mHealthAmount);
	}
	else
	{
		ErrorLogger::Writeln(L"Bad reference to Knight in a Boon.");
	}
}
