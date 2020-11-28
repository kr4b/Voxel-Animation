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

struct Ray {
	vec3 org;
	vec3 dir;
};

struct Spline {
    vec3 a; // t^3
    vec3 b; // t^2
    vec3 c; // t^1
    vec3 d; // t^0
};

struct Plane {
    vec3 normal;
    float D;
};

Spline make_spline(in vec2 aFragCoord) {
	const vec4 hray = vec4(aFragCoord * 2.0 - vec2(1.0), 1.0, 1.0);
	const vec4 wray = uCamera.inverseProjCamera * hray;

	const vec3 origin = uCamera.cameraWorldPos;
	const vec3 direction = normalize( wray.xyz / wray.w - origin );

    const vec3 P1 = origin;
    const vec3 P2 = origin + direction * length(origin) * 2.0f;

    const vec3 P0 = vec3(0.0, 0.0, 0.0);
    const vec3 P3 = vec3(0.0, 0.0, 0.0);

    Spline spline;
    spline.a = 2.0f * P1 - 2.0f * P2 + 1.0f * P0 + 1.0f * P3;
    spline.b = -3.0f * P1 + 3.0f * P2 - 2.0f * P0 - 1.0f * P3;
    spline.c = P0;
    spline.d = P1;

    return spline;
}

#define M_PI 3.14159265

// From at^3 + bt^2 + ct + d = 0
// To x^3 + px + q = 0
float depressed_cubic(in float a, in float b, in float c, in float d) {
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
    } else {
        // 3 real roots
        const float fac0 = 2.0f * sqrt(-p / 3.0f);
        const float arccos = acos(3.0f * q / (2.0f * p) * sqrt(-3.0f / p)) / 3.0f;

        // const t0 = fac0 * Math.cos(arccos);
        const float t1 = fac0 * cos(arccos - 2.0f * M_PI / 3.0f);
        // const t2 = fac0 * Math.cos(arccos - 4 * M_PI / 3);

        return t1;
    }
}

vec3 intersect_spline_axis(in Spline aSpline, in vec3 aDistancesFromOrigin) {
    // const float a = aCoefficients.x;
    // const float b = aCoefficients.y;
    // const float c = aCoefficients.z;
    // const float d = aCoefficients.w - aDistancesFromOrigin;

    return vec3(
        depressed_cubic(aSpline.a.x, aSpline.b.x, aSpline.c.x, aSpline.d.x - aDistancesFromOrigin.x),
        depressed_cubic(aSpline.a.y, aSpline.b.y, aSpline.c.y, aSpline.d.y - aDistancesFromOrigin.y),
        depressed_cubic(aSpline.a.z, aSpline.b.z, aSpline.c.z, aSpline.d.z - aDistancesFromOrigin.z)) - aSpline.b / (3.0f * aSpline.a);
}

#define EPSILON 1e-6f

vec3 intersected_aabb(in vec3 t, in Spline aSpline, in vec3 aAABBMin, in vec3 aAABBMax) {
    const vec3 tt = t * t;
    const vec3 ttt = tt * t;

    const vec3 P0 = ttt.x * aSpline.a + tt.x * aSpline.b + t.x * aSpline.c + aSpline.d;
    const vec3 P1 = ttt.y * aSpline.a + tt.y * aSpline.b + t.y * aSpline.c + aSpline.d;
    const vec3 P2 = ttt.z * aSpline.a + tt.z * aSpline.b + t.z * aSpline.c + aSpline.d;

    const vec3 resultT = step(vec3(0.0), t) * step(t, vec3(1.0));
    const vec3 result0 = step(aAABBMin - EPSILON, P0) * step(P0, aAABBMax + EPSILON);
    const vec3 result1 = step(aAABBMin - EPSILON, P1) * step(P1, aAABBMax + EPSILON);
    const vec3 result2 = step(aAABBMin - EPSILON, P2) * step(P2, aAABBMax + EPSILON);

    return vec3(
        resultT.x * result0.x * result0.y * result0.z,
        resultT.y * result1.x * result1.y * result1.z,
        resultT.z * result2.x * result2.y * result2.z);
}

#define MAX_VALUE 1e6
#define MIN_VALUE -MAX_VALUE

vec2 intersect_spline_aabb(in Spline aSpline, in vec3 aAABBMin, in vec3 aAABBMax) {
    const vec3 conversion = -aSpline.b / (3.0f * aSpline.a);

    const vec3 t1 = conversion + vec3(
        depressed_cubic(aSpline.a.x, aSpline.b.x, aSpline.c.x, aSpline.d.x - aAABBMin.x),
        depressed_cubic(aSpline.a.y, aSpline.b.y, aSpline.c.y, aSpline.d.y - aAABBMin.y),
        depressed_cubic(aSpline.a.z, aSpline.b.z, aSpline.c.z, aSpline.d.z - aAABBMin.z));
    const vec3 t2 = conversion + vec3(
        depressed_cubic(aSpline.a.x, aSpline.b.x, aSpline.c.x, aSpline.d.x - aAABBMax.x),
        depressed_cubic(aSpline.a.y, aSpline.b.y, aSpline.c.y, aSpline.d.y - aAABBMax.y),
        depressed_cubic(aSpline.a.z, aSpline.b.z, aSpline.c.z, aSpline.d.z - aAABBMax.z));

    const vec3 it1 = intersected_aabb(t1, aSpline, aAABBMin, aAABBMax);
    const vec3 it2 = intersected_aabb(t2, aSpline, aAABBMin, aAABBMax);

    const vec3 nt1 = t1 * it1 + (1.0f - it1) * MAX_VALUE;
    const vec3 nt2 = t2 * it2 + (1.0f - it2) * MAX_VALUE;

    const vec3 ft1 = t1 * it1 + (1.0f - it1) * MIN_VALUE;
    const vec3 ft2 = t2 * it2 + (1.0f - it2) * MIN_VALUE;

    const vec3 inear = min(nt1, nt2);
    const float near = min(inear.x, min(inear.y, inear.z));

    const vec3 ifar = max(ft1, ft2);
    const float far = max(ifar.x, max(ifar.y, ifar.z));

    return vec2(near, far);
}

vec3 point_on_spline(float t, Spline spline) {
    return t * t * t * spline.a + t * t * spline.b + t * spline.c + spline.d;
}

void main() {
    const Spline spline = make_spline(gl_FragCoord.xy * uCamera.reciprocalWindowSize);
    vec2 ts = intersect_spline_aabb(spline, uVolMeta.volMin, uVolMeta.volMax);

	// Step through volume
	// Only do this if we hit the volume
    vec3 col = vec3(0.0);

	if( ts.x <= ts.y && ts.y >= 0.0f ) {
        ts.x = max(0.0, ts.x);

		for( int i = 0; i < steps; ++i ) {
			const float ii = float(i) / float(steps);
			const vec3 samplePos = point_on_spline(mix(ts.x, ts.y, ii), spline);
			const float voxel = texture(texVol, samplePos).x;
			
			if (voxel > 0.1f) {
                col = vec3( voxel );
				break;
			}
		}
	}
    oColor = col;
}
