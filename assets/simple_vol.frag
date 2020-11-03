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

struct Ray {
	vec3 org;
	vec3 dir;
};

Ray make_ray( vec2 aFragCoord ) {
	const vec4 hray = vec4( aFragCoord * 2.0 - vec2(1.0), 1.0, 1.0 );
	const vec4 wray = uCamera.inverseProjCamera * hray;

	Ray ray;
	ray.org = uCamera.cameraWorldPos;
	ray.dir = normalize( wray.xyz / wray.w - ray.org );
	return ray;
}

vec2 intersect_ray_aabb( in Ray aRay, in vec3 aAABBMin, in vec3 aAABBMax ) {
	const vec3 t1 = (aAABBMin - aRay.org) / aRay.dir;
	const vec3 t2 = (aAABBMax - aRay.org) / aRay.dir;
	
	const vec3 mins = min( t1, t2 );
	const vec3 maxs = max( t1, t2 );

	const float near = max( max( mins.x, mins.y ), mins.z );
	const float far = min( min( maxs.x, maxs.y ), maxs.z );
	return vec2( near, far );
};

// #define VOLUME_STEPS 64

void main() {
	// Create ray and intesect with the volume's bounding box
	const Ray ray = make_ray( gl_FragCoord.xy * uCamera.reciprocalWindowSize );
	vec2 ts = intersect_ray_aabb( ray, uVolMeta.volMin, uVolMeta.volMax );

	// Step through volume
	// Only do this if we hit the volume
  vec3 col = vec3( 0.f );

	if( ts.x <= ts.y && ts.y >= 0.0f ) {
		if (ts.x < 0.0f) {
			ts.x = 0.0f;
		}
		const vec3 worldEntry = ray.org + ts.x * ray.dir;
		const vec3 worldExit = ray.org + ts.y * ray.dir;

		const vec3 scale = uVolMeta.volMax - uVolMeta.volMin;
		const vec3 ventry = (worldEntry - uVolMeta.volMin) / scale;
		const vec3 vexit = (worldExit - uVolMeta.volMin) / scale;

		float accum = 0.f;
		for( int i = 0; i < steps; ++i ) {
			const float ii = float(i) / float(steps);
			const vec3 samplePos = mix( ventry, vexit, ii );
			accum += texture( texVol, samplePos ).x;
		}

		accum /= float(steps);
		col = vec3( accum );
	}

    oColor = col;
}
