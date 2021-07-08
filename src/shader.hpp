#pragma once

#include <fstream>
#include <string>

#include <glad/glad.h>

class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);

  inline void use() { glUseProgram(this->program); }

private:
  GLuint program;
};
