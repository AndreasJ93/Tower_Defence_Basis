#include "Map.h"
#include <iostream>


int main()
{
	
	auto map = TowerDefence::Game::Map::MapInformation();

	bool temp = map.CheckIfPathCanBeConstructedBetweenPoints(0, 0, 5, 5);

	std::cin.get();

	return 0;
}