#include "Map.h"
#include <algorithm>
#include <string>


using namespace TowerDefence::Game::Map;

void MapInformation::CreateMapFromFile(std::string filePath)
{
}

MapInformation::MapInformation(int xRange, int yRange)
	: mapRange({ xRange, yRange })
{

	for (int x = -xRange; x <= xRange; x++)
		for (int y = -yRange; y <= yRange; y++)
		{
			cellMap.emplace(Hex(x, y), CellInformation());
		}
	
}

MapInformation::MapInformation(std::string filePath)
{
	CreateMapFromFile(filePath);
}

bool MapInformation::CheckIfCellIsBuildable(int xCord, int yCord) const
{
	if (const auto element = cellMap.find(Hex(xCord, yCord)); cellMap.end() != element)
	{
		return !(element->second.buildable) ? false :
			element->second.occupied ? false : true;
	}
	return false;
}

bool MapInformation::CheckIfCellIsTraversable(int xCord, int yCord) const
{
	if (const auto element = cellMap.find(Hex(xCord, yCord)); cellMap.end() != element)
	{
		return !(element->second.traversable) ? false : element->second.occupied;
	}
	return false;
}

bool MapInformation::CheckIfCellIsOccupied(int xCord, int yCord) const
{
	if (const auto element = cellMap.find(Hex(xCord, yCord)); cellMap.end() != element)
	{
		return element->second.occupied;
	}
	return false;
}

inline void MapInformation::PruneCells(const std::set<CellCost>& closedList, std::vector<CellCost>& newHexes) const
{
	std::vector<CellCost> replacementVec;
	for (CellCost cell : newHexes)
	{
		if (closedList.find(cell) == closedList.end())
			replacementVec.push_back(cell);
	}

	newHexes = replacementVec;
}

inline void MapInformation::InsertNewCellsInOpenList(std::priority_queue<CellCost>& openList, std::vector<CellCost>& newHexes)
{
	for (CellCost toInsert : newHexes)
		openList.push(toInsert);
}

inline Hex MapInformation::FindBestCellInOpenList(std::priority_queue<CellCost>& openList) const
{
	Hex toReturn = openList.top().cell;
	openList.pop();
	return toReturn;
}

inline std::vector<Hex> MapInformation::GetAllNeighboursOfCell(const Hex & cell) const
{
	std::vector<Hex> toReturn;

	std::vector<Hex> beforeRangeCheck = HexHelper::FindAllHexesInRange(cell, 1);

	for (Hex cellToCheck : beforeRangeCheck)
		if (CheckIfHexIsInRange(cellToCheck))
			toReturn.push_back(cellToCheck);

	return toReturn;
}

inline bool TowerDefence::Game::Map::MapInformation::CheckIfHexIsInRange(const Hex & cell) const
{
	if(cell.q() <= -mapRange.x)
		return false;
	if (cell.q() >= mapRange.x)
		return false;
	if (cell.r() <= -mapRange.y)
		return false;
	if (cell.r() >= mapRange.y)
		return false;

	return true;
}

bool MapInformation::CheckIfPathCanBeConstructedBetweenPoints(int xStart, int yStart, int xEnd, int yEnd)
{
	if (xStart == xEnd && yStart == yEnd)
		return false;

	std::set<CellCost> closedList;
	std::priority_queue<CellCost> openList;

	Hex endCell(xEnd, yStart);

	CellCost current(xStart, yStart);
	current.fCost = endCell.DistanceToHex(current.cell);
	current.gCost = 0;

	while (endCell != current.cell)
	{
		std::vector<Hex> neighbours = GetAllNeighboursOfCell(current.cell);
		std::vector<CellCost> newCells;

		for (Hex cell : neighbours)
			newCells.push_back({ cell, current.gCost, endCell.DistanceToHex(cell) });
		
		PruneCells(closedList, newCells);

		InsertNewCellsInOpenList(openList, newCells);

		while (!openList.empty() && closedList.find(openList.top()) != closedList.end())
			openList.pop();

		if (openList.empty())
			return false;

		current = openList.top();
		closedList.emplace(current);
		openList.pop();
		
	}
	
	return true;
}
