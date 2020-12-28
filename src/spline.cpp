#include "spline.hpp"

const int detail = 100;

Spline::Spline(const gl::GLapi* gl) {
	this->a = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);
	this->b = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);
	this->c = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);
	this->d = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);

	this->init_program(gl);
	this->init_vao(gl);
}

void Spline::init_program(const gl::GLapi* gl) {
	this->debugProgram = gl::load_program_from_vfs({
	{ gl::GL::VERTEX_SHADER, "/@flux/opt/assets/debug.vert" },
	{ gl::GL::FRAGMENT_SHADER, "/@flux/opt/assets/debug.frag" }
		});
}

void Spline::init_vao(const gl::GLapi* gl) {
	gl->genVertexArrays(1, &this->debugVao);
	gl->bindVertexArray(this->debugVao);

	gl->genBuffers(2, this->buffers);

	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * detail * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(0, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(0);

	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * detail * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(1, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(1);

	gl->bindVertexArray(0);
}

void Spline::update_from_screen_coords(const gl::GLapi* gl, const vec2f coords, const mat44f inverseProjCamera, const vec3f cameraWorldPos) {
	const vec2f transformedCoordinates = coords * 2.0f - fml::make_vector<vec2f>(1.0f, 1.0f);
	const vec4f hray = fml::make_vector<vec4f>(transformedCoordinates.x, transformedCoordinates.y, 1.0f, 1.0f);
	const vec4f wray = inverseProjCamera * hray;

	const vec3f origin = cameraWorldPos;
	const vec3f direction = fml::normalize(-cameraWorldPos);// fml::normalize(fml::make_vector<vec3f>(wray.x, wray.y, wray.z) / wray.w - origin);

	const vec3f P1 = origin;
	const vec3f P2 = origin + direction * fml::length(origin) * 2.0f;

	const vec3f P0 = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);
	const vec3f P3 = fml::make_vector<vec3f>(0.0f, 10.0f, 0.0f);

	this->a = 2.0f * P1 - 2.0f * P2 + 1.0f * P0 + 1.0f * P3;
	this->b = -3.0f * P1 + 3.0f * P2 - 2.0f * P0 - 1.0f * P3;
	this->c = P0;
	this->d = P1;

	this->update_buffers(gl);
}

void Spline::update_buffers(const gl::GLapi *gl) {
	const float stepSize = 1.0f / (float)detail;

	gl::GL::Float vertices[detail * 3];
	gl::GL::Float colors[detail * 3];

	float t = 0;
	for (int i = 0; i < detail; i++, t += stepSize) {
		const vec3f point = this->position_on_spline(t);
		vertices[i * 3 + 0] = point.x;
		vertices[i * 3 + 1] = point.y;
		vertices[i * 3 + 2] = point.z;

		colors[i * 3 + 0] = 1.0f - t;
		colors[i * 3 + 1] = t;
		colors[i * 3 + 2] = t;
	}

	gl->bindVertexArray(this->debugVao);

	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
	gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * detail * sizeof(gl::GL::Float), vertices);

	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
	gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * detail * sizeof(gl::GL::Float), colors);

	gl->bindVertexArray(0);
}

void Spline::render(const gl::GLapi* gl, const mat44f view, const mat44f proj) {
	gl->useProgram(this->debugProgram);

	gl->bindVertexArray(this->debugVao);
	gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "view"), 1, gl::GL::GLFALSE, view.data());
	gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "proj"), 1, gl::GL::GLFALSE, proj.data());

	gl->drawArrays(gl::GL::LINE_STRIP, 0, detail);

	gl->bindVertexArray(0);
}

inline vec3f Spline::position_on_spline(float t) {
	return t * t * t * this->a + t * t * this->b + t * this->c + this->d;
	// return fml::make_vector<vec3f>(2.0f, 0.0f, 1.0f * t);
}