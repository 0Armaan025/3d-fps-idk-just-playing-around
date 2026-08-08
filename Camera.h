#pragma once

#include <glm/glm.hpp>

class Camera {
public:
  glm::vec3 position;

  float yaw;
  float pitch;
  float speed;

  Camera(glm::vec3 startPosition, float movementSpeed);

  void moveForward(float deltaTime);

  void moveBackward(float deltaTime);

  void moveLeft(float deltaTime);

  void moveRight(float deltaTime);

  glm::vec3 getForwardDirection();
  glm::vec3 getRightDirection();

  glm::mat4 getViewMatrix();
};
