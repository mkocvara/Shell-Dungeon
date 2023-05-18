/*
* RenderLayer.h
* 19002175
*
* Static helper struct for determining z-index of rendered objects based on unified layers. 
* Can be expanded with more layers, current layers can be reordered.
*/

#pragma once

struct RenderLayer {
	static const int MapLayer = 0;
	static const int EnemyLayer = 5;
	static const int PlayerLayer = 10;
};