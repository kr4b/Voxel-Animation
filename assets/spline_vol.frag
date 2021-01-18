#version 450

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

Spline make_spline(in vec2 aFragCoord) {
	const vec4 hray = vec4(aFragCoord * 2.0 - vec2(1.0), 1.0, 1.0);
	const vec4 wray = uCamera.inverseProjCamera * hray;

	const vec3 origin = uCamera.cameraWorldPos;
	const vec3 direction = normalize( wray.xyz / wray.w - origin );

    const vec3 P1 = origin;
    vec3 P2 = origin + (P2Matrix * vec4(direction, 1.0f)).xyz * length(origin) * 2.0f;
    // float tmp = P2.y;
    // P2.y = P2.z;
    // P2.z = tmp - 0.5f;

    const vec3 P0 = tangent1;
    const vec3 P3 = tangent2;

    Spline spline;
    spline.a = 2.0f * P1 - 2.0f * P2 + 1.0f * P0 + 1.0f * P3;
    spline.b = -3.0f * P1 + 3.0f * P2 - 2.0f * P0 - 1.0f * P3;
    spline.c = P0;
    spline.d = P1;

    return spline;
}

#define M_PI 3.14159265

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
        solve_cubic(cubic.discriminant.x, cubic.p.x, q.y, cubic.root.x, cubic.fac0.x, cubic.arccos.x),
        solve_cubic(cubic.discriminant.y, cubic.p.y, q.y, cubic.root.y, cubic.fac0.y, cubic.arccos.y),
        solve_cubic(cubic.discriminant.z, cubic.p.z, q.y, cubic.root.z, cubic.fac0.z, cubic.arccos.z));
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

    if (near == far) {
        const vec3 first_t1 = vec3(
            solve_first_cubic(cubic_min.discriminant.x, cubic_min.root.x, cubic_min.fac0.x, cubic_min.arccos.x),
            solve_first_cubic(cubic_min.discriminant.y, cubic_min.root.y, cubic_min.fac0.y, cubic_min.arccos.y),
            solve_first_cubic(cubic_min.discriminant.z, cubic_min.root.z, cubic_min.fac0.z, cubic_min.arccos.z));
        const vec3 first_t2 = vec3(
            solve_first_cubic(cubic_max.discriminant.x, cubic_max.root.x, cubic_max.fac0.x, cubic_max.arccos.x),
            solve_first_cubic(cubic_max.discriminant.y, cubic_max.root.y, cubic_max.fac0.y, cubic_max.arccos.y),
            solve_first_cubic(cubic_max.discriminant.z, cubic_max.root.z, cubic_max.fac0.z, cubic_max.arccos.z));

        determine_new_near_far(first_t1, first_t2, near, far, aSpline, aAABBMin, aAABBMax);

        if (near == far) {
            const vec3 third_t1 = vec3(
                solve_third_cubic(cubic_min.discriminant.x, cubic_min.root.x, cubic_min.fac0.x, cubic_min.arccos.x),
                solve_third_cubic(cubic_min.discriminant.y, cubic_min.root.y, cubic_min.fac0.y, cubic_min.arccos.y),
                solve_third_cubic(cubic_min.discriminant.z, cubic_min.root.z, cubic_min.fac0.z, cubic_min.arccos.z));
            const vec3 third_t2 = vec3(
                solve_third_cubic(cubic_max.discriminant.x, cubic_max.root.x, cubic_max.fac0.x, cubic_max.arccos.x),
                solve_third_cubic(cubic_max.discriminant.y, cubic_max.root.y, cubic_max.fac0.y, cubic_max.arccos.y),
                solve_third_cubic(cubic_max.discriminant.z, cubic_max.root.z, cubic_max.fac0.z, cubic_max.arccos.z));

            determine_new_near_far(third_t1, third_t2, near, far, aSpline, aAABBMin, aAABBMax);
        }
    }                                                        
                                                             
    return vec2(near, far);
}

void main() {
    const Spline spline = make_spline(gl_FragCoord.xy * uCamera.reciprocalWindowSize);
    vec2 ts = intersect_spline_aabb(spline, uVolMeta.volMin, uVolMeta.volMax);

	// Step through volume
	// Only do this if we hit the volume
    vec3 col = vec3(0.0);
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

    oColor = col;
    // gl_FragDepth = depth;
}
