#pragma once

#include <fstream>
#include <string>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);
  ~Shader();

  inline void use() const { glUseProgram(this->program); }

  inline void uniformMat4(const char* name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(matrix));
  }

  inline void uniformFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(this->program, name), value);
  }

  inline void uniformUInt(const char* name, uint8_t value) const {
    glUniform1ui(glGetUniformLocation(this->program, name), value);
  }

private:
  GLuint program;
};
