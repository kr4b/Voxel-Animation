#version 450

// See 
// https://www.saschawillems.de/blog/2016/08/13/vulkan-tutorial-on-rendering-a-fullscreen-quad-without-buffers/

layout( location = 0 ) out vec2 v2fTexCoord;

void main() 
{
	v2fTexCoord = vec2(
		float((gl_VertexID << 1) & 2),
		float(gl_VertexID & 2)
	);

	gl_Position = vec4(
		v2fTexCoord * 2.0 - 1.0,
		0.0,
		1.0 
	);
}
