#ifndef TOWERDEFENCE_GAME_MAP_HEXREPRESENTATION_H
#define TOWERDEFENCE_GAME_MAP_HEXREPRESENTATION_H

#include <cstdint>
#include <array>
#include <functional>
#include "AssertException.h"

namespace TowerDefence::Game::Map
{
	enum class MapDirections
	{
		NORTH_WEST,
		NORTH_EAST,
		EAST,
		SOUTH_EAST,
		SOUTH_WEST,
		WEST
	};

	enum class MapDiagonalDirections
	{
		NORTH_WEST,
		NORTH,
		NORTH_EAST,
		SOUTH_EAST,
		SOUTH,
		SOUTH_WEST
	};

	class FractionalHex;
	class Hex
	{
		//Store the coordinates in a {q, r, s} fashion (x,y,z)
		std::array<int16_t,3> centerCoordinates;

		static const std::vector<Hex> HexDirections;
		static const std::vector<Hex> HexDiagonal;
		
		inline int Length() {
			return static_cast<int>((
				std::abs(centerCoordinates[0]) +
				std::abs(centerCoordinates[1]) +
				std::abs(centerCoordinates[2])) / 2);
		}

		inline float lerp(float qDiff, float rDiff, float slope) const
		{
			return qDiff * (1.0f - slope) + rDiff * slope;
		}

		FractionalHex LerpTo(const Hex& other, float slope)const;

	public:
		Hex() : centerCoordinates{ 0,0,0 } {}
		Hex(int16_t q, int16_t r, int16_t s) : centerCoordinates{q, r, s} {
			throw_assert(q + r + s == 0, "Invalid coordinates passed into constructor") 
		}
		Hex(const Hex &other) : centerCoordinates{ other.centerCoordinates } {};
		Hex(int16_t q, int16_t r) : centerCoordinates{ q, r, -q - r } { }

		inline int16_t q() const { return centerCoordinates[0]; }
		inline int16_t r() const { return centerCoordinates[1]; }
		inline int16_t s() const { return centerCoordinates[2]; }

		bool operator==(const Hex &other) const
		{
			return	this->centerCoordinates[0] != other.centerCoordinates[0] ? false : 
					this->centerCoordinates[1] != other.centerCoordinates[1] ? false :
					this->centerCoordinates[2] != other.centerCoordinates[2] ? false :
					true;
		}
		bool operator!=(const Hex &other) const
		{
			return !(*this == other);
		}

		Hex operator+(const Hex &other) const
		{
			return Hex(
				this->centerCoordinates[0] + other.centerCoordinates[0],
				this->centerCoordinates[1] + other.centerCoordinates[1],
				this->centerCoordinates[2] + other.centerCoordinates[2]
				);
		}

		Hex operator-(const Hex &other) const
		{
			return Hex(
				this->centerCoordinates[0] - other.centerCoordinates[0],
				this->centerCoordinates[1] - other.centerCoordinates[1],
				this->centerCoordinates[2] - other.centerCoordinates[2]
			);
		}

		Hex operator*(int scale) const
		{
			return Hex(
				this->centerCoordinates[0] *scale,
				this->centerCoordinates[1] *scale,
				this->centerCoordinates[2] *scale
			);
		}
		
		Hex &operator=(const Hex &other)
		{
			centerCoordinates = { other.centerCoordinates };
			return *this;
		}

		Hex &operator=(Hex && other)
		{
			return *this;
		}

		inline int DistanceToHex(const Hex& targetHex) const
		{
			return (*this - targetHex).Length();
		}

		inline Hex HexDirection(MapDirections direction)
		{
			return HexDirections[static_cast<int>(direction)];
		}

		inline Hex HexDiagonalDirection(MapDiagonalDirections direction)
		{
			return HexDiagonal[static_cast<int>(direction)];
		}

		inline Hex NeighbourInDirection(MapDirections direction)
		{
			return *this + HexDirection(direction);
		}

		inline Hex NeighbourInDiagonal(MapDiagonalDirections direction)
		{
			return *this + HexDiagonalDirection(direction);
		}

		inline Hex RotateLeft()
		{
			return Hex(-centerCoordinates[2], -centerCoordinates[0], -centerCoordinates[1]);
		}
		inline Hex RotateRight()
		{
			return Hex(-centerCoordinates[1], -centerCoordinates[2], -centerCoordinates[0]);
		}

		inline FractionalHex operator*(float fractionalScale)const;


		inline std::vector<Hex> HexesOnTheLineToPoint(const Hex &other)const;

	};

	//FractionalHex is used to help with some conversions!
	class FractionalHex
	{
		//Store the coordinates in a {q, r, s} fashion (x,y,z)
		const std::array<float,3> centerCoordinates;

	public:
		FractionalHex(float q = 0, float r = 0, float s = 0) : centerCoordinates{ q, r, s } { 
			//small epsilon used for floating point errors
			throw_assert(std::abs(q + r + s) <= 0.000005f, "Invalid coordinates passed into constructor") 
		}
		FractionalHex(float q, float r) : centerCoordinates{ q, r, -q - r } { }

		inline float q() { return centerCoordinates[0]; }
		inline float r() { return centerCoordinates[1]; }
		inline float s() { return centerCoordinates[2]; }


		bool operator==(const FractionalHex &other) const
		{
			return	this->centerCoordinates[0] != other.centerCoordinates[0] ? false :
				this->centerCoordinates[1] != other.centerCoordinates[1] ? false :
				this->centerCoordinates[2] != other.centerCoordinates[2] ? false :
				true;
		}
		bool operator!=(const FractionalHex &other) const
		{
			return !(*this == other);
		}

		FractionalHex operator+(const FractionalHex &other) const
		{
			return FractionalHex(
				this->centerCoordinates[0] + other.centerCoordinates[0],
				this->centerCoordinates[1] + other.centerCoordinates[1],
				this->centerCoordinates[2] + other.centerCoordinates[2]
			);
		}

		FractionalHex operator-(const FractionalHex &other) const
		{
			return FractionalHex(
				this->centerCoordinates[0] - other.centerCoordinates[0],
				this->centerCoordinates[1] - other.centerCoordinates[1],
				this->centerCoordinates[2] - other.centerCoordinates[2]
			);
		}

		FractionalHex operator*(float scale) const
		{
			return FractionalHex(
				this->centerCoordinates[0] * scale,
				this->centerCoordinates[1] * scale,
				this->centerCoordinates[2] * scale
			);
		}

		FractionalHex operator*(int scale) const
		{
			return FractionalHex(
				this->centerCoordinates[0] * scale,
				this->centerCoordinates[1] * scale,
				this->centerCoordinates[2] * scale
			);
		}

		Hex roundToHex() 
		{
			int q = static_cast<int>(std::roundf(centerCoordinates[0]));
			int r = static_cast<int>(std::roundf(centerCoordinates[1]));
			int s = static_cast<int>(std::roundf(centerCoordinates[2]));

			double qDiff = std::abs(q - centerCoordinates[0]);
			double rDiff = std::abs(r - centerCoordinates[1]);
			double sDiff = std::abs(s - centerCoordinates[2]);

			if (qDiff > rDiff && qDiff > sDiff)
				q = -r - s;
			else if (rDiff > sDiff)
				r = -q - s;
			else
				s = -q - r;
			
			return Hex(q, r, s);
		}
	};


	inline FractionalHex Hex::operator*(float fractionalScale) const
	{
		return FractionalHex(
			static_cast<float>(this->centerCoordinates[0]) * fractionalScale,
			static_cast<float>(this->centerCoordinates[1]) * fractionalScale,
			static_cast<float>(this->centerCoordinates[2]) * fractionalScale
		);
	}
	
	inline FractionalHex Hex::LerpTo(const Hex& other, float slope)const
	{
		//The "epsilons" are there to nudge the FractionalHex to always be in the same "direction" when rounded to normal Hex.
		return FractionalHex(
			lerp(centerCoordinates[0] + 1e-6f, other.centerCoordinates[0] + 1e-6f, slope),
			lerp(centerCoordinates[1] + 1e-6f, other.centerCoordinates[1] + 1e-6f, slope),
			lerp(centerCoordinates[2] - 2e-6f, other.centerCoordinates[2] - 2e-6f, slope)
		);
	}

	inline std::vector<Hex> Hex::HexesOnTheLineToPoint(const Hex & other) const
	{
		int N = this->DistanceToHex(other);

		std::vector<Hex> results;
		results.reserve(N);

		float step = 1.0f / std::max(N, 1);

		results.push_back(*this);

		for (int i = 1; i <= N; ++i)
			results.push_back(LerpTo(other, step*i).roundToHex());

		return results;
	}
	
	namespace HexHelper
	{
		inline std::vector<Hex> FindAllHexesInRange(const Hex& centerPoint, int range)
		{
			std::vector<Hex> returnVector;
			for (int q = -range; q <= range; ++q)
				for (int s = std::max(-range, -q - range); s <= std::min(range, -q + range); ++s)
				{
					int r = -q - s;
					returnVector.push_back(Hex(centerPoint.q() + q, centerPoint.r() + r, centerPoint.s() + s));
				}
			return returnVector;
		};

		inline std::vector<Hex> FindAllHexesInIntersection(const Hex& centerPointA, int rangeA, const Hex&centerPointB, int rangeB)
		{
			std::vector<Hex> returnVector;

			int minQ = std::max(centerPointA.q() - rangeA, centerPointB.q() - rangeB);
			int maxQ = std::max(centerPointA.q() + rangeA, centerPointB.q() + rangeB);
			int minR = std::max(centerPointA.r() - rangeA, centerPointB.r() - rangeB);
			int maxR = std::max(centerPointA.r() + rangeA, centerPointB.r() + rangeB);
			int minS = std::max(centerPointA.s() - rangeA, centerPointB.s() - rangeB);
			int maxS = std::max(centerPointA.s() + rangeA, centerPointB.s() + rangeB);

			for (int q = minQ; q < maxQ; ++q)
				for (int r = std::max(minR, -q - maxS); r < std::min(maxR, -q - minS); ++r)
				{
					int s = -q - r;
					returnVector.push_back(Hex(q, r, s));
				}

			return returnVector;
		};

	};

}

template<> struct std::hash<TowerDefence::Game::Map::Hex>
{
	size_t operator()(const TowerDefence::Game::Map::Hex& hexToHash) const
	{
		hash<int> hashingFunction;
		size_t hq = hashingFunction(hexToHash.q());
		size_t hr = hashingFunction(hexToHash.r());

		return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
	}
};

#endif