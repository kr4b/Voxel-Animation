#include "spline.hpp"
#include "depressed_cubic.hpp"

#define POINT_COUNT 21

Spline::Spline(const gl::GLapi* gl) {
	this->intersection = false;
	this->worldEntry = fml::make_zero<vec3f>();
	this->worldExit = fml::make_zero<vec3f>();

	this->lastCursorPos = fml::make_zero<vec2f>();

	this->a = fml::make_zero<vec3f>();
	this->b = fml::make_zero<vec3f>();
	this->c = fml::make_zero<vec3f>();
	this->d = fml::make_zero<vec3f>();
    this->start = fml::make_zero<vec3f>();

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

    // Spline vertices
	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * detail * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(0, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(0);

    // Spline colors
	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * detail * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(1, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(1);

	gl->genVertexArrays(1, &this->pointsVao);
	gl->bindVertexArray(this->pointsVao);

    // Point vertices
	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[2]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * POINT_COUNT * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(0, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(0);

    // Point colors
	gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[3]);
	gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * POINT_COUNT * sizeof(gl::GL::Float), nullptr, gl::GL::DYNAMIC_DRAW);
	gl->vertexAttribPointer(1, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
	gl->enableVertexAttribArray(1);

	gl->bindVertexArray(0);
}

void Spline::update_from_screen_coords(const gl::GLapi* gl, const vec2f coords, const mat44f inverseProjCamera, const vec3f cameraWorldPos, const vec3f tangent1, const vec3f tangent2) {
	const vec2f transformed = coords * 2.0f - fml::make_vector<vec2f>(1.0f, 1.0f);
	const vec4f hray = fml::make_vector<vec4f>(transformed.x, transformed.y, 1.0f, 1.0f);
	const vec4f wray = inverseProjCamera * hray;

	const vec3f origin = cameraWorldPos;
	const vec3f direction = fml::normalize(fml::make_vector<vec3f>(wray.x, wray.y, wray.z) / wray.w - origin);
    this->start = origin + direction * 0.015f;

	const vec3f P1 = origin;
	vec3f P2 = origin + direction * fml::length(origin) * 2.0f;
	float tmp = P2.y;
	P2.y = P2.z;
	P2.z = tmp - 0.5f;

	const vec3f P0 = tangent1;
	const vec3f P3 = tangent2;

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

            colors[i * 3 + 0] = 1.0f;
			colors[i * 3 + 1] = 1.0f;
			colors[i * 3 + 2] = 1.0f;
		}

		gl->bindVertexArray(this->lineVao);

		gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
		gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * detail * sizeof(gl::GL::Float), vertices);
		gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
		gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * detail * sizeof(gl::GL::Float), colors);

		gl->bindVertexArray(0);
	}

	{
		gl::GL::Float vertices[3 * POINT_COUNT];
		gl::GL::Float colors[3 * POINT_COUNT];

        vertices[0] = this->start.x; vertices[1] = this->start.y; vertices[2] = this->start.z;
        colors[0] = 0.0f; colors[1] = 1.0f; colors[2] = 0.0f;

        auto random = []() {
            return float(rand()) / float(RAND_MAX);
        };

        for (int i = 0; i < POINT_COUNT - 1; i++) {
		    vertices[3 + i * 3] = this->points[i].x; vertices[4 + i * 3] = this->points[i].y; vertices[5 + i * 3] = this->points[i].z;
		    colors[3 + i * 3] = random(); colors[4 + i * 3] = random(); colors[5 + i * 3] = random();
        }

        colors[(POINT_COUNT - 2) * 3] = 1.0f; colors[(POINT_COUNT - 2) * 3 + 1] = 0.0f; colors[(POINT_COUNT - 2) * 3 + 2] = 0.0f;
        colors[(POINT_COUNT - 1) * 3] = 1.0f; colors[(POINT_COUNT - 1) * 3 + 1] = 0.0f; colors[(POINT_COUNT - 1) * 3 + 2] = 0.0f;

		gl->bindVertexArray(this->pointsVao);

		gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[2]);
		gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * POINT_COUNT * sizeof(gl::GL::Float), vertices);
		gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[3]);
		gl->bufferSubData(gl::GL::ARRAY_BUFFER, 0, 3 * POINT_COUNT * sizeof(gl::GL::Float), colors);

		gl->bindVertexArray(0);
	}
}

void Spline::render(const gl::GLapi* gl, const mat44f view, const mat44f proj) {
	gl->useProgram(this->debugProgram);

	gl->bindVertexArray(this->lineVao);
	gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "view"), 1, gl::GL::GLFALSE, view.data());
	gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "proj"), 1, gl::GL::GLFALSE, proj.data());

    gl->lineWidth(3.0f);
	gl->drawArrays(gl::GL::LINE_STRIP, 0, detail);
    gl->lineWidth(1.0f);

    gl->bindVertexArray(this->pointsVao);
    gl->pointSize(10.0f);
    gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "view"), 1, gl::GL::GLFALSE, view.data());
    gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "proj"), 1, gl::GL::GLFALSE, proj.data());

    gl->drawArrays(gl::GL::POINTS, 0, intersection ? POINT_COUNT : 1);
    gl->pointSize(1.0f);

	gl->bindVertexArray(0);
}

inline vec3f Spline::position_on_spline(float t) {
	return t * t * t * this->a + t * t * this->b + t * this->c + this->d;
}

void Spline::intersect_spline_aabb(const vec3f aAABBMin, const vec3f aAABBMax) {
	const vec3f conversion = -this->b / (3.0f * this->a);
	
	const vec3f t1 = conversion + vec3f(
		DepressedCubic::find_roots_static(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMin.x),
		DepressedCubic::find_roots_static(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMin.y),
		DepressedCubic::find_roots_static(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMin.z));
	const vec3f t2 = conversion + vec3f(
		DepressedCubic::find_roots_static(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMax.x),
		DepressedCubic::find_roots_static(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMax.y),
		DepressedCubic::find_roots_static(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMax.z));

	vec2f ts = fml::make_zero<vec2f>();
	calculate_near_far(t1, t2, aAABBMin, aAABBMax, &ts);

	this->intersection = ts.x <= ts.y && ts.y >= 0.0f;
	if (this->intersection) {
		this->worldEntry = this->position_on_spline(ts.x);
		this->worldExit = this->position_on_spline(ts.y);
		std::cout << ts.x << ", " << ts.y << std::endl;
	}

    const vec3f p1 = conversion + vec3f(
        DepressedCubic::find_roots_first(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMin.x),
        DepressedCubic::find_roots_first(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMin.y),
        DepressedCubic::find_roots_first(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMin.z));
    const vec3f p2 = conversion + vec3f(
        DepressedCubic::find_roots_first(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMax.x),
        DepressedCubic::find_roots_first(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMax.y),
        DepressedCubic::find_roots_first(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMax.z));

    const vec3f p3 = conversion + vec3f(
        DepressedCubic::find_roots_second(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMin.x),
        DepressedCubic::find_roots_second(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMin.y),
        DepressedCubic::find_roots_second(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMin.z));
    const vec3f p4 = conversion + vec3f(
        DepressedCubic::find_roots_second(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMax.x),
        DepressedCubic::find_roots_second(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMax.y),
        DepressedCubic::find_roots_second(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMax.z));

    const vec3f p5 = conversion + vec3f(
        DepressedCubic::find_roots_third(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMin.x),
        DepressedCubic::find_roots_third(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMin.y),
        DepressedCubic::find_roots_third(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMin.z));
    const vec3f p6 = conversion + vec3f(
        DepressedCubic::find_roots_third(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMax.x),
        DepressedCubic::find_roots_third(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMax.y),
        DepressedCubic::find_roots_third(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMax.z));

    std::vector<float> tVector;
    printf("p1: (%f, %f, %f)\n", p1.x, p1.y, p1.z);
    tVector.push_back(p1.x);
    tVector.push_back(p1.y);
    tVector.push_back(p1.z);
    printf("p2: (%f, %f, %f)\n", p2.x, p2.y, p2.z);
    tVector.push_back(p2.x);
    tVector.push_back(p2.y);
    tVector.push_back(p2.z);
    printf("p3: (%f, %f, %f)\n", p3.x, p3.y, p3.z);
    tVector.push_back(p3.x);
    tVector.push_back(p3.y);
    tVector.push_back(p3.z);
    printf("p4: (%f, %f, %f)\n", p4.x, p4.y, p4.z);
    tVector.push_back(p4.x);
    tVector.push_back(p4.y);
    tVector.push_back(p4.z);
    printf("p5: (%f, %f, %f)\n", p5.x, p5.y, p5.z);
    tVector.push_back(p5.x);
    tVector.push_back(p5.y);
    tVector.push_back(p5.z);
    printf("p6: (%f, %f, %f)\n", p6.x, p6.y, p6.z);
    tVector.push_back(p6.x);
    tVector.push_back(p6.y);
    tVector.push_back(p6.z);

    this->points.clear();
    for (float t : tVector) {
        this->points.push_back(this->position_on_spline(t));
    }
    this->points.push_back(this->worldEntry);
    this->points.push_back(this->worldExit);
}

void Spline::calculate_near_far(const vec3f t1, const vec3f t2, const vec3f aAABBMin, const vec3f aAABBMax, vec2f* ts) {
	const vec3f it1 = this->intersected_aabb(t1, aAABBMin, aAABBMax);
	const vec3f it2 = this->intersected_aabb(t2, aAABBMin, aAABBMax);

	const vec3f nt1 = t1 * it1 + (ONE_VECTOR - it1) * MAX_VALUE;
	const vec3f nt2 = t2 * it2 + (ONE_VECTOR - it2) * MAX_VALUE;

	const vec3f ft1 = t1 * it1 + (ONE_VECTOR - it1) * MIN_VALUE;
	const vec3f ft2 = t2 * it2 + (ONE_VECTOR - it2) * MIN_VALUE;

	const vec3f inear = fml::make_vector<vec3f>(std::min(nt1.x, nt2.x), std::min(nt1.y, nt2.y), std::min(nt1.z, nt2.z));
	ts->x = std::min(inear.x, std::min(inear.y, inear.z));

	const vec3f ifar = fml::make_vector<vec3f>(std::max(ft1.x, ft2.x), std::max(ft1.y, ft2.y), std::max(ft1.z, ft2.z));
	ts->y = std::max(ifar.x, std::max(ifar.y, ifar.z));
}

vec3f Spline::intersected_aabb(const vec3f t, vec3f aAABBMin, vec3f aAABBMax) {
	const vec3f P0 = this->position_on_spline(t.x);
	const vec3f P1 = this->position_on_spline(t.y);
	const vec3f P2 = this->position_on_spline(t.z);

	const vec3f resultT = step(ZERO_VECTOR, t) * step(t, ONE_VECTOR);
	const float result0 = point_in_aabb(P0, aAABBMin, aAABBMax);
	const float result1 = point_in_aabb(P1, aAABBMin, aAABBMax);
	const float result2 = point_in_aabb(P2, aAABBMin, aAABBMax);

	return fml::make_vector<vec3f>(
		resultT.x * result0,
		resultT.y * result1,
		resultT.z * result2);
}

float point_in_aabb(const vec3f aPoint, const vec3f aAABBMin, const vec3f aAABBMax) {
	const vec3f result = step(aAABBMin - EPSILON, aPoint) * step(aPoint, aAABBMax + EPSILON);
	return result.x * result.y * result.z;
}