#include "W1Character.h"
#include <algorithm>

/*
	Character(int str, int agi, int hp, std::string name);
	void displayDetails();
	int getStrength();
	int getAgility();
	int getHitPoints();
	std::string getName();

private:
	void takeDamage();
};
*/

W1Character::W1Character(int str, int agi, int hp, std::string name)
{
	strength = str;
	agility = agi;
	maxHitPoints = hp;
	hitPoints = maxHitPoints;
	this->name = name;
}

void W1Character::displayDetails()
{
	printf("====================\n");
	printf("%s:\n", name.c_str());
	printf("----------\n");
	printf("Strength:\t%d\n", strength);
	printf("Agility:\t%d\n", agility);
	printf("Maximum HP:\t%d\n", maxHitPoints);
	printf("Current HP:\t%d\n", hitPoints);
	printf("====================\n\n");
}

int W1Character::getStrength()
{
	return strength;
}

int W1Character::getAgility()
{
	return agility;
}

int W1Character::getHitPoints()
{
	return hitPoints;
}

std::string W1Character::getName()
{
	return name;
}

bool W1Character::isDead()
{
	return hitPoints == 0;
}

void W1Character::fight(W1Character &enemy)
{
	enemy.takeDamage(strength - enemy.agility);
	if (!enemy.isDead())
		return

	takeDamage(enemy.strength - agility);
}

// PRIVATE

void W1Character::takeDamage(int dmg)
{
	hitPoints -= dmg;
	hitPoints = std::min(maxHitPoints, std::max(hitPoints, 0)); // clamp 0 - maxHitPoints

	printf("%s took %d damage.\n", name.c_str(), dmg);

	if (isDead())
	{
		printf("%s died!\n", name.c_str());
	}

	printf("\n");
}