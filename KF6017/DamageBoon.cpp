/*
* DamageBoon.cpp
* 19002175
*
* Damage boon; increases Knight's damage dealt when applied.
*/

#include "DamageBoon.h"
#include "Knight.h"
#include "ErrorLogger.h"


// PUBLIC

DamageBoon::DamageBoon(const std::weak_ptr<Knight> pPlayerKnight, const int damageAmount)
	: Super(pPlayerKnight), mDamageAmount(damageAmount)
{
}

DamageBoon::~DamageBoon()
{
	Remove();
}

void DamageBoon::Apply()
{
	const std::shared_ptr<Knight> pPlayerKnight = mpPlayerKnight.lock();
	if (pPlayerKnight)
	{
		pPlayerKnight->ChangeBonusDamage(mDamageAmount);
	}
	else
	{
		ErrorLogger::Writeln(L"Bad reference to Knight in a Boon.");
	}
}

void DamageBoon::Remove()
{
	const std::shared_ptr<Knight> pPlayerKnight = mpPlayerKnight.lock();
	if (pPlayerKnight)
	{
		pPlayerKnight->ChangeBonusDamage(-mDamageAmount);
	}
	else
	{
		ErrorLogger::Writeln(L"Bad reference to Knight in a Boon.");
	}
}
