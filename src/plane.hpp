#pragma once

#include <glad/glad.h>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "glm/trigonometric.hpp"
#include "math_util.hpp"

class Ray;

// Plane described by one (corner) vertex and 2 spanning vectors
// TODO: Split up in cpp + hpp
class Plane {
public:
    const glm::vec3 point;
    const glm::vec3 span1;
    const glm::vec3 span2;
    const glm::vec3 normal;

    glm::mat4 matrix;
    glm::mat4 inv_matrix;

    Plane(glm::vec3 point, glm::vec3 span1, glm::vec3 span2) :
        point(point),
        span1(span1),
        span2(span2),
        normal(construct_normal(span1, span2))
    {
        // Axis aligned plane
        if (abs(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), normal)) == 1.0f) {
            this->matrix = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                point.x, point.y, point.z, 1.0f
            );
        // Non-axis aligned plane
        } else {
            const glm::vec3 rotationAxis = glm::normalize(glm::cross(this->normal, glm::vec3(0.0f, 1.0f, 0.0f)));
            const float rotationAngle = M_PI - glm::acos(this->normal.y);

            const float cosAngle = cos(rotationAngle);
            const float sinAngle = sin(rotationAngle);

            this->matrix = glm::transpose(glm::mat4(
                cosAngle + rotationAxis.x * rotationAxis.x * (1.0f - cosAngle),
                rotationAxis.x * rotationAxis.y * (1.0f - cosAngle) - rotationAxis.z * sinAngle,
                rotationAxis.x * rotationAxis.z * (1.0f - cosAngle) + rotationAxis.y * sinAngle,
                point.x,

                rotationAxis.y * rotationAxis.x * (1.0f - cosAngle) + rotationAxis.z * sinAngle,
                cosAngle + rotationAxis.y * rotationAxis.y * (1.0f - cosAngle),
                rotationAxis.y * rotationAxis.z * (1.0f - cosAngle) - rotationAxis.x * sinAngle,
                point.y,

                rotationAxis.z * rotationAxis.x * (1.0f - cosAngle) - rotationAxis.y * sinAngle,
                rotationAxis.z * rotationAxis.y * (1.0f - cosAngle) + rotationAxis.x * sinAngle,
                cosAngle + rotationAxis.z * rotationAxis.z * (1.0f - cosAngle),
                point.z,

                0.0f, 0.0f, 0.0f, 1.0f
            ));
        }
        this->inv_matrix = glm::inverse(this->matrix);
    }

    // Transform this plane with the given matrix
    Plane transform(const glm::mat4& matrix) const {
        return Plane(
            glm::vec3(matrix * glm::vec4(this->point, 1.0f)),
            glm::normalize(glm::vec3(matrix * glm::vec4(this->span1, 0.0f))),
            glm::normalize(glm::vec3(matrix * glm::vec4(this->span2, 0.0f)))
        );
    };

    void init_vao(const glm::vec3 color) {
        glGenVertexArrays(1, &this->vao);
        glBindVertexArray(this->vao);

        glGenBuffers(2, this->buffers);

        GLfloat vertices[4 * 3];
        GLfloat colors[4 * 3];

        const float scale = 1.0f;
        const glm::vec3 v1 = point;
        const glm::vec3 v2 = point + span1 * scale;
        const glm::vec3 v3 = point + (span1 + span2) * scale;
        const glm::vec3 v4 = point + span2 * scale;
        vertices[0] = v1.x;
        vertices[1] = v1.y;
        vertices[2] = v1.z;
        vertices[3] = v2.x;
        vertices[4] = v2.y;
        vertices[5] = v2.z;
        vertices[6] = v3.x;
        vertices[7] = v3.y;
        vertices[8] = v3.z;
        vertices[9] = v4.x;
        vertices[10] = v4.y;
        vertices[11] = v4.z;

        colors[0] = color.x;
        colors[1] = color.y;
        colors[2] = color.z;
        colors[3] = color.x;
        colors[4] = color.y;
        colors[5] = color.z;
        colors[6] = color.x;
        colors[7] = color.y;
        colors[8] = color.z;
        colors[9] = color.x;
        colors[10] = color.y;
        colors[11] = color.z;

        // Vertices
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // Colors
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), colors, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    };

    void render() {
        glBindVertexArray(this->vao);

        glLineWidth(3.0f);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glLineWidth(1.0f);

        glBindVertexArray(0);
    }

    void clean() {
        glDeleteVertexArrays(1, &this->vao);
        glDeleteBuffers(2, this->buffers);
    }

private:
    GLuint vao;
    GLuint buffers[2];

    // Construct normal that always points up, maybe the up vector could be given as a parameter
    const glm::vec3 construct_normal(glm::vec3 span1, glm::vec3 span2) {
        const glm::vec3 normal = glm::normalize(glm::cross(span1, span2));
        return normal;
    }
};
