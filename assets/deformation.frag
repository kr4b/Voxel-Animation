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
    float p;
    float q;
    float root;
    float discriminant;
    float fac;
    float arccos;
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
bool make_spline(in Ray ray, in vec4 data, inout Spline spline);
Spline spline_with_tangents(in vec3 point1, in vec3 point2, in vec3 tangent1, in vec3 tangent2);
Spline spline_with_control_points(in vec3 point1, in vec3 point2, in vec3 control1, in vec3 control2);


/// Overwrite
bool make_spline(in Ray ray, in vec4 data, inout Spline spline) {
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
    spline.a = control1 * -1.0 * tau + point1 * (2.0 - tau) + point2 * (tau - 2.0) + control2 *  tau;
    spline.b = control1 *  2.0 * tau + point1 * (tau - 3.0) + point2 * (3.0 - 2.0 * tau) + control2 * -tau;
    spline.c = control1 * -tau + point2 * tau;
    spline.d = point1;

    return spline;
}

/// Sampler
bool get_spline_from_sampler(in Sampler sampler, in Ray ray, in vec3 samplePos, inout Spline spline) {
    ivec3 position = ivec3(samplePos * sampler.size);

    const uvec4 dataIndex = texelFetch(sampler.dataCheck, position, 0);
    if (dataIndex.r == 0) {
        return false;
    }

    return make_spline(ray, texelFetch(sampler.data[dataIndex.r - 1], position, 0), spline);
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

bool intersect_ray_sampler(in Ray ray, in Sampler sampler, inout Spline spline) {
    vec2 ts = intersect_ray_aabb(ray, sampler.aabb);

    if (ts.x <= ts.y && ts.y >=  0.0) {
        if (ts.x < 0.0) {
            ts.x = 0.0;
        }

        const vec3 wentry = ray.origin + ray.direction * ts.x;
        const vec3 wexit  = ray.origin + ray.direction * ts.y;

        const vec3 vscale = sampler.aabb.max - sampler.aabb.min;
        const vec3 ventry = (wentry - sampler.aabb.min) / vscale;
        const vec3 vexit  = (wexit  - sampler.aabb.min) / vscale;

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

void get_origin_direction(in vec2 fragCoord, out vec3 origin, out vec3 direction) {
	const vec4 hray = vec4(fragCoord * 2.0 - vec2(1.0), 1.0, 1.0);
	const vec4 wray = uCamera.inverseProjCamera * hray;

	origin = uCamera.cameraWorldPos;
	direction = normalize( wray.xyz / wray.w - origin );
}

/// Cubic
float single_real_root(inout Cubic cubic) {
    if (cubic.discriminant > 0.0) {
        const float D = sqrt(cubic.q * cubic.q / 4.0 + cubic.p * cubic.p * cubic.p / 27.0);
        const float C0 = -0.5 * cubic.q + D;
        const float C1 = -0.5 * cubic.q - D;

        cubic.root = sign(C0) * pow(abs(C0), 1.0 / 3.0) + sign(C1) * pow(abs(C1), 1.0 / 3.0);
    }
    return cubic.root;
}

float first_root(inout Cubic cubic){
    if (cubic.discriminant <= 0.0) cubic.root = cubic.fac * cos(cubic.arccos);
    return cubic.root;
}

float second_root(inout Cubic cubic){
    if (cubic.discriminant <= 0.0) cubic.root = cubic.fac * cos(cubic.arccos - 2.0 / 3.0 * M_PI);
    return cubic.root;
}

float third_root(inout Cubic cubic){
    if (cubic.discriminant <= 0.0) cubic.root = cubic.fac * cos(cubic.arccos - 4.0 / 3.0 * M_PI);
    return cubic.root;
}

float calculate_default_root(inout Cubic cubic) {
    if (cubic.discriminant > 0.0) {
        cubic.root = single_real_root(cubic);
    } else {
        cubic.fac = 2.0 * sqrt(-cubic.p / 3.0);
        cubic.arccos = acos(3.0 * cubic.q / (2.0 * cubic.p) * sqrt(-3.0 / cubic.p)) / 3.0;

        cubic.root = second_root(cubic);
    }

    return cubic.root;
}

Cubic make_cubic(in float a, in float b, in float c, in float d) {
    Cubic cubic;
    cubic.p = (3.0 * a * c - b * b) / (3.0 * a * a);
    cubic.q = (2.0 * b * b * b - 9.0 * a * b * c + 27.0 * a * a * d) / (27.0 * a * a * a);

    cubic.discriminant = 27.0 * cubic.q * cubic.q + 4.0 * cubic.p * cubic.p * cubic.p;

    cubic.root = calculate_default_root(cubic);

    return cubic;
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

#define MAX_VALUE 2.0
#define MIN_VALUE -MAX_VALUE

bool calculate_near_far(in vec3 t1, in vec3 t2, in Spline aSpline, in vec3 aAABBMin, in vec3 aAABBMax, inout vec2 ts) {
    const vec3 it1 = intersected_aabb(t1, aSpline, aAABBMin, aAABBMax);
    const vec3 it2 = intersected_aabb(t2, aSpline, aAABBMin, aAABBMax);

    const vec3 nt1 = t1 * it1 + (1.0 - it1) * MAX_VALUE;
    const vec3 nt2 = t2 * it2 + (1.0 - it2) * MAX_VALUE;

    const vec3 ft1 = t1 * it1 + (1.0 - it1) * MIN_VALUE;
    const vec3 ft2 = t2 * it2 + (1.0 - it2) * MIN_VALUE;

    const vec3 inear = min(nt1, nt2);
    const vec3 ifar  = max(ft1, ft2);

    ts.x = min(ts.x, min(inear.x, min(inear.y, inear.z)));
    ts.y = max(ts.y, max(ifar.x,  max(ifar.y, ifar.z)));

    return ts.x < ts.y && ts.y >= 0.0;
}

vec2 intersect_spline_aabb(in Spline aSpline, in vec3 aAABBMin, in vec3 aAABBMax) {
    const vec3 conversion = -aSpline.b / (3.0 * aSpline.a);

    Cubic cubic_min_x = make_cubic(aSpline.a.x, aSpline.b.x, aSpline.c.x, aSpline.d.x - aAABBMin.x);
    Cubic cubic_min_y = make_cubic(aSpline.a.y, aSpline.b.y, aSpline.c.y, aSpline.d.y - aAABBMin.y);
    Cubic cubic_min_z = make_cubic(aSpline.a.z, aSpline.b.z, aSpline.c.z, aSpline.d.z - aAABBMin.z);
    Cubic cubic_max_x = make_cubic(aSpline.a.x, aSpline.b.x, aSpline.c.x, aSpline.d.x - aAABBMax.x);
    Cubic cubic_max_y = make_cubic(aSpline.a.y, aSpline.b.y, aSpline.c.y, aSpline.d.y - aAABBMax.y);
    Cubic cubic_max_z = make_cubic(aSpline.a.z, aSpline.b.z, aSpline.c.z, aSpline.d.z - aAABBMax.z);

    const vec3 t1 = conversion + vec3(
        second_root(cubic_min_x),
        second_root(cubic_min_y),
        second_root(cubic_min_z));
    const vec3 t2 = conversion + vec3(
        second_root(cubic_max_x),
        second_root(cubic_max_y),
        second_root(cubic_max_z));

    vec2 ts = vec2(2.0, -2.0);
    bool result = calculate_near_far(t1, t2, aSpline, aAABBMin, aAABBMax, ts);

    if (!result) {
        const vec3 first_t1 = conversion + vec3(
            first_root(cubic_min_x),
            first_root(cubic_min_y),
            first_root(cubic_min_z));
        const vec3 first_t2 = conversion + vec3(
            first_root(cubic_max_x),
            first_root(cubic_max_y),
            first_root(cubic_max_z));

        result = calculate_near_far(first_t1, first_t2, aSpline, aAABBMin, aAABBMax, ts);

        if (!result) {
            const vec3 third_t1 = conversion + vec3(
                third_root(cubic_min_x),
                third_root(cubic_min_y),
                third_root(cubic_min_z));
            const vec3 third_t2 = conversion + vec3(
                third_root(cubic_max_x),
                third_root(cubic_max_y),
                third_root(cubic_max_z));

            result = calculate_near_far(third_t1, third_t2, aSpline, aAABBMin, aAABBMax, ts);
        }
    }

    return ts;
}

void main() {
    AABB real_aabb;
    real_aabb.min = uVolMeta.volMin;
    real_aabb.max = uVolMeta.volMax;

    vec3 origin;
    vec3 direction;
    get_origin_direction(gl_FragCoord.xy * uCamera.reciprocalWindowSize, origin, direction);

    Ray ray = make_ray(origin, direction);

    Spline spline;
    bool result = intersect_ray_sampler(ray, sampler, spline);

    vec3 col = vec3(0.0);
    if (result == false) {
        spline = spline_with_tangents(ray.origin, ray.origin + ray.direction * length(ray.origin) * 2.0, vec3(0.0), vec3(0.0));
    }

    vec2 ts = intersect_spline_aabb(spline, uVolMeta.volMin * 2.0, uVolMeta.volMax * 2.0);

    // Step through volume
    // Only do this if we hit the volume
    // float depth = 1.0;

    if( result == true /*ts.x <= ts.y && ts.y >= 0.0f*/ ) {
        if (ts.x < 0.0) {
            ts.x = 0.0;
        }

        const vec3 world_entry = position_on_spline(1.0, spline);
        const float epsilon = 1e-2;
        const vec3 dt1 = position_on_spline(1.0 - epsilon, spline);
        const vec3 dt2 = position_on_spline(1.0 - epsilon * 2.0, spline);
        const vec3 dtde = (dt2 - dt1) / (epsilon * 2.0);

        Ray sub_ray;
        sub_ray.origin = world_entry;
        sub_ray.direction = normalize(dtde);

        const vec2 sub_ts = intersect_ray_aabb(sub_ray, real_aabb);
        if (sub_ts.x <= sub_ts.y && sub_ts.y >= 0.0) {
            const vec3 scale = uVolMeta.volMax - uVolMeta.volMin;

            for( int i = 0; i < steps; ++i ) {
                const float ii = float(i) / float(steps);
                const vec3 sub_world_entry = sub_ray.origin + sub_ray.direction * mix(sub_ts.y, sub_ts.x, ii);
                const vec3 samplePos = (sub_world_entry - uVolMeta.volMin) / scale;
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
