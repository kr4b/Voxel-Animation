#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "shader.hpp"
#include "window.hpp"

class Setup {
public:
    Setup();
    ~Setup();

    void update(const Window& window, const State& state);
    void start_render(const Shader& shader) const;
    void end_render() const;
    void debug(const Shader& shader) const;

private:
    GLuint vao;
    GLuint cameraUniform;
    glm::mat4 view, proj;
  
};