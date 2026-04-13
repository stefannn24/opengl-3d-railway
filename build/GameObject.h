#pragma once
#include "utils/glm_utils.h"
#include <string>

// Represents a generic object in our 2D game world.
class GameObject {
public:
    // State
    glm::vec2 position, size, velocity;
    glm::vec3 color;
    std::string meshName;

    // Constructor
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, std::string meshName, glm::vec3 color = glm::vec3(1), glm::vec2 velocity = glm::vec2(0));

    // Update the object's position based on its velocity.
    void Update(float deltaTime);
};