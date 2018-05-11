#ifndef TOWERDEFENCE_GAME_MAP_MAP_H
#define TOWERDEFENCE_GAME_MAP_MAP_H

#include <unordered_map>
#include "HexRepresentation.h"
#include <queue>
#include <set>
/*
*
*
*	Using AXIAL coordinates!
*	Hexadecimal data mostly taken from: 
*	https://www.redblobgames.com/grids/hexagons/implementation.html 
*	https://www.redblobgames.com/grids/hexagons/
*
*
*
*/


namespace TowerDefence::Game::Map
{
	class MapInformation
	{
	private:
		struct CellInformation
		{
			bool blocksView = false;
			bool traversable = false;
			bool occupied = false;
			bool buildable = false;
		};
		std::unordered_map<Hex, CellInformation> cellMap;

		struct Range
		{
			int x = 0;
			int y = 0;

		} mapRange;

		void CreateMapFromFile(std::string filePath = "");

	public:
		MapInformation(int xRange = 10, int yRange = 10);
		MapInformation(std::string filePath);

		bool CheckIfCellIsBuildable(int xCord, int yCord) const;
		bool CheckIfCellIsTraversable(int xCord, int yCord) const;
		bool CheckIfCellIsOccupied(int xCord, int yCord) const;

	private:
		struct CellCost
		{
			Hex cell;
			int gCost = 0;	//Cost to get here
			int fCost = 0;	//Cost to get to goal from here

			CellCost() = delete;
			CellCost(int x, int y) : cell(x, y) {};
			CellCost(int x, int y, int gCost, int fCost) : cell(x, y), gCost(gCost), fCost(fCost) {  };
			CellCost(Hex cell, int gCost, int fCost) : cell(cell), gCost(gCost), fCost(fCost) {};

			inline int h() const { return gCost + fCost; };

			bool operator<(const CellCost &other)const
			{
				return this->h() < other.h();
			}
			bool operator>(const CellCost &other) const
			{
				return this->h() > other.h();
			}



			CellCost &operator=(const CellCost &other)
			{
				this->cell = other.cell;
				this->gCost = other.gCost;
				this->fCost = other.fCost;
				return *this;
			}



		};

		struct CellKey
		{
			std::size_t operator()(const CellCost &c) const
			{
				return std::hash<Hex>()(c.cell);
			}
		};

		inline void PruneCells(const std::set<CellCost> &closedList, std::vector<CellCost> &newHexes) const;
		inline void InsertNewCellsInOpenList(std::priority_queue<CellCost> &openList, std::vector<CellCost> &newHexes);
		inline Hex FindBestCellInOpenList(std::priority_queue<CellCost> &openList) const;
		inline std::vector<Hex> GetAllNeighboursOfCell(const Hex& cell) const;
		inline bool CheckIfHexIsInRange(const Hex& cell) const;

	public:

		bool CheckIfPathCanBeConstructedBetweenPoints(int xStart, int yStart, int xEnd, int yEnd);
	};

}

	
#endif