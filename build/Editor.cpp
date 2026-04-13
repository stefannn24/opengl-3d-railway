#include "Tema1/Editor.h"
#include "Tema1/Constants.h"
#include <iostream>

Editor::Editor(gfxc::SimpleScene* scene) 
	: scene(scene), isDragging(false), draggedBlockType(BlockType::EMPTY), constraintsValid(false), startGame(false) {
}

Editor::~Editor() {}

void Editor::Init() {
	CreateMeshes();
}

void Editor::Update(float deltaTimeSeconds) {
	DrawUI();
}

void Editor::OnMouseMove(int mouseX, int mouseY) {
	mouseCoords = glm::vec2(mouseX, mouseY);
}

void Editor::OnMousePress(int mouseX, int mouseY, int button) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		// convert mouse's y coord from screen space to world space
		float worldMouseY = scene->GetWindow()->GetResolution().y - mouseY;

		// check if click was on any of the pallete items
		BlockType types[] = { BlockType::SOLID, BlockType::CANNON, BlockType::ENGINE, BlockType::BUMPER };
		for (int i = 0; i < 4; i++) {
			glm::vec2 itemPos = glm::vec2(Constants::PALLETTE_START_POS.x, Constants::PALLETE_START_POS.y - i * Constants::PALLETE_SPACING);
			// start dragging if the click is inside the pallete
			if (mouseX > itemPos.x && mouseX < itemPos.x + Constants::PALLETE_ITEM_SIZE &&
				worldMouseY > itemPos.y && worldMouseY < itemPos.y + Constants::PALLETE_ITEM_SIZE)
			{
				isDragging = true;
				draggedBlockType = types[i];
				return;
			}
		}

		// otherwise, check if the start button was pressed
		float startBtnX = Constants::TOP_BAR_START_POS.x + Constants::MAX_SHIP_SIZE * (Constants::TOP_BAR_SQUARE_SIZE + Constants::TOP_BAR_SPACING);
		float startBtnY = Constants::TOP_BAR_START_POS.y;

		if (mouseX > startBtnX && mouseX < startBtnX + Constants::TOP_BAR_SQUARE_SIZE / 2 &&
			worldMouseY > startBtnX && worldMouseY < startBtnY + Constants::TOP_BAR_SQUARE_SIZE) {		// since start button apprx is half size
			if (constraintsValid) {
				startGame = true;
			}
			else {
				std::cout << "Ship design is invalid! Cannot start the game." << std::endl;
			}
		}

		// checl for right mouse click
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			auto getGridPos = GetGridCoords(mouseX, mouseY);
			if (getGridPos != -1) {
				ship.removeBlock(glm::vec2(getGridPos.second, getGridPos.first));
				CheckCostraints();
			}
		}
	}
}

void Editor