#pragma once

#include "setup.hpp"

class Axis {
public:
    Axis() {
        init();
    }

    void render(const Setup& setup) {
        glBindVertexArray(vao);
        glLineWidth(5.0f);

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

        glBindVertexArray(0);
    }

private:
    GLuint vao;

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