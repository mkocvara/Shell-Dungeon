/*
* ObjectType.h
* 19002175
*
* List of object types available in the game.
* A name should be added for each possible type of entity in your game.
*/

#pragma once


enum class ObjectType
{
	unknown, 
	knight,
	orc,
	attack,
	mapTile,
	mapTileCollidable
};