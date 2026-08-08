#pragma once
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexSource, const char *fragmentSource);

  ~Shader();

  void use();
  void setMat4(const char *name, const glm::mat4 &matrix);
};
