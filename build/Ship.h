#pragma once

#include <vector>

#include "lab_m1/Tema1/Constants.h"

enum BlockType {
	EMPTY,
	SOLID,
	CANNON,
	ENGINE,
	BUMPER
};

struct Block {
	BlockType type = EMPTY;
	glm::vec2 position = glm::vec2(0, 0);
};

class Ship {
public:
	Ship();
	~Ship();

	void AddBlock(BlockType type, glm::vec2 gridPos);
	void RemoveBlock(glm::vec2 gridPos);

	int GetBlockCount() const;
	const std::vector<std::vector<Block>>& GetGrid() const;

private:
	std::vector<std::vector<Block>> grid;
	int blockCount;
};
