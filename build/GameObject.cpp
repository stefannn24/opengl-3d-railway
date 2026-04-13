#include "GameObject.h"

GameObject::GameObject()
    : position(0, 0), size(1, 1), velocity(0, 0), color(1), meshName("") {
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, std::string meshName, glm::vec3 color, glm::vec2 velocity)
    : position(pos), size(size), meshName(meshName), color(color), velocity(velocity) {
}

void GameObject::Update(float deltaTime) {
    // Simple physics: position changes based on velocity and time.
    this->position += this->velocity * deltaTime;
}