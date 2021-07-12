#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "setup.hpp"

struct CameraUniform
{
    alignas(16) glm::mat4 inverseProjCamera;
    alignas(16) glm::vec3 cameraWorldPos;
    alignas(16) glm::vec2 reciprocalWindowSize;
};

Setup::Setup() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glCreateVertexArrays(1, &this->vao);

    glCreateBuffers(1, &this->cameraUniform);
    glNamedBufferStorage(this->cameraUniform, sizeof(CameraUniform), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->cameraUniform);
}

Setup::~Setup() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->cameraUniform);
}

void Setup::update(const Window& window, const State& state) {
    const auto size = window.get_size();
    glViewport(0, 0, size.first, size.second);

    this->proj = glm::perspective(
        glm::radians(60.0f),
        float(size.first) / float(size.second),
        0.01f, 100.0f
    );

    this->view = glm::translate(glm::mat4(1.0f), state.cameraOff) * glm::toMat4(state.cameraRot);
    const glm::vec4 cameraWorldPos = glm::inverse(this->view) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->reciprocalWindowSize = glm::vec2(
        1.0f / float(size.first),
        1.0f / float(size.second)
    );

    this->invProjView = glm::inverse(this->proj * this->view);

    const CameraUniform camera {
        this->invProjView,
        glm::vec3(cameraWorldPos.x, cameraWorldPos.y, cameraWorldPos.z) / cameraWorldPos.w,
        this->reciprocalWindowSize,
    };

    glNamedBufferSubData(this->cameraUniform, 0, sizeof(CameraUniform), &camera);
}

void Setup::start_render(const Shader& shader) const {
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Setup::end_render() const {
    glUseProgram(0);
    glBindVertexArray(0);
}

void Setup::debug(const Shader& shader) const {
    shader.uniformMat4("view", this->view);
    shader.uniformMat4("proj", this->proj);
}
