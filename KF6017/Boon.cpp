/*
* Boon.cpp
* 19002175
*
* Abstract superclass for boons. Represents a buff (or potentially debuff) to be applied to the Knight.
* Prescribes a function to apply and remove the effect. Removes the effect on destruction.
*/

#include "Boon.h"


// PUBLIC

Boon::Boon(const std::weak_ptr<Knight> pPlayerKnight)
	: mpPlayerKnight(pPlayerKnight)
{
}

Boon::~Boon()
{
}

void Boon::SetKnight(const std::weak_ptr<Knight> pPlayerKnight)
{
	mpPlayerKnight = pPlayerKnight;
}
