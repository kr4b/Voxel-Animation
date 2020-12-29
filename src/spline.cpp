#include "spline.hpp"

const int detail = 100;

const double M_PI = 3.14159265;

const vec3f EPSILON = fml::make_vector<vec3f>(1.0e-6f, 1.0e-6f, 1.0e-6f);
const vec3f ZERO_VECTOR = fml::make_zero<vec3f>();
const vec3f ONE_VECTOR = fml::make_one<vec3f>();
const vec3f MAX_VALUE = fml::make_vector<vec3f>(1.0e6f, 1.0e6f, 1.0e6f);
const vec3f MIN_VALUE = -MAX_VALUE;

template <typename T> inline int sign(T x) {
	return (T(0) < x) - (x < T(0));
}

inline vec3f step(vec3f edge, vec3f x) {
	return fml::make_vector<vec3f>(
		x.x >= edge.x ? 1.0f : 0.0f,
		x.y >= edge.y ? 1.0f : 0.0f,
		x.z >= edge.z ? 1.0f : 0.0f);
}

Spline::Spline(const gl::GLapi* gl) {
	this->intersection = false;
	this->worldEntry = fml::make_zero<vec3f>();
	this->worldExit = fml::make_zero<vec3f>();

	this->lastCursorPos = fml::make_zero<vec2f>();

	this->a = fml::make_zero<vec3f>();
	this->b = fml::make_zero<vec3f>();
	this->c = fml::make_zero<vec3f>();
	this->d = fml::make_zero<vec3f>();

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
	gl->genVertexArrays(1, &this->lineVao);
	gl->bindVertexArray(this->lineVao);

	gl->genBuffers(4, this->buffers);

	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * detail * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(0, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(0);

	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * detail * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(1, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(1);

	gl->genVertexArrays(1, &this->pointsVao);
	gl->bindVertexArray(this->pointsVao);

	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[2]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 2 * 3 * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(0, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(0);

	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[3]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 2 * 3 * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(1, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(1);

	gl->bindVertexArray(0);
}

void Spline::update_from_screen_coords(const gl::GLapi* gl, const vec2f coords, const mat44f inverseProjCamera, const vec3f cameraWorldPos) {
	const vec2f transformed = coords * 2.0f - fml::make_vector<vec2f>(1.0f, 1.0f);
	const vec4f hray = fml::make_vector<vec4f>(transformed.x, transformed.y, 1.0f, 1.0f);
	const vec4f wray = inverseProjCamera * hray;

	const vec3f origin = cameraWorldPos;
	const vec3f direction = fml::normalize(fml::make_vector<vec3f>(wray.x, wray.y, wray.z) / wray.w - origin);

	const vec3f P1 = origin;
	const vec3f P2 = origin + direction * fml::length(origin) * 2.0f;

	const vec3f P0 = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);
	const vec3f P3 = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);

	this->a = 2.0f * P1 - 2.0f * P2 + 1.0f * P0 + 1.0f * P3;
	this->b = -3.0f * P1 + 3.0f * P2 - 2.0f * P0 - 1.0f * P3;
	this->c = P0;
	this->d = P1;
}

void Spline::update_buffers(const gl::GLapi *gl) {
	{
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

		gl->bindVertexArray(this->lineVao);

		gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
		gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * detail * sizeof(gl::GL::Float), vertices);
		gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
		gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * detail * sizeof(gl::GL::Float), colors);

		gl->bindVertexArray(0);
	}

	{
		gl::GL::Float vertices[2 * 3];
		gl::GL::Float colors[2 * 3];

		vertices[0] = worldEntry.x; vertices[1] = worldEntry.y; vertices[2] = worldEntry.z;
		vertices[3] = worldExit.x; vertices[4] = worldExit.y; vertices[5] = worldExit.z;

		colors[0] = 1.0f; colors[1] = 0.0f; colors[2] = 0.0f;
		colors[3] = 0.0f; colors[4] = 0.0f; colors[5] = 1.0f;

		gl->bindVertexArray(this->pointsVao);

		gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[2]);
		gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 2 * 3 * sizeof(gl::GL::Float), vertices);
		gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[3]);
		gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 2 * 3 * sizeof(gl::GL::Float), colors);

		gl->bindVertexArray(0);
	}
}

void Spline::render(const gl::GLapi* gl, const mat44f view, const mat44f proj) {
	gl->useProgram(this->debugProgram);

	gl->bindVertexArray(this->lineVao);
	gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "view"), 1, gl::GL::GLFALSE, view.data());
	gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "proj"), 1, gl::GL::GLFALSE, proj.data());

	gl->drawArrays(gl::GL::LINE_STRIP, 0, detail);

	if (intersection) {
		gl->bindVertexArray(this->pointsVao);
		gl->pointSize(10.0f);
		gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "view"), 1, gl::GL::GLFALSE, view.data());
		gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "proj"), 1, gl::GL::GLFALSE, proj.data());

		gl->drawArrays(gl::GL::POINTS, 0, 3);
		gl->pointSize(1.0f);
	}

	gl->bindVertexArray(0);
}

inline vec3f Spline::position_on_spline(float t) {
	return t * t * t * this->a + t * t * this->b + t * this->c + this->d;
}

void Spline::intersect_spline_aabb(const vec3f aAABBMin, const vec3f aAABBMax) {
	const vec3f conversion = -this->b / (3.0f * this->a);

	const vec3f t1 = conversion + vec3f(
		depressed_cubic(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMin.x),
		depressed_cubic(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMin.y),
		depressed_cubic(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMin.z));
	const vec3f t2 = conversion + vec3f(
		depressed_cubic(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMax.x),
		depressed_cubic(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMax.y),
		depressed_cubic(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMax.z));

	const vec3f it1 = intersected_aabb(t1, this, aAABBMin, aAABBMax);
	const vec3f it2 = intersected_aabb(t2, this, aAABBMin, aAABBMax);

	const vec3f nt1 = t1 * it1 + (ONE_VECTOR - it1) * MAX_VALUE;
	const vec3f nt2 = t2 * it2 + (ONE_VECTOR - it2) * MAX_VALUE;

	const vec3f ft1 = t1 * it1 + (ONE_VECTOR - it1) * MIN_VALUE;
	const vec3f ft2 = t2 * it2 + (ONE_VECTOR - it2) * MIN_VALUE;

	const vec3f inear = fml::make_vector<vec3f>(std::min(nt1.x, nt2.x), std::min(nt1.y, nt2.y), std::min(nt1.z, nt2.z));
	const float near = std::min(inear.x, std::min(inear.y, inear.z));

	const vec3f ifar = fml::make_vector<vec3f>(std::max(ft1.x, ft2.x), std::max(ft1.y, ft2.y), std::max(ft1.z, ft2.z));
	const float far = std::max(ifar.x, std::max(ifar.y, ifar.z));

	this->intersection = near <= far && far >= 0.0f;
	if (this->intersection) {
		this->worldEntry = this->position_on_spline(near);
		this->worldExit = this->position_on_spline(far);
	}
}

float depressed_cubic(float a, float b, float c, float d) {
	const float p = (3.0f * a * c - b * b) / (3.0f * a * a);
	const float q = (2.0f * b * b * b - 9.0f * a * b * c + 27.0f * a * a * d) / (27.0f * a * a * a);

	const float discriminant = q * q + 4.0f * p * p * p / 27.0f;

	if (discriminant > 0.0f) {
		// 1 real root
		const float D = sqrt(q * q / 4.0f + p * p * p / 27.0f);
		const float C0 = -0.5f * q + D;
		const float C1 = -0.5f * q - D;
		const float t0 = sign(C0) * pow(abs(C0), 1.0f / 3.0f) + sign(C1) * pow(abs(C1), 1.0f / 3.0f);

		return t0;
	}
	else {
		// 3 real roots
		const float fac0 = 2.0f * sqrt(-p / 3.0f);
		const float arccos = acos(3.0f * q / (2.0f * p) * sqrt(-3.0f / p)) / 3.0f;

		// const t0 = fac0 * Math.cos(arccos);
		const float t1 = fac0 * cos(arccos - 2.0f * M_PI / 3.0f);
		// const t2 = fac0 * Math.cos(arccos - 4 * M_PI / 3);

		return t1;
	}
}

float point_in_aabb(const vec3f aPoint, const vec3f aAABBMin, const vec3f aAABBMax) {
	const vec3f result = step(aAABBMin - EPSILON, aPoint) * step(aPoint, aAABBMax + EPSILON);
	return result.x * result.y * result.z;
}

vec3f intersected_aabb(const vec3f t, Spline* aSpline, vec3f aAABBMin, vec3f aAABBMax) {
	const vec3f P0 = aSpline->position_on_spline(t.x);
	const vec3f P1 = aSpline->position_on_spline(t.y);
	const vec3f P2 = aSpline->position_on_spline(t.z);

	const vec3f resultT = step(ZERO_VECTOR, t) * step(t, ONE_VECTOR);
	const float result0 = point_in_aabb(P0, aAABBMin, aAABBMax);
	const float result1 = point_in_aabb(P1, aAABBMin, aAABBMax);
	const float result2 = point_in_aabb(P2, aAABBMin, aAABBMax);

	return fml::make_vector<vec3f>(
		resultT.x * result0,
		resultT.y * result1,
		resultT.z * result2);
}