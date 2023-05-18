/*
* DamageBoon.h
* 19002175
*
* Damage boon; increases Knight's damage dealt when applied.
*/

#pragma once
#include "Boon.h"

class DamageBoon : public Boon
{
	typedef Boon Super;

public:
	DamageBoon(const std::weak_ptr<Knight> pPlayerKnight, const int mDamageAmount);
	virtual ~DamageBoon();

	virtual void Apply() override;
	virtual void Remove() override;

private:
	int mDamageAmount;
};

