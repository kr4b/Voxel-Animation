#include <vector>

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;


float map(float current, float min1, float max1, float min2, float max2) {
    return min2 + current / (max1 - min1) * (max2 - min2);
}

Sampler<vec3f> prepare_deformation_scale(const gl::GLapi *gl, const size_t size, const float strength) {
    std::vector<unsigned char> samplers;
    std::vector<vec3f> data;
    std::vector<vec3f> colors;

    samplers.reserve(size * size * size);
    data.reserve(size * size * size);
    colors.reserve(size * size * size);

    for (size_t k = 0; k < size; k++) {
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                unsigned char sample = 1;
                vec3f d = fml::make_zero<vec3f>();
                vec3f c = fml::make_zero<vec3f>();

                if (k > 0 && k < size - 1 && i > 0 && i < size - 1 && j == 0) {
                    d = vec3f(
                        -strength,
                        map(i, 1, size - 1, -strength, strength),
                        map(k, 1, size - 1, -strength, strength));

                    c = vec3f(map(k, 1, size - 1, 1.0f, 0), map(i, 1, size - 1, 0, 1.0f), 0);
                }
                else if (k > 0 && k < size - 1 && i > 0 && i < size - 1 && j == size - 1) {
                    d = vec3f(
                        strength,
                        map(i, 1, size - 1, -strength, strength),
                        map(k, 1, size - 1, -strength, strength));

                    c = vec3f(map(i, 1, size - 1, 1.0f, 0), map(k, 1, size - 1, 1.0f, 0), 0);
                }
                else if (k > 0 && k < size - 1 && j > 0 && j < size - 1 && i == 0) {
                    d = vec3f(
                        map(j, 1, size - 1, -strength, strength),
                        -strength,
                        map(k, 1, size - 1, -strength, strength));

                    c = vec3f(0, map(j, 1, size - 1, 0, 1.0f), map(k, 1, size - 1, 1.0f, 0));
                }
                else if (k > 0 && k < size - 1 && j > 0 && j < size - 1 && i == size - 1) {
                    d = vec3f(
                        map(j, 1, size - 1, -strength, strength),
                        strength,
                        map(k, 1, size - 1, -strength, strength));

                    c = vec3f(0, map(k, 1, size - 1, 1.0f, 0), map(j, 1, size - 1, 0, 1.0f));
                }
                else if (i > 0 && i < size - 1 && j > 0 && j < size - 1 && k == 0) {
                    d = vec3f(
                        map(j, 1, size - 1, -strength, strength),
                        map(i, 1, size - 1, -strength, strength),
                        -strength);

                    c = vec3f(map(j, 1, size - 1, 0, 1.0f), 0, map(i, 1, size - 1, 1.0f, 0));
                }
                else if (i > 0 && i < size - 1 && j > 0 && j < size - 1 && k == size - 1) {
                    d = vec3f(
                        map(j, 1, size - 1, -strength, strength),
                        map(i, 1, size - 1, -strength, strength),
                        strength);

                    c = vec3f(map(i, 1, size - 1, 1.0f, 0), 0, map(j, 1, size - 1, 0, 1.0f));
                }
                else {
                    sample = 0;
                }

                samplers.push_back(sample);
                data.push_back(d);
                colors.push_back(c);
            }
        }
    }

    const vec3f aabbMin = fml::make_splat<vec3f>(-1.0f);
    const vec3f aabbMax = fml::make_splat<vec3f>(1.0f);

    Sampler<vec3f> sampler (
        AABB(aabbMin * 2.0f, aabbMax * 2.0f),
        AABB(aabbMin, aabbMax),
        size,
        samplers,
        data,
        colors,
        [gl](Ray ray, float t, vec3f data, vec3f c) {
            const vec3f P1 = ray.origin;
            const vec3f P2 = P1 + ray.dir * t;

            const vec3f P0 = fml::make_zero<vec3f>();
            const vec3f P3 = data;

            Spline spline(gl);
            spline.parameters_from_tangents(P1, P2, P0, P3);
            spline.set_color(c);
            return spline;
        },
        gl
            );

    sampler.create_sampler_texture();
    sampler.create_data_texture(gl::GL::RGB32F, gl::GL::RGB, gl::GL::FLOAT, data.data());

    return sampler;
}