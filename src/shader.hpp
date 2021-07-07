#pragma once

#include <fstream>
#include <string>

#include <GL/glew.h>

class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);

  inline void use() { glUseProgram(this->program); }

private:
  GLuint program;
};
