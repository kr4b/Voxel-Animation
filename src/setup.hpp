#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "shader.hpp"
#include "window.hpp"

class Setup {
public:
    Setup();
    ~Setup();

    void update(const Window& window, const State& state);
    void start_render() const;
    void end_render() const;
    void debug(const Shader& shader) const;

    inline glm::mat4 get_proj() const { return this->proj; };
    inline glm::mat4 get_view() const { return this->view; };
    inline glm::mat4 get_inv_proj_view() const { return this->invProjView; };
    inline glm::vec2 get_reciprocal_window_size() const { return this->reciprocalWindowSize; };

private:
    GLuint vao;
    GLuint cameraUniform;
    glm::mat4 view, proj, invProjView;
    glm::vec2 reciprocalWindowSize;
  
};
