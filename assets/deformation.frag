#version 450

#define VOLUME_STEPS 1024
#define MAX_SAMPLERS 1
#define M_PI 3.14159265

layout( location = 0 ) in vec2 v2fTexCoord;

layout( location = 0 ) out vec3 oColor;

layout( std140, binding = 0 ) uniform UVolumeMeta {
	vec3 volMin;
	vec3 volMax;
} uVolMeta;

layout( std140, binding = 1 ) uniform UCamera {
	mat4 inverseProjCamera;
	vec3 cameraWorldPos;
	vec2 reciprocalWindowSize;
} uCamera;

layout( binding = 0 ) uniform sampler3D texVol;

uniform int steps;
uniform float time;
uniform vec3 tangent1;
uniform vec3 tangent2;
uniform mat4 P2Matrix;

/// Structs
struct Spline {
    vec3 a; // t^3
    vec3 b; // t^2
    vec3 c; // t^1
    vec3 d; // t^0
};

struct Cubic {
    vec3 p;
    vec3 q_1;
    vec3 root;
    vec3 discriminant;
    vec3 fac0;
    vec3 arccos;
};

struct AABB {
    vec3 min;
    vec3 max;
};

struct Ray {
	vec3 origin;
	vec3 direction;
};

struct Sampler {
    usampler3D dataCheck;
    sampler3D data[MAX_SAMPLERS];
    int size;
    AABB aabb;
};

uniform Sampler sampler;

/// Function signatures
bool make_spline(in Ray ray, in vec4 data, out Spline spline);
Spline spline_with_tangents(in vec3 point1, in vec3 point2, in vec3 tangent1, in vec3 tangent2);
Spline spline_with_control_points(in vec3 point1, in vec3 point2, in vec3 control1, in vec3 control2);


/// Overwrite
bool make_spline(in Ray ray, in vec4 data, out Spline spline) {
    const vec3 P1 = ray.origin;
    const vec3 P2 = P1 + ray.direction * 2.0 * length(P1);

    const vec3 P0 = vec3(0.0);
    const vec3 P3 = data.xyz;

    spline = spline_with_tangents(P1, P2, P0, P3);
    return true;
}

/// Spline
Spline spline_with_tangents(in vec3 point1, in vec3 point2, in vec3 tangent1, in vec3 tangent2) {
    Spline spline;
    spline.a = point1 *  2.0 + point2 * -2.0 + tangent1 *  1.0 + tangent2 *  1.0;
    spline.b = point1 * -3.0 + point2 *  3.0 + tangent1 * -2.0 + tangent2 * -1.0;
    spline.c = tangent1;
    spline.d = point1;

    return spline;
}

Spline spline_with_control_points(in vec3 point1, in vec3 point2, in vec3 control1, in vec3 control2) {
    const float tau = 0.2;

    Spline spline;
    spline.a = control1 * -1.0 * tau + point1 * (2.0 - tau) + point2 * (tau - 3.0) + control2 *  tau;
    spline.b = control1 *  2.0 * tau + point1 * (tau - 3.0) + point2 * (3.0 - tau) + control2 * -tau;
    spline.c = control1 * -tau + point2 * tau;
    spline.c = point1;

    return spline;
}

/// Sampler
bool get_spline_from_sampler(in Sampler sampler, in Ray ray, in vec3 samplePos, out Spline spline) {
    const uvec4 dataIndex = texture(sampler.dataCheck, samplePos);
    if (dataIndex.r == 0) {
        return false;
    }

    return make_spline(ray, texture(sampler.data[dataIndex.r - 1], samplePos), spline);
}

/// Ray
Ray make_ray(in vec3 origin, in vec3 direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
}

vec2 intersect_ray_aabb(in Ray ray, in AABB aabb) {
    const vec3 t1 = (aabb.min - ray.origin) / ray.direction;
    const vec3 t2 = (aabb.max - ray.origin) / ray.direction;

    const vec3 mins = min(t1, t2);
    const vec3 maxs = max(t1, t2);
    
    const float near = max(mins.x, (max(mins.y, mins.z)));
    const float far  = min(maxs.x, (min(maxs.y, maxs.z)));

    return vec2(near, far);
}

bool intersect_ray_sampler(in Ray ray, in Sampler sampler, out Spline spline) {
    vec2 ts = intersect_ray_aabb(ray, sampler.aabb);

    if (ts.x <= ts.y && ts.y >=  0.0) {
        ts.x = max(ts.x, 0.0);

        const vec3 wentry = ray.origin + ray.direction * ts.x;
        const vec3 wexit  = ray.origin + ray.direction * ts.y;

        const vec3 vscale = sampler.aabb.max - sampler.aabb.min;
        const vec3 ventry = (wentry - sampler.aabb.min) / vscale;
        const vec3 vexit  = (wexit  - sampler.aabb.max) / vscale;

        for (int i = 0; i < VOLUME_STEPS; i++) {
			const float ii = float(i) / float(VOLUME_STEPS);
            const vec3 samplePos = mix(ventry, vexit, ii);

            if (get_spline_from_sampler(sampler, ray, samplePos, spline)) {
                return true;
            }
        }
    }
    
    return false;
}

void get_origin_direcion(in vec2 fragCoord, out vec3 origin, out vec3 direction) {
	const vec4 hray = vec4(fragCoord * 2.0 - vec2(1.0), 1.0, 1.0);
	const vec4 wray = uCamera.inverseProjCamera * hray;

	origin = uCamera.cameraWorldPos;
	direction = normalize( wray.xyz / wray.w - origin );
}

/// Cubic
Cubic make_cubic(in Spline spline) {
    Cubic cubic;
    cubic.p = (3.0f * spline.a * spline.c - spline.b * spline.b) / (3.0f * spline.a * spline.a);
    cubic.q_1 = (2.0f * spline.b * spline.b * spline.b - 9.0f * spline.a * spline.b * spline.c) / (27.0f * spline.a * spline.a * spline.a);
    return cubic;
}

float single_real_root(in float p, in float q) {
    const float D = sqrt(q * q / 4.0f + p * p * p / 27.0f);
    const float C0 = -0.5f * q + D;
    const float C1 = -0.5f * q - D;
    const float t0 = sign(C0) * pow(abs(C0), 1.0f / 3.0f) + sign(C1) * pow(abs(C1), 1.0f / 3.0f);

    return t0;
}

void three_real_root_variables(in float p, in float q, out float fac0, out float arccos) {
    fac0 = 2.0f * sqrt(-p / 3.0f);
    arccos = acos(3.0f * q / (2.0f * p) * sqrt(-3.0f / p)) / 3.0f;
}

float first_real_root(in float fac0, in float arccos) {
    return fac0 * cos(arccos);
}

float second_real_root(in float fac0, in float arccos) {
    return fac0 * cos(arccos - 2.0f * M_PI / 3.0f);
}

float third_real_root(in float fac0, in float arccos) {
    return fac0 * cos(arccos - 4.0f * M_PI / 3.0f);
}

float solve_first_cubic(in float discriminant, in float root, in float fac0, in float arccos) {
    if (discriminant > 0.0f) {
        return root;
    } else {
        return first_real_root(fac0, arccos);
    }
}

float solve_third_cubic(in float discriminant, in float root, in float fac0, in float arccos) {
    if (discriminant > 0.0f) {
        return root;
    } else {
        return third_real_root(fac0, arccos);
    }
}

float solve_cubic(in float discriminant, in float p, in float q, out float root, out float fac0, out float arccos) {
    if (discriminant > 0.0f) {
        root = single_real_root(p, q);
        return root;
    } else {
        three_real_root_variables(p, q, fac0, arccos);

        return second_real_root(fac0, arccos);
    }
}


// From at^3 + bt^2 + ct + d = 0
// To x^3 + px + q = 0
vec3 depressed_cubic(inout Cubic cubic, in Spline spline, in vec3 offset) {
    const vec3 q = cubic.q_1 + (spline.d - offset) / spline.a;
    cubic.discriminant = 27.0f * q * q + 4.0f * cubic.p * cubic.p * cubic.p;

    return vec3(
        solve_cubic(cubic.discriminant.x, cubic.p.x, q.x, cubic.root.x, cubic.fac0.x, cubic.arccos.x),
        solve_cubic(cubic.discriminant.y, cubic.p.y, q.y, cubic.root.y, cubic.fac0.y, cubic.arccos.y),
        solve_cubic(cubic.discriminant.z, cubic.p.z, q.z, cubic.root.z, cubic.fac0.z, cubic.arccos.z));
}

vec3 position_on_spline(in float t, in Spline spline) {
    return t * t * t * spline.a + t * t * spline.b + t * spline.c + spline.d;
}

#define EPSILON 1e-2f

float point_in_aabb(in vec3 aPoint, in vec3 aAABBMin, in vec3 aAABBMax) {
    const vec3 result = step(aAABBMin - EPSILON, aPoint) * step(aPoint, aAABBMax + EPSILON);
    return result.x * result.y * result.z;
}

vec3 intersected_aabb(in vec3 t, in Spline aSpline, in vec3 aAABBMin, in vec3 aAABBMax) {
    const vec3 P0 = position_on_spline(t.x, aSpline);
    const vec3 P1 = position_on_spline(t.y, aSpline);
    const vec3 P2 = position_on_spline(t.z, aSpline);

    const vec3 resultT = step(vec3(0.0), t) * step(t, vec3(1.0));
    const float result0 = point_in_aabb(P0, aAABBMin, aAABBMax);
    const float result1 = point_in_aabb(P1, aAABBMin, aAABBMax);
    const float result2 = point_in_aabb(P2, aAABBMin, aAABBMax);

    return vec3(
        resultT.x * result0,
        resultT.y * result1,
        resultT.z * result2);
}

#define MAX_VALUE 2.0f
#define MIN_VALUE -MAX_VALUE

void determine_new_near_far(in vec3 t1, in vec3 t2, inout float near, inout float far, in Spline aSpline, in vec3 aAABBMin, in vec3 aAABBMax) {
    const vec3 it1 = intersected_aabb(t1, aSpline, aAABBMin, aAABBMax);
    const vec3 it2 = intersected_aabb(t2, aSpline, aAABBMin, aAABBMax);

    const vec3 nt1 = t1 * it1 + (1.0f - it1) * MAX_VALUE;
    const vec3 nt2 = t2 * it2 + (1.0f - it2) * MAX_VALUE;

    const vec3 ft1 = t1 * it1 + (1.0f - it1) * MIN_VALUE;
    const vec3 ft2 = t2 * it2 + (1.0f - it2) * MIN_VALUE;

    const vec3 inear = min(nt1, nt2);
    near = min(near, min(inear.x, min(inear.y, inear.z)));

    const vec3 ifar = max(ft1, ft2);
    far = max(far, max(ifar.x, max(ifar.y, ifar.z)));
}

vec2 intersect_spline_aabb(in Spline aSpline, in vec3 aAABBMin, in vec3 aAABBMax) {
    const vec3 conversion = -aSpline.b / (3.0f * aSpline.a);

    Cubic cubic_min = make_cubic(aSpline);
    Cubic cubic_max = cubic_min;
    const vec3 t1 = conversion + depressed_cubic(cubic_min, aSpline, aAABBMin);
    const vec3 t2 = conversion + depressed_cubic(cubic_max, aSpline, aAABBMax);

    const vec3 it1 = intersected_aabb(t1, aSpline, aAABBMin, aAABBMax);
    const vec3 it2 = intersected_aabb(t2, aSpline, aAABBMin, aAABBMax);

    const vec3 nt1 = t1 * it1 + (1.0f - it1) * MAX_VALUE;
    const vec3 nt2 = t2 * it2 + (1.0f - it2) * MAX_VALUE;

    const vec3 ft1 = t1 * it1 + (1.0f - it1) * MIN_VALUE;
    const vec3 ft2 = t2 * it2 + (1.0f - it2) * MIN_VALUE;

    const vec3 inear = min(nt1, nt2);
    float near = min(inear.x, min(inear.y, inear.z));

    const vec3 ifar = max(ft1, ft2);
    float far = max(ifar.x, max(ifar.y, ifar.z));

    // p1: (-2.469688, 11.644688, 2.899517)
    // p2: (3.919728, 11.687952, 2.755641)
    // p3: (-2.469688, 1.856308, 2.899517)
    // p4: (3.919728, 1.478349, 0.658754)
    // p5: (-2.469688, 0.207449, 2.899517)
    // p6: (3.919728, 0.542143, -0.531658)

    if (near == far || near > 1.0 || far < 0.0) {
        const vec3 first_t1 = conversion + vec3(
            solve_first_cubic(cubic_min.discriminant.x, cubic_min.root.x, cubic_min.fac0.x, cubic_min.arccos.x),
            solve_first_cubic(cubic_min.discriminant.y, cubic_min.root.y, cubic_min.fac0.y, cubic_min.arccos.y),
            solve_first_cubic(cubic_min.discriminant.z, cubic_min.root.z, cubic_min.fac0.z, cubic_min.arccos.z));
        const vec3 first_t2 = conversion + vec3(
            solve_first_cubic(cubic_max.discriminant.x, cubic_max.root.x, cubic_max.fac0.x, cubic_max.arccos.x),
            solve_first_cubic(cubic_max.discriminant.y, cubic_max.root.y, cubic_max.fac0.y, cubic_max.arccos.y),
            solve_first_cubic(cubic_max.discriminant.z, cubic_max.root.z, cubic_max.fac0.z, cubic_max.arccos.z));

        determine_new_near_far(first_t1, first_t2, near, far, aSpline, aAABBMin, aAABBMax);

        if (near == far || near > 1.0 || far < 0.0) {
            const vec3 third_t1 = conversion + vec3(
                solve_third_cubic(cubic_min.discriminant.x, cubic_min.root.x, cubic_min.fac0.x, cubic_min.arccos.x),
                solve_third_cubic(cubic_min.discriminant.y, cubic_min.root.y, cubic_min.fac0.y, cubic_min.arccos.y),
                solve_third_cubic(cubic_min.discriminant.z, cubic_min.root.z, cubic_min.fac0.z, cubic_min.arccos.z));
            const vec3 third_t2 = conversion + vec3(
                solve_third_cubic(cubic_max.discriminant.x, cubic_max.root.x, cubic_max.fac0.x, cubic_max.arccos.x),
                solve_third_cubic(cubic_max.discriminant.y, cubic_max.root.y, cubic_max.fac0.y, cubic_max.arccos.y),
                solve_third_cubic(cubic_max.discriminant.z, cubic_max.root.z, cubic_max.fac0.z, cubic_max.arccos.z));

            determine_new_near_far(third_t1, third_t2, near, far, aSpline, aAABBMin, aAABBMax);
        }
    }                                                        
                                                             
    return vec2(near, far);
}

void main() {

    vec3 origin;
    vec3 direction;
    get_origin_direcion(gl_FragCoord.xy * uCamera.reciprocalWindowSize, origin, direction);

    Ray ray = make_ray(origin, direction);

    Spline spline;
    bool result = intersect_ray_sampler(ray, sampler, spline);

    vec3 col = vec3(0.0);
    if (result == true) {
        vec2 ts = intersect_spline_aabb(spline, uVolMeta.volMin, uVolMeta.volMax);

        // Step through volume
        // Only do this if we hit the volume
        // float depth = 1.0;

        if( ts.x <= ts.y && ts.y >= 0.0f ) {
            ts.x = max(0.0, ts.x);

            const vec3 scale = uVolMeta.volMax - uVolMeta.volMin;
            for( int i = 0; i < steps; ++i ) {
                const float ii = float(i) / float(steps);
                const vec3 splinePos = position_on_spline(mix(ts.x, ts.y, ii), spline);
                const vec3 samplePos = (splinePos - uVolMeta.volMin) / scale;
                const float voxel = texture(texVol, samplePos).x;
                
                if (voxel > 0.1f) {
                    col = samplePos;
                    // depth = length(splinePos - uCamera.cameraWorldPos) / length(uCamera.cameraWorldPos) * 0.5;
                    break;
                }
            }
        }
	}

    oColor = col;
}
