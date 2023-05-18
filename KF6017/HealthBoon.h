/*
* HealthBoon.h
* 19002175
*
* Health boon; increases Knight's health when applied.
*/

#pragma once
#include "Boon.h"

class HealthBoon :  public Boon
{
	typedef Boon Super;

public:
	HealthBoon(const std::weak_ptr<Knight> pPlayerKnight, const int healthAmount);
	virtual ~HealthBoon();

	virtual void Apply() override;
	virtual void Remove() override;

private:
	int mHealthAmount;
};

