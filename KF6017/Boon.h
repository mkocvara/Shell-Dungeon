#pragma once
#include <memory>

class Knight;

class Boon
{
public:
	virtual ~Boon();

	// Must be implemented to give something (a bonus) to the Knight
	virtual void Apply() = 0;

	// Must be implemented to remove the boon's effects from the Knight
	virtual void Remove() = 0;

	void SetKnight(const std::weak_ptr<Knight> pPlayerKnight);

protected:
	Boon(const std::weak_ptr<Knight> pPlayerKnight);

	std::weak_ptr<Knight> mpPlayerKnight;
};

