#include "Camera.h"
#include "glad/include/glad/glad.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Shader.h"

const char *vertexShaderSource = R"(
    #version 330 core

    layout (location = 0) in vec3 aPosition;
    layout (location = 1) in vec3 aNormal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec3 Normal;

    void main()
    {
        Normal = mat3(transpose(inverse(model))) * aNormal;

        gl_Position =
            projection *
            view *
            model *
            vec4(aPosition, 1.0);
    }
)";
const char *fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

in vec3 Normal;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    vec3 normal = normalize(Normal);

    vec3 lightDir = normalize(-lightDirection);

    float diffuse = max(
        dot(normal, lightDir),
        0.0
    );

    float ambient = 0.2;

    vec3 lighting =
        (ambient + diffuse) * lightColor;

    vec3 finalColor =
        objectColor * lighting;

    FragColor = vec4(finalColor, 1.0);
}
)";
int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';

    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window *window = SDL_CreateWindow(
      "Mini 3D Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280,
      720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    std::cerr << "Window creation failed: " << SDL_GetError() << '\n';

    SDL_Quit();

    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);

  if (!context) {
    std::cerr << "OpenGL context creation failed: " << SDL_GetError() << '\n';

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 1;
  }

  SDL_SetRelativeMouseMode(SDL_TRUE);

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";

    SDL_GL_DeleteContext(context);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 1;
  }

  SDL_GL_SetSwapInterval(1);
  glEnable(GL_DEPTH_TEST);

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';

  float vertices[] = {

      // position                    // normal

      -50.0f, 0.0f, -50.0f, 0.0f,   1.0f, 0.0f,  50.0f, 0.0f, -50.0f,
      0.0f,   1.0f, 0.0f,   50.0f,  0.0f, 50.0f, 0.0f,  1.0f, 0.0f,

      -50.0f, 0.0f, -50.0f, 0.0f,   1.0f, 0.0f,  50.0f, 0.0f, 50.0f,
      0.0f,   1.0f, 0.0f,   -50.0f, 0.0f, 50.0f, 0.0f,  1.0f, 0.0f};
  GLuint VAO = 0;

  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  GLuint VBO = 0;

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glEnableVertexAttribArray(1);
  Shader shader(vertexShaderSource, fragmentShaderSource);

  // ============================================================
  // GAME LOOP
  //
  //
  // ============================================================
  //

  Camera camera(glm::vec3(0.0f, 1.6f, 3.0f), 3.0f, 0.1f);
  glm::mat4 projection = glm::perspective(glm::radians(60.0f), // FOV
                                          1280.0f / 720.0f,    // aspect ratio
                                          0.1f,                // near plane
                                          100.0f               // far plane
  );

  Uint64 lastTime = SDL_GetPerformanceCounter();

  bool running = true;

  glm::vec3 lightDirection(-0.5, -1.0f, -0.3f);
  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

  glm::vec3 groundColor(0.35f, 0.45f, 0.35f);

  while (running) {
    // --------------------------------------------------------
    // Events
    // --------------------------------------------------------

    Uint64 currentTime = SDL_GetPerformanceCounter();

    float deltaTime = static_cast<float>(currentTime - lastTime) /
                      static_cast<float>(SDL_GetPerformanceFrequency());

    lastTime = currentTime;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }

      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
      }

      if (event.type == SDL_MOUSEMOTION) {
        camera.processMouseMovement(event.motion.xrel, event.motion.yrel);
      }
    }

    const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);

    if (keyboard[SDL_SCANCODE_W]) {

      camera.moveForward(deltaTime);
    }

    if (keyboard[SDL_SCANCODE_S]) {
      camera.moveBackward(deltaTime);
    }

    if (keyboard[SDL_SCANCODE_A]) {
      camera.moveLeft(deltaTime);
    }

    if (keyboard[SDL_SCANCODE_D]) {
      camera.moveRight(deltaTime);
    }

    glClearColor(0.03f, 0.03f, 0.05f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    glm::mat4 view = camera.getViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    shader.setVec3("lightDirection", lightDirection);
    shader.setVec3("lightColor", lightColor);
    shader.setVec3("objectColor", groundColor);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    SDL_GL_SwapWindow(window);
  }

  glDeleteVertexArrays(1, &VAO);

  glDeleteBuffers(1, &VBO);

  SDL_GL_DeleteContext(context);

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
