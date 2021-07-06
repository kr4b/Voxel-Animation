#include "plane.hpp"
#include "ray.hpp"

// z = 0: p = (x, -y, 0) | p = (-x, y, 0)
// x = 0: p = (0, y, -z) | p = (0, -y, z)
// y = 0: p = (x, 0, -z) | p = (-x, 0, z)
// p = (x, -y, abs(y) * z + abs(x) * -z)
Plane::Plane(vec3f center, vec3f half_size) : center(center), half_size(half_size) {
    // assert((half_size.x == 0.0f || half_size.y == 0.0f || half_size.z == 0.0f));
    const vec3f p1 = center - half_size;
    const vec3f p2 = center + vec3f(half_size.x, -half_size.y, abs(half_size.y) * half_size.z + abs(half_size.x) * -half_size.z);
    const vec3f p3 = center + half_size;
    const vec3f v = p2 - p1;
    const vec3f w = p3 - p1;
    this->normal = fml::normalize(vec3f(v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z, v.x * w.y - v.y * w.x));
    this->span1 = v;
    this->span2 = p3 - p2;

    this->size = half_size * vec3f(2.0f, 2.0f, 2.0f);

    this->min = this->center - this->half_size;
    this->max = this->center + this->half_size;

    // https://stackoverflow.com/questions/13199126/find-opengl-rotation-matrix-for-a-plane-given-the-normal-vector-after-the-rotat
    vec3f rotationAxis = fml::cross(vec3f(0.0f, 1.0f, 0.0f), this->normal);
    float rotationAngle = acos(fml::dot(vec3f(0.0f, 1.0f, 0.0f), this->normal));

    float cosAngle = cos(rotationAngle);
    float sinAngle = sin(rotationAngle);

    // https://en.wikipedia.org/wiki/Rotation_matrix#Conversion_from_rotation_matrix_and_to_axis%E2%80%93angle
    this->matrix = fml::make_matrix<mat44f>(
        cosAngle + rotationAxis.x * rotationAxis.x * (1.0f - cosAngle),
        rotationAxis.x * rotationAxis.y * (1.0f - cosAngle) - rotationAxis.z * sinAngle,
        rotationAxis.x * rotationAxis.z * (1.0f - cosAngle) + rotationAxis.y * sinAngle,
        this->center.x,

        rotationAxis.y * rotationAxis.x * (1.0f - cosAngle) + rotationAxis.z * sinAngle,
        cosAngle + rotationAxis.y * rotationAxis.y * (1.0f - cosAngle),
        rotationAxis.y * rotationAxis.z * (1.0f - cosAngle) - rotationAxis.x * sinAngle,
        this->center.y,

        rotationAxis.z * rotationAxis.x * (1.0f - cosAngle) - rotationAxis.y * sinAngle,
        rotationAxis.z * rotationAxis.y * (1.0f - cosAngle) + rotationAxis.x * sinAngle,
        cosAngle + rotationAxis.z * rotationAxis.z * (1.0f - cosAngle),
        this->center.z,

        0.0f, 0.0f, 0.0f, 1.0f
    );
    /*
    float hypotXy = hypotf(this->normal.x, this->normal.y);
    this->matrix = fml::make_matrix<mat44f>(
        this->normal.y / hypotXy, -this->normal.x / hypotXy, 0.0f, this->center.x,
        this->normal.x * this->normal.z / hypotXy, this->normal.y * this->normal.z / hypotXy, -hypotXy, this->center.y,
        this->normal.x, this->normal.y, this->normal.z, this->center.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    */
    this->inv_matrix = fml::invert(this->matrix);
}

void Plane::init_vao(const gl::GLapi* gl, const vec3f color) {
    gl->genVertexArrays(1, &this->vao);
    gl->bindVertexArray(this->vao);

    gl->genBuffers(2, this->buffers);

    gl::GL::Float vertices[4 * 3];
    gl::GL::Float colors[4 * 3];

    const vec3f v1 = center - half_size;
    const vec3f v2 = v1 + span1;
    const vec3f v3 = center + half_size;
    const vec3f v4 = v1 + span2;
    vertices[0] = v1.x; vertices[1] = v1.y; vertices[2] = v1.z;
    vertices[3] = v2.x; vertices[4] = v2.y; vertices[5] = v2.z;
    vertices[6] = v3.x; vertices[7] = v3.y; vertices[8] = v3.z;
    vertices[9] = v4.x; vertices[10] = v4.y; vertices[11] = v4.z;

    colors[0] = color.x; colors[1] = color.y; colors[2] = color.z;
    colors[3] = color.x; colors[4] = color.y; colors[5] = color.z;
    colors[6] = color.x; colors[7] = color.y; colors[8] = color.z;
    colors[9] = color.x; colors[10] = color.y; colors[11] = color.z;

    // Vertices
    gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[0]);
    gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * 4 * sizeof(gl::GL::Float), vertices, gl::GL::DYNAMIC_DRAW);
    gl->vertexAttribPointer(0, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
    gl->enableVertexAttribArray(0);

    // Colors
    gl->bindBuffer(gl::GL::ARRAY_BUFFER, this->buffers[1]);
    gl->bufferData(gl::GL::ARRAY_BUFFER, 3 * 4 * sizeof(gl::GL::Float), colors, gl::GL::DYNAMIC_DRAW);
    gl->vertexAttribPointer(1, 3, gl::GL::FLOAT, gl::GL::GLFALSE, 0, 0);
    gl->enableVertexAttribArray(1);

    gl->bindVertexArray(0);
}

void Plane::clean(const gl::GLapi* gl) {
    gl->deleteVertexArrays(1, &this->vao);
    gl->deleteBuffers(2, this->buffers);
}

void Plane::render(const gl::GLapi* gl) {
    gl->bindVertexArray(this->vao);

    gl->lineWidth(3.0f);
    gl->drawArrays(gl::GL::LINE_LOOP, 0, 4);
    gl->lineWidth(1.0f);

    gl->bindVertexArray(0);
}

std::optional<vec2f> Plane::intersect(const Ray& ray) const {
    const float discriminant = dot(this->normal, ray.dir);
    if (abs(discriminant) < 1.0e-6) {
        return std::nullopt;
    }

    const float t = dot(this->center - ray.origin, this->normal) / discriminant;
    if (t < 0.0f) {
        return std::nullopt;
    }

    const vec3f worldPos = ray.origin + ray.dir * t;
    const vec3f samplePos = (worldPos - this->center / this->size + fml::make_splat<vec3f>(1.0f)) * 0.5f;
    vec2f uv;
    if (this->size.x == 0.0f) {
        uv = vec2f(samplePos.z, samplePos.y);
    }
    else if (this->size.y == 0.0f) {
        uv = vec2f(samplePos.x, samplePos.z);
    }
    else {
        uv = vec2f(samplePos.x, samplePos.y);
    }

    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f) {
        return std::nullopt;
    }

    return std::make_optional(uv);
}
