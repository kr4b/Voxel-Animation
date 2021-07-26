#version 450

// Constants
#define MAX_SPLINES 5
#define VOLUME_STEPS 1024
#define MAX_SAMPLERS 1
#define M_PI 3.14159265
#define EPSILON 1e-2f
#define MAX_VALUE 2.0
#define MIN_VALUE -MAX_VALUE

layout( location = 0 ) in vec2 v2fTexCoord;

layout( location = 0 ) out vec3 oColor;

layout( std140, binding = 0 ) uniform UVolume {
    vec3 volMin;
    vec3 volMax;
} uVolume;

layout( std140, binding = 1 ) uniform UCamera {
	mat4 inverseProjCamera;
	vec3 cameraWorldPos;
	vec2 reciprocalWindowSize;
} uCamera;

layout( binding = 0 ) uniform sampler3D texVol;

uniform float step_size;
uniform float threshold;

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

struct Plane {
    vec3 center;
    vec3 half_size;
    vec3 size;
    vec3 normal;
    vec3 span1, span2;
    vec3 min, max;
    vec3 transformed_min, transformed_max;

    mat4 matrix;
    mat4 inv_matrix;
};

struct Ray {
	vec3 origin;
	vec3 direction;
    vec3 inv_dir;
};

struct SplineChain {
    Spline splines[MAX_SPLINES];
    float amount;
};

struct SplineMap {
    AABB aabb;
    Plane base;
    SplineChain spline_chain;
    SplineChain transformed_spline_chain;

    float size_squared;

    float width;
    float height;
};

layout( std140, binding = 2 ) uniform USplineMap {
    SplineMap spline_map;
} uSplineMap;

// Function Signatures
///////////////////////////////////////
//   _____         _  _               //
//  / ____|       | |(_)              //
// | (___   _ __  | | _  _ __    ___  //
//  \___ \ | '_ \ | || || '_ \  / _ \ //
//  ____) || |_) || || || | | ||  __/ //
// |_____/ | .__/ |_||_||_| |_| \___| //
//         | |                        //
//         |_|                        //
//                                    //
////////////////////////////////////////
void spline_get_extremes(in Spline spline, float extremes[8]);
vec3 position_on_spline(in Spline spline, in float t);
bool intersect_spline_plane(in Spline spline, in vec4 p, inout float t);

////////////////////////////////////
//   _____        _      _        // 
//  / ____|      | |    (_)       // 
// | |     _   _ | |__   _   ___  // 
// | |    | | | || '_ \ | | / __| // 
// | |____| |_| || |_) || || (__  // 
//  \_____|\__,_||_.__/ |_| \___| //
//                                //
////////////////////////////////////
Cubic cubic_constructor(in float a, in float b, in float c, in float d);
float calculate_default_root(inout Cubic cubic);
float single_real_root(inout Cubic cubic);
float first_root(inout Cubic cubic);
float second_root(inout Cubic cubic);
float third_root(inout Cubic cubic);

///////////////////////////
//  _____                //
// |  __ \               //
// | |__) | __ _  _   _  //
// |  _  / / _` || | | | //
// | | \ \| (_| || |_| | //
// |_|  \_\\__,_| \__, | //
//                 __/ | //
//                |___/  //
//                       //
///////////////////////////
Ray ray_constructor(in vec3 origin, in vec3 direction);
bool intersect_ray_aabb(in Ray ray, in AABB aabb, inout vec2 ts);
bool walk_spline_map(in Ray ray, in SplineMap spline_map, in ivec3 size, in float step_size, inout ivec3 texel, inout float t);

/////////////////////////////////////////////////////////////////////////
//    _____         _  _                _____  _             _         //
//   / ____|       | |(_)              / ____|| |           (_)        //
//  | (___   _ __  | | _  _ __    ___ | |     | |__    __ _  _  _ __   //
//   \___ \ | '_ \ | || || '_ \  / _ \| |     | '_ \  / _` || || '_ \  //
//   ____) || |_) || || || | | ||  __/| |____ | | | || (_| || || | | | //
//  |_____/ | .__/ |_||_||_| |_| \___| \_____||_| |_| \__,_||_||_| |_| //
//          | |                                                        //
//          |_|                                                        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
void spline_chain_get_extremes(in SplineChain spline_chain, float extremes[8 * MAX_SPLINES]);
vec3 position_on_spline_chain(in SplineChain spline_chain, in float t);
bool intersect_spline_chain_plane(in SplineChain spline_chain, in vec4 p, inout float t);

//////////////////////////////////////////////////////////////
//   _____         _  _               __  __                //
//  / ____|       | |(_)             |  \/  |               //
// | (___   _ __  | | _  _ __    ___ | \  / |  __ _  _ __   //
//  \___ \ | '_ \ | || || '_ \  / _ \| |\/| | / _` || '_ \  //
//  ____) || |_) || || || | | ||  __/| |  | || (_| || |_) | //
// |_____/ | .__/ |_||_||_| |_| \___||_|  |_| \__,_|| .__/  //
//         | |                                      | |     //
//         |_|                                      |_|     //
//                                                          //
//////////////////////////////////////////////////////////////
bool texture_coords(in SplineMap spline_map, in vec3 pos, inout vec3 coords);

// Function Implementations
// Spline
void spline_get_extremes(in Spline spline, float extremes[8]) {
    const vec3 a = spline.a * 3.0;
    const vec3 b = spline.b * 2.0;
    const vec3 c = spline.c;
    const vec3 D = b * b - 4.0 * a * c;

    extremes[0] = -1.0; extremes[1] = -1.0; // X
    extremes[2] = -1.0; extremes[3] = -1.0; // Y
    extremes[4] = -1.0; extremes[5] = -1.0; // Z
    extremes[6] =  0.0; extremes[7] =  1.0; // Start & End

    if (D.x >= 0.0) {
        const float Dx = sqrt(D.x);
        extremes[0] = (-b.x + Dx) / a.x * 0.5;
        extremes[1] = (-b.x - Dx) / a.x * 0.5;
    }

    if (D.y >= 0.0) {
        const float Dy = sqrt(D.y);
        extremes[2] = (-b.y + Dy) / a.y * 0.5;
        extremes[3] = (-b.y - Dy) / a.y * 0.5;
    }

    if (D.z >= 0.0) {
        const float Dz = sqrt(D.z);
        extremes[4] = (-b.z + Dz) / a.z * 0.5;
        extremes[5] = (-b.z - Dz) / a.z * 0.5;
    }
}

vec3 position_on_spline(in Spline spline, in float t) {
    return spline.a * t * t * t + spline.b * t * t + spline.c * t + spline.d;
}

bool intersect_spline_plane(in Spline spline, in vec4 p, inout float t) {
    const vec3 conversion = -spline.b / (3.0 * spline.a);
    Cubic cubic = cubic_constructor(
        spline.a.y,
        spline.b.y,
        spline.c.y,
        spline.d.y - p.y
    );

    t = conversion.y + first_root(cubic);
    if (t < -EPSILON || t > 1.0 + EPSILON) {
        t = conversion.y + second_root(cubic);
        if (t < -EPSILON || t > 1.0 + EPSILON) {
            t = conversion.y + third_root(cubic);
            if (t < -EPSILON || t > 1.0 + EPSILON) {
                return false;
            }
        }
    }

    return true;
}

// Cubic
Cubic cubic_constructor(in float a, in float b, in float c, in float d) {
    Cubic cubic;

    cubic.p = (3.0 * a * c - b * b) / (3.0 * a * a);
    cubic.q = (2.0 * b * b * b - 9.0 * a * b * c + 27.0 * a * a * d) / (27.0 * a * a * a);

    cubic.discriminant = 27.0 * cubic.q * cubic.q + 4.0 * cubic.p * cubic.p * cubic.p;

    cubic.root = calculate_default_root(cubic);

    return cubic;
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

// Ray
Ray ray_constructor(in vec3 origin, in vec3 direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = normalize(direction);
    ray.inv_dir = 1.0 / ray.direction;

    return ray;
}

bool intersect_ray_aabb(in Ray ray, in AABB aabb, inout vec2 ts) {
    const vec3 t1 = (aabb.min - ray.origin) * ray.inv_dir;
    const vec3 t2 = (aabb.max - ray.origin) * ray.inv_dir;

    const vec3 mins = min(t1, t2);
    const vec3 maxs = max(t1, t2);

    const float near = max(mins.x, max(mins.y, mins.z));
    const float far  = min(maxs.x, min(maxs.y, maxs.z));

    ts = vec2(near, far);
    return ts.x <= ts.y && ts.y >= 0.0;
}

bool walk_spline_map(in Ray ray, in SplineMap spline_map, in ivec3 size, inout ivec3 texel, inout float t) {
    vec2 ts;
    const bool result = intersect_ray_aabb(ray, spline_map.aabb, ts);

    if (result) {
        for (float i = max(0.0, ts.x); i <= ts.y; i += step_size) {
            const vec3 pos = ray.origin + ray.direction * i;
            vec3 coords;

            if (texture_coords(spline_map, pos, coords)) {
                texel = ivec3(coords * size);
                const float color = texelFetch(texVol, texel, 0).r;

                if (color > threshold) {
                    t = i;
                    return true;
                }
            }
        }
    }

    return false;
}

// SplineChain
void spline_chain_get_extremes(in SplineChain spline_chain, float extremes[8 * MAX_SPLINES]) {
    const float inv_amount = 1.0 / spline_chain.amount;
    float j = 0;

    for (int i = 0; i < int(spline_chain.amount); i++) {
        float temp[8];
        spline_get_extremes(spline_chain.splines[i], temp);

        extremes[i * 8]     = temp[0] * inv_amount + j;
        extremes[i * 8 + 1] = temp[1] * inv_amount + j;
        extremes[i * 8 + 2] = temp[2] * inv_amount + j;
        extremes[i * 8 + 3] = temp[3] * inv_amount + j;
        extremes[i * 8 + 4] = temp[4] * inv_amount + j;
        extremes[i * 8 + 5] = temp[5] * inv_amount + j;
        extremes[i * 8 + 6] = temp[6] * inv_amount + j;
        extremes[i * 8 + 7] = temp[7] * inv_amount + j;

        j += inv_amount;
    }
}

vec3 position_on_spline_chain(in SplineChain spline_chain, in float t) {
    const float clamped_t = max(0.0, min(t, 1.0 - 1e-4));
    const int   index     = int(floor(clamped_t * spline_chain.amount));
    const float t_prime   = (clamped_t - float(index) / spline_chain.amount) * spline_chain.amount;

    return position_on_spline(spline_chain.splines[index], t_prime);
}

bool intersect_spline_chain_plane(in SplineChain spline_chain, in vec4 p, inout float t) {
    bool r0 = spline_chain.amount > 0.0 && intersect_spline_plane(spline_chain.splines[0], p, t);
    bool r1 = spline_chain.amount > 1.0 && !r0 && intersect_spline_plane(spline_chain.splines[1], p, t);
    bool r2 = spline_chain.amount > 2.0 && !r1 && intersect_spline_plane(spline_chain.splines[2], p, t);
    bool r3 = spline_chain.amount > 3.0 && !r2 && intersect_spline_plane(spline_chain.splines[3], p, t);
    bool r4 = spline_chain.amount > 4.0 && !r3 && intersect_spline_plane(spline_chain.splines[4], p, t);

    return r0 || r1 || r2 || r3 || r4;
}

// SplineMap
bool texture_coords(in SplineMap spline_map, in vec3 pos, inout vec3 coords) {
    const vec4 p = spline_map.base.inv_matrix * vec4(pos, 1.0);
    float t;

    if (intersect_spline_chain_plane(spline_map.transformed_spline_chain, p, t)) {
        const vec3 edge1 = position_on_spline_chain(spline_map.spline_chain, t);
        const vec3 edge2 = edge1 + spline_map.base.size;
        const vec3 diff1 = pos - edge1;
        const vec3 diff2 = pos - edge2;

        if (dot(diff1, diff1) > spline_map.size_squared - EPSILON
        ||  dot(diff2, diff2) > spline_map.size_squared - EPSILON) {
            return false;
        }

        const float xComp = dot(diff1, spline_map.base.span1) / dot(spline_map.base.span1, spline_map.base.span1);
        const float zComp = dot(diff1, spline_map.base.span2) / dot(spline_map.base.span2, spline_map.base.span2);

        coords = vec3(xComp, t, zComp);
        return true;
    }

    return false;
}

void get_origin_direction(in vec2 fragCoord, out vec3 origin, out vec3 direction) {
	const vec4 hray = vec4(fragCoord * 2.0 - vec2(1.0), 1.0, 1.0);
	const vec4 wray = uCamera.inverseProjCamera * hray;

	origin = uCamera.cameraWorldPos;
	direction = normalize( wray.xyz / wray.w - origin );
}

void main() {
    oColor = vec3(0.0);

    vec3 origin;
    vec3 direction;
    get_origin_direction(gl_FragCoord.xy * uCamera.reciprocalWindowSize, origin, direction);

    Ray ray = ray_constructor(origin, direction);

    ivec3 texel;
    float t;
    if (walk_spline_map(ray, uSplineMap.spline_map, textureSize(texVol, 0), texel, t)) {
        // oColor = texelFetch(texVol, texel, 0).rrr;
        oColor = vec3(texel) / vec3(textureSize(texVol, 0));
    } else {
        discard;
    }
}
