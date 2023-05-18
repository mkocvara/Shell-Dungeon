/*
* SpeedBoon.cpp
* 19002175
*
* Speed boon; increases Knight's movement speed when applied.
*/

#include "SpeedBoon.h"
#include "Knight.h"
#include "ErrorLogger.h"


// PUBLIC

SpeedBoon::SpeedBoon(const std::weak_ptr<Knight> pPlayerKnight, const float speedAmount)
	: Super(pPlayerKnight), mSpeedAmount(speedAmount)
{
}

SpeedBoon::~SpeedBoon()
{
	Remove();
}

void SpeedBoon::Apply()
{
	const std::shared_ptr<Knight> pPlayerKnight = mpPlayerKnight.lock();
	if (pPlayerKnight)
	{
		pPlayerKnight->SetMovementSpeed(pPlayerKnight->GetMovementSpeed() + mSpeedAmount);
	}
	else
	{
		ErrorLogger::Writeln(L"Bad reference to Knight in a Boon.");
	}
}

void SpeedBoon::Remove()
{
	const std::shared_ptr<Knight> pPlayerKnight = mpPlayerKnight.lock();
	if (pPlayerKnight)
	{
		pPlayerKnight->SetMovementSpeed(pPlayerKnight->GetMovementSpeed() - mSpeedAmount);
	}
	else
	{
		ErrorLogger::Writeln(L"Bad reference to Knight in a Boon.");
	}
}

