#pragma once

#include <string>

class W1Character
{
private:
	int strength,
		agility,
		maxHitPoints,
		hitPoints;
	std::string name;

public:
	W1Character(int str, int agi, int hp, std::string name);
	void displayDetails();
	int getStrength();
	int getAgility();
	int getHitPoints();
	std::string getName();
	bool isDead();
	void fight(W1Character &enemy);

private:
	void takeDamage(int dmg);
};

