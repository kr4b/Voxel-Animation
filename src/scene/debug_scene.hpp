#pragma once

#include "window.hpp"
#include "state.hpp"
#include "setup.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

struct UMatrices {
    alignas(16) glm::mat4x4 proj;
    alignas(16) glm::mat4x4 view;
};

class Axis {
public:
    Axis(Window &window, State &state) :
        window(window),
        state(state),
        setup(Setup()),
        shader(Shader("assets/debug.vert", "assets/debug.frag"))
    {
        init();
    }

    void render() {
        const auto size = window.get_size();
        glViewport(0, 0, size.first, size.second);

        shader.use();

        glBindVertexArray(vao);
        glLineWidth(5.0f);

        matrices.proj = glm::perspective(
            glm::radians(60.0f),
            float(size.first) / float(size.second),
            0.0f, 100.0f
        );
        matrices.view = glm::translate(glm::mat4(1.0f), state.cameraOff) * glm::toMat4(state.cameraRot);

        glNamedBufferSubData(uniform, 0, sizeof(UMatrices), &matrices);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform);

        glDrawArrays(GL_LINES, 0, 6);

        glBindVertexArray(0);
        glLineWidth(1.0f);
    }

protected:
    void init() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data_length, data, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 6, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(GLfloat) * 6, (GLvoid *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glCreateBuffers(1, &uniform);
        glNamedBufferStorage(uniform, sizeof(UMatrices), &matrices, GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform);

        glBindVertexArray(0);
    }

private:
    Window &window;
    State  &state;
    Setup  setup;
    Shader shader;

    GLuint vao;
    GLuint uniform;

    UMatrices matrices = { glm::mat4x4(), glm::mat4x4() };

    const unsigned int data_length = 36;
    const float data[36] = {
        -50.0f,   0.0f,   0.0f,  // X1
          0.5f,   0.1f,   0.1f,  // Color X1
         50.0f,   0.0f,   0.0f,  // X2
          0.5f,   0.1f,   0.1f,  // Color X2
          0.0f, -50.0f,   0.0f,  // Y1
          0.1f,   0.5f,   0.1f,  // Color Y1
          0.0f,  50.0f,   0.0f,  // Y2
          0.1f,   0.5f,   0.1f,  // Color Y2
          0.0f,   0.0f, -50.0f,  // Z1
          0.1f,   0.1f,   0.5f,  // Color Z1
          0.0f,   0.0f,  50.0f,  // Z2
          0.1f,   0.1f,   0.5f   // Color Z2
    };
};