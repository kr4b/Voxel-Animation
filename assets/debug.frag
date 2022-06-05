#version 450

layout( location = 0 ) in vec3 iColor;

layout( location = 0 ) out vec3 oColor;

void main() {
  oColor = iColor;
  gl_FragDepth = 0.98;
}