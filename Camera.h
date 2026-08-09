#pragma once

#include <glm/glm.hpp>

class Camera {
public:
  glm::vec3 position;

  float yaw;
  float pitch;
  float speed;

  float sensitivity;

  Camera(glm::vec3 startPosition, float movementSpeed, float mouseSensitivity);

  void moveForward(float deltaTime);

  void moveBackward(float deltaTime);

  void moveLeft(float deltaTime);

  void moveRight(float deltaTime);

  void processMouseMovement(float offsetX, float offsetY);

  glm::vec3 getForwardDirection();
  glm::vec3 getRightDirection();
  glm::vec3 getHorizontalForwardDirection();

  glm::mat4 getViewMatrix();
};
