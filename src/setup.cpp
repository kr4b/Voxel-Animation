#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "setup.hpp"

struct VolumeUniform
{
    alignas(16) glm::vec3 volMin;
    alignas(16) glm::vec3 volMax;
};
struct CameraUniform
{
    alignas(16) glm::mat4 inverseProjCamera;
    alignas(16) glm::vec3 cameraWorldPos;
    alignas(16) glm::vec2 reciprocalWindowSize;
};

Setup::Setup(glm::vec3 volumeMin, glm::vec3 volumeMax) {
    glClearColor(0.0f, 0.6f, 0.8f, 1.0f);

    glCreateVertexArrays(1, &this->vao);

    const VolumeUniform volume { volumeMin, volumeMax };
    glCreateBuffers(1, &this->volumeUniform);
    glNamedBufferStorage(this->volumeUniform, sizeof(VolumeUniform), &volume, GL_NONE);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->volumeUniform);

    glCreateBuffers(1, &this->cameraUniform);
    glNamedBufferStorage(this->cameraUniform, sizeof(CameraUniform), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->cameraUniform);
}

Setup::~Setup() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->volumeUniform);
    glDeleteBuffers(1, &this->cameraUniform);
}

void Setup::update(const Window& window, const State& state) const {
    const auto size = window.get_size();
    glViewport(0, 0, size.first, size.second);

    const glm::mat4 proj = glm::perspective(
        glm::radians(60.0f),
        float(size.first) / float(size.second),
        0.01f, 100.0f
    );

    const glm::mat4 view = glm::translate(glm::toMat4(state.cameraRot), state.cameraOff);
    const glm::vec4 cameraWorldPos = glm::inverse(view) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec2 reciprocalWindowSize = glm::vec2(
        1.0f / float(size.first),
        1.0f / float(size.second)
    );

    const CameraUniform camera {
        glm::inverse(proj * view),
        cameraWorldPos.xyz() / cameraWorldPos.w,
        reciprocalWindowSize,
    };

    glNamedBufferSubData(this->cameraUniform, 0, sizeof(CameraUniform), &camera);
}

void Setup::render(const Shader& shader) const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    glBindVertexArray(this->vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(0);
    glBindVertexArray(0);
}
