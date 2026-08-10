// https://docs.gl/es3/glGetShaderiv for reference
// though, I just used chatgpt lol
//
#include "Shader.h"
#include <iostream>

// compiling a shader - important

static GLuint compileShader(GLenum type, const char *source) {
  GLuint shader = glCreateShader(type);

  glShaderSource(shader, 1, &source, nullptr);

  glCompileShader(shader);

  // check compilation or throw error

  GLint success = 0;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
    std::cerr << "Shader compilation failed:\n" << infoLog << '\n';
  }

  return shader;
}

Shader::Shader(const char *vertexSource, const char *fragmentSource) {
  // compiling a vertex shader (how? we use our created function)

  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);

  // compiling fragmentShader

  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

  ID = glCreateProgram();

  glAttachShader(ID, vertexShader);

  glAttachShader(ID, fragmentShader);

  // link

  glLinkProgram(ID);

  // check linking
  GLint success = 0;
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(ID, sizeof(infoLog), nullptr, infoLog);
    std::cerr << "Shader linking failed:\n" << infoLog << '\n';
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}
Shader::~Shader() { glDeleteProgram(ID); }

void Shader::use() { glUseProgram(ID); }

void Shader::setMat4(const char *name, const glm::mat4 &matrix) {

  GLint location = glGetUniformLocation(ID, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setVec3(const char *name, const glm::vec3 &value) {
  GLint location = glGetUniformLocation(ID, name);

  glUniform3fv(location, 1, &value[0]);
}
