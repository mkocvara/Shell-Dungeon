#pragma once
#include "Boon.h"

class SpeedBoon : public Boon
{
	typedef Boon Super;

public:
	SpeedBoon(const std::weak_ptr<Knight> pPlayerKnight, const float speedAmount);
	virtual ~SpeedBoon();

	virtual void Apply() override;
	virtual void Remove() override;

private:
	float mSpeedAmount;
};


