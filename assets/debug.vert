#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 iColor;

uniform mat4 proj;
uniform mat4 view;

void main() {
	iColor = color;
	gl_Position = proj * view * vec4(position, 1.0);
}
