#pragma once

#include "../aabb.hpp"
#include "../setup.hpp"

#include <glm/vec3.hpp>

class WireframeAABB {
public:
    WireframeAABB(const AABB& aabb) {
        init(aabb, glm::vec3(1.0f));
    }
    
    WireframeAABB(const AABB& aabb, const glm::vec3 color) {
        init(aabb, color);
    }

    void render(const Setup& setup) {
        glBindVertexArray(vao);
        glLineWidth(2.0f);

        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glLineWidth(1.0f);
    }

    void update(const AABB& aabb) {
        const unsigned int points_length = 3 * 8;
        const float points[points_length] = {
            aabb.min.x, aabb.min.y, aabb.min.z, // 0  -X -Y -Z
            aabb.min.x, aabb.min.y, aabb.max.z, // 1  -X -Y  Z
            aabb.min.x, aabb.max.y, aabb.min.z, // 2  -X  Y -Z
            aabb.min.x, aabb.max.y, aabb.max.z, // 3  -X  Y  Z
            aabb.max.x, aabb.min.y, aabb.min.z, // 4   X -Y -Z
            aabb.max.x, aabb.min.y, aabb.max.z, // 5   X -Y  Z
            aabb.max.x, aabb.max.y, aabb.min.z, // 6   X  Y -Z
            aabb.max.x, aabb.max.y, aabb.max.z, // 7   X  Y  Z
        };

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * points_length, points);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

protected:
    void init(const AABB& aabb, const glm::vec3 color) {
        const float color_data[3] = { color.x, color.y, color.z };

        const unsigned int points_length = 3 * 8;
        const float points[points_length] = {
            aabb.min.x, aabb.min.y, aabb.min.z, // 0  -X -Y -Z
            aabb.min.x, aabb.min.y, aabb.max.z, // 1  -X -Y  Z
            aabb.min.x, aabb.max.y, aabb.min.z, // 2  -X  Y -Z
            aabb.min.x, aabb.max.y, aabb.max.z, // 3  -X  Y  Z
            aabb.max.x, aabb.min.y, aabb.min.z, // 4   X -Y -Z
            aabb.max.x, aabb.min.y, aabb.max.z, // 5   X -Y  Z
            aabb.max.x, aabb.max.y, aabb.min.z, // 6   X  Y -Z
            aabb.max.x, aabb.max.y, aabb.max.z, // 7   X  Y  Z
        };

        // Line indices
        const unsigned int indices_length = 2 * 12;
        const unsigned int indices[indices_length] = {
            0, 1,
            0, 4,
            0, 2,
            7, 6,
            7, 3,
            7, 5,
            1, 3,
            1, 5,
            2, 3,
            2, 6,
            4, 5,
            4, 6
        };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbos[2];
        glGenBuffers(1, &vertex_buffer);
        glGenBuffers(2, vbos);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points_length, points, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3, color_data, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
        glVertexAttribDivisor(1, indices_length / 2);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_length, indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

private:
    GLuint vao, vertex_buffer;
};

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