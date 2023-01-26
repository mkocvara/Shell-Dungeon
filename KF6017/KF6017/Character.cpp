#include "Character.h"
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

Character::Character(int str, int agi, int hp, std::string name)
{
	strength = str;
	agility = agi;
	maxHitPoints = hp;
	hitPoints = maxHitPoints;
	this->name = name;
}

void Character::displayDetails()
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

int Character::getStrength()
{
	return strength;
}

int Character::getAgility()
{
	return agility;
}

int Character::getHitPoints()
{
	return hitPoints;
}

std::string Character::getName()
{
	return name;
}

bool Character::isDead()
{
	return hitPoints == 0;
}

void Character::fight(Character &enemy)
{
	enemy.takeDamage(strength - enemy.agility);
	if (!enemy.isDead())
		return

	takeDamage(enemy.strength - agility);
}

// PRIVATE

void Character::takeDamage(int dmg)
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