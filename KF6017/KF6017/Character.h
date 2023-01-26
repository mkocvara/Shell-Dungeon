#pragma once

#include <string>

class Character
{
private:
	int strength,
		agility,
		maxHitPoints,
		hitPoints;
	std::string name;

public:
	Character(int str, int agi, int hp, std::string name);
	void displayDetails();
	int getStrength();
	int getAgility();
	int getHitPoints();
	std::string getName();
	bool isDead();
	void fight(Character &enemy);

private:
	void takeDamage(int dmg);
};

