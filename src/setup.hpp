#pragma once

#include <glad/glad.h>

#include <glm/vec3.hpp>

#include "shader.hpp"
#include "window.hpp"

class Setup {
public:
    Setup(glm::vec3, glm::vec3);
    ~Setup();

    void update(const Window& window, const State& state) const;
    void render(const Shader& shader) const;

private:
    GLuint vao;
    GLuint volumeUniform, cameraUniform;
  
};