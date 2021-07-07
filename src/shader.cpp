#include "shader.hpp"

GLuint load_shader(const char* path, const GLenum type) {
  int status;
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
  const GLchar* shaderSource = content.c_str();

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (!status) {
    char log[1024];
    glGetShaderInfoLog(shader, sizeof(log), NULL, log);
    char error[sizeof(log) + 64];
    snprintf(error, sizeof(error), "%s shader compilation failed: %s\n", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
    throw new std::runtime_error(error);
  }

  return shader;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  int status;
  const GLuint vertex = load_shader(vertexPath, GL_VERTEX_SHADER);
  const GLuint fragment = load_shader(fragmentPath, GL_FRAGMENT_SHADER);

  this->program = glCreateProgram();
  glAttachShader(this->program, vertex);
  glAttachShader(this->program, fragment);
  glLinkProgram(this->program);
  glGetProgramiv(this->program, GL_LINK_STATUS, &status);

  if (!status) {
    char log[1024];
    glGetProgramInfoLog(program, sizeof(log), NULL, log);
    char error[sizeof(log) + 64];
    snprintf(error, sizeof(error), "Program linking failed: %s\n");
    throw new std::runtime_error(error);
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
