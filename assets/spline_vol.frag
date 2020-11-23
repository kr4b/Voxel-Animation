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

    const vec3 P0 = origin;
    const vec3 P1 = origin;
    const vec3 P2 = origin - direction;
    const vec3 P3 = origin - direction;

    Spline spline;
    spline.a = -0.5 * P0 + 1.5 * P1 - 1.5 * P2 + 0.5 * P3;
    spline.b = P0 - 2.5 * P1 + 2.0 * P2 - 0.5 * P3;
    spline.c = -0.5 * P0 + 0.5 * P2;
    spline.d = P1;

    return spline;
}

// From at^3 + bt^2 + ct + d = 0
// To x^3 + px + q = 0
vec3 depressed_cubic(in vec3 a, in vec3 b, in vec3 c, in vec3 d) {
    const vec3 p = (3.0 * a * c - b * b) / (3.0 * a * a);
    const vec3 q = (2.0 * b * b * b - 9.0 * a * b * c + 27.0 * a * a * d) / (27.0 * a * a * a);

    const vec3 discriminant = -(4.0 * p * p * p + 27.0 * q * q);

    const vec3 _p = p * p * p / 27.0;
    const vec3 _q = q * q / 4.0;

    const vec3 D = sqrt(abs(_q + _p));
    const vec3 C = sign(-0.5 * q + D) * pow(abs(-0.5 * q + D), vec3(1.0 / 3.0));

    return C - p / (3.0 * C);
}

vec3 intersect_spline_axis(in Spline aSpline, in vec3 aDistancesFromOrigin) {
    const vec3 a = aSpline.a;
    const vec3 b = aSpline.b;
    const vec3 c = aSpline.c;
    const vec3 d = aSpline.d - aDistancesFromOrigin;

    return depressed_cubic(a, b, c, d) - b / (3.0 * a);
}

vec2 intersect_spline_aabb(in Spline aSpline, in vec3 aAABBMin, in vec3 aAABBMax) {
    const vec3 t1 = intersect_spline_axis(aSpline, aAABBMin);
    const vec3 t2 = intersect_spline_axis(aSpline, aAABBMax);

    const vec3 mins = min(t1, t2);
    const vec3 maxs = max(t1, t2);

    const float near = max(max(mins.x, mins.y), mins.z);
    const float far = min(min(maxs.x, maxs.y), maxs.z);

    return vec2(near, far);
}

void main() {
    const Spline spline = make_spline(gl_FragCoord.xy * uCamera.reciprocalWindowSize);
    vec2 ts = intersect_spline_aabb(spline, uVolMeta.volMin, uVolMeta.volMax);

	// Step through volume
	// Only do this if we hit the volume
    vec3 col = vec3(0.0);

	if( ts.x <= ts.y && ts.y >= 0.0f ) {
        ts.x = max(0.0, ts.x);

        const vec2 tts = ts * ts;
        const vec2 ttts = tts * ts;

		const vec3 worldEntry = ttts.x * spline.a + tts.x * spline.b + ts.x * spline.c + spline.d;
		const vec3 worldExit = ttts.y * spline.a + tts.y * spline.b + ts.y * spline.c + spline.d;

		const vec3 scale = uVolMeta.volMax - uVolMeta.volMin;
		const vec3 ventry = (worldEntry - uVolMeta.volMin) / scale;
		const vec3 vexit = (worldExit - uVolMeta.volMin) / scale;

		float accum = 0.f;
		for( int i = 0; i < steps; ++i ) {
			const float ii = float(i) / float(steps);
			const vec3 samplePos = mix( ventry, vexit, ii );
			const float voxel = texture( texVol, samplePos ).x;
			
			if (voxel > 0.1f) {
				accum = voxel;
				break;
			}
		}

		// accum /= float(steps);
		col = vec3( accum );
	}
    oColor = col;
}
