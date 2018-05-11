#include "HexRepresentation.h"

using namespace TowerDefence::Game::Map;
//Should probably move most of the stuff here...

const std::vector<Hex> Hex::HexDirections = {
	Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1),
	Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)
};
const std::vector<Hex> Hex::HexDiagonal = {
	Hex(+2, -1, -1), Hex(+1, +1, -2), Hex(-1, +2, -1),
	Hex(-2, +1, +1), Hex(-1, -1, +2), Hex(+1, -2, +1),
};