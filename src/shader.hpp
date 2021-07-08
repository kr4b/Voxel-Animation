#pragma once

#include <fstream>
#include <string>

#include <glad/glad.h>

class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);
  ~Shader();

  inline void use() const { glUseProgram(this->program); }

private:
  GLuint program;
};
