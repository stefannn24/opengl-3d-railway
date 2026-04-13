#include <vector>

#include "Tema1/Ship.h"

Ship::Ship() : blockCount(0) {
	grid.resize(Constants::GRID_ROWS, std::vector<Block>(Constants::GRID_COLS));

	for (int i = 0; i < Constants::GRID_ROWS; ++i) {
		for (int j = 0; j < Constants::GRID_COLS; ++j) {
			grid[i][j].position = glm::vec2(j, i);
		}
	}
}

Ship::~Ship() {}

void Ship::AddBlock(BlockType type, glm::vec2 gridPos) {
	int row = static_cast<int>(gridPos.y);
	int col = static_cast<int>(gridPos.x);

	// check if the cell is indeed empty
	if (grid[row][col].type == EMPTY && type != EMPTY) {
		grid[row][col].type = type;
		blockCount++;
	}
}

void Ship::RemoveBlock(glm::vec2 gridPos) {
	int row = static_cast<int>(gridPos.y);
	int col = static_cast<int>(gridPos.x);

	if (grid[row][col].type != EMPTY) {
		grid[row][col].type = EMPTY;
		blockCount--;
	}
}

int Ship::GetBlockCount() const {
	return blockCount;
}

const std::vector<std::vector<Block>>& Ship::GetGrid() const {
	return grid;
}