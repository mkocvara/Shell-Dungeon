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

