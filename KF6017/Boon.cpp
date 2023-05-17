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
