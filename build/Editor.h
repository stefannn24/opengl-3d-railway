#pragma once

#include <vector>
#include <unordered_map>

#include "Tema1/Ship.h"
#include "components/simple_scene.h"


class Editor {
public:
	Editor(gfxc::SimpleScene* scene);		// pointer to the main scene that will allow calling engine functions
	~Editor();

	void Init();
	void Update(float deltaTimeSeconds);

	void OnMouseMove(int mouseX, int mouseY);
	void OnMousePress(int mouseX, int mouseY, int button);
	void OnMouseRelease(int mouseX, int mouseY, int button);

	bool ShipIsValid() const;
	const Ship& GetShip() const;		// validated ship design that will be passed to the game

private:
	void CreateMeshes();
	void DrawUI();

	void DrawGrid();
	void DrawPallete();
	void DrawTopBar();
	void DrawDivieders();
	void DrawDraggedBlock();
	
	void CheckConstraints();

	std::	<int, int> GetGridCoords(int mouseX, int mouseY);		// convert mouse coords to grid indices

	gfxc::SimpleScene* scene;
	Ship ship;

	bool isDragging;
	BlockType draggedBlockType;
	glm::vec2 mouseCoords;
	bool constraintsValid;
	bool startGame;

	std::unordered_map<BlockType, std::string> blockMeshNames;		// map to find the correct mesh name for a given block type
};