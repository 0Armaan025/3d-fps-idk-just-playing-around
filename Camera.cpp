#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normalize_dot.hpp>

Camera::Camera(glm::vec3 startPosition, float movementSpeed) {

  position = startPosition;
  speed = movementSpeed;

  yaw = -90.0f;
  pitch = 0.0f;
}

glm::vec3 Camera::getForwardDirection() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  return glm::normalize(direction);
}
glm::vec3 Camera::getRightDirection() {
  glm::vec3 forward = getForwardDirection();
  return glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Camera::moveBackward(float deltaTime) {
  position -= getForwardDirection() * speed * deltaTime;
}

void Camera::moveForward(float deltaTime) {
  position += getForwardDirection() * speed * deltaTime;
}

void Camera::moveLeft(float deltaTime) {
  position -= getRightDirection() * speed * deltaTime;
}

void Camera::moveRight(float deltaTime) {
  position += getRightDirection() * speed * deltaTime;
}

glm::mat4 Camera::getViewMatrix() {

  glm::vec3 forward = getForwardDirection();

  return glm::lookAt(position,

                     position + forward,
                     // I don't get this math

                     glm::vec3(0.0f, 1.0f, 0.0f));
}

// glm::lookAt(position, target, up)
