#pragma once

#include "lab_m1/Tema1/Tema1.h"
#include "GameObject.h"
#include "Ship.h"
#include <vector>

class PongLogic {
public:
    PongLogic(m1::Tema1* scene);
    ~PongLogic();

    // Initializes the game, creates the paddles from the ship design, and resets the state.
    void Init(const Ship& playerShip);
    // The main game loop, called every frame by Tema1.
    void Update(float deltaTimeSeconds);
    // Receives keyboard input from Tema1.
    void OnInputUpdate(float deltaTime, int mods);

private:
    // --- Private Helper Functions ---
    void CreatePlayerPaddleMesh(const Ship& playerShip);
    void HandleCollisions();
    void ResetRound();
    void MoveAI(float deltaTimeSeconds);
    void DrawScene();
    void DrawUI();

private:
    // A pointer to the main scene, used for rendering.
    m1::Tema1* scene;

    // --- Game Objects ---
    GameObject playerPaddle;
    GameObject aiPaddle;
    std::vector<GameObject> balls;

    // --- Game State & Ship Properties ---
    int playerScore, aiScore;
    float playerSpeed;
    int cannonCount;
    int bumperCount;

    // Game world boundaries
    glm::vec2 worldSize;
};