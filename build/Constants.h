#pragma once

#include "utils/glm_utils.h"

namespace Constants {
	const int MAX_SHIP_SIZE = 10;
	
	// constants for the construction grid
	const int GRID_ROWS = 9;
	const int GRID_COLS = 17;
	const float CELL_SIZE = 20.0f;
	const float GRID_SPACING = 5.0f;
	const glm::vec2 GRID_START_POS = glm::vec2(150, 50);

	// constants for the block pallete on the left
	const glm::vec2 PALLETE_START_POS = glm::vec2(50, 600);
	const float PALLETE_ITEM_SIZE = 80.0f;
	const float PALLETE_SPACING = 180.0f;

	// constants for the top bar
	const glm::vec2 TOP_BAR_START_POS = glm::vec2(250, 700);
	const float TOP_BAR_SQUARE_SIZE = 50.0f;
	const float TOP_BAR_SPACING = 10.0f;

	// constants for colors
	const glm::vec3 COLOR_DIVIDER = glm::vec3(1.0f, 0.0f, 0.0f);		// red for the divider lines
	const glm::vec3 COLOR_GRID_LINE = glm::vec3(0.0f, 0.0f, 0.5f);		// dark blue for the grid lines
	const glm::vec3 COLOR_GRID_BLOCK = glm::vec3(0.2f, 0.5f, 1.0f);		// light blue for the blocks in the grid

	const glm::vec3 COLOR_SOLID_BLOCK = glm::vec3(0.8f, 0.8f, 0.8f);		// medium grey for solid blocks in the pallete
	const glm::vec3 COLOR_CANNON_BASE = glm::vec3(0.95f, 0.95f, 0.95f);		// light grey for the base of the canon block
	const glm::vec3 COLOR_CANNON_MIDDLE = glm::vec3(0.8f, 0.8f, 0.8f);		// medium grey for the middle part of the canon block
	const glm::vec3 COLOR_CANNON_BARREL = glm::vec3(0.2f, 0.2f, 0.2f);		// dark grey for the barrel of the canon block
	const glm::vec3 COLOR_ENGINE_FLAME1 = glm::vec3(1.0f, 0.5f, 0.0f);		// orange for the lower flame of the engine block
	const glm::vec3 COLOR_ENGINE_FLAME2 = glm::vec3(1.0f, 0.65f, 0.0f);		// lighter orange for the upper flame of the engine block

	const glm::vec3 COLOR_TOP_APPROVED = glm::vec3(0.1f, 0.8f, 0.1f);		// green for approved status in the top bar
	const glm::vec3 COLOR_TOP_REJECTED = glm::vec3(0.9f, 0.1f, 0.1f);		// red for rejected status in the top bar
}