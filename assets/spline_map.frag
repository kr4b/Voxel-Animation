#version 450

// Constants
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

uniform int steps;
uniform float time;

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

    mat4 matrix;
    mat4 inv_matrix;
};

struct Ray {
	vec3 origin;
	vec3 direction;
    vec3 inv_dir;
};

struct SplineMap {
    AABB aabb;
    Plane base;
    Spline spline;

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
Spline spline_constructor(in vec3 a, in vec3 b, in vec3 c, in vec3 d);
Spline spline_with_tangents(in vec3 point1, in vec3 point2, in vec3 tangent1, in vec3 tangent2);
Spline spline_with_control_points(in vec3 point1, in vec3 point2, in vec3 control1, in vec3 control2);
Spline spline_transform(in Spline spline, in mat4 matrix);
void spline_get_extremes(in Spline spline, float extremes[8]);
vec3 position_on_spline(in Spline spline, in float t);
bool intersect_spline_aabb(in Spline spline, in AABB aabb, inout vec2 ts);
bool intersect_spline_plane(in Spline spline, in Plane plane, inout float t);
bool calculate_near_far(in Spline spline, in AABB aabb, in vec3 t1, in vec3 t2, inout vec2 ts);
vec3 intersected_aabb(in Spline spline, in AABB aabb, in vec3 t);

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

////////////////////////////////////////
//                      ____   ____   //
//     /\        /\    |  _ \ |  _ \  //
//    /  \      /  \   | |_) || |_) | //
//   / /\ \    / /\ \  |  _ < |  _ <  //
//  / ____ \  / ____ \ | |_) || |_) | //
// /_/    \_\/_/    \_\|____/ |____/  //
//                                    //
////////////////////////////////////////
AABB aabb_constructor(in vec3 aabb_min, in vec3 aabb_max);

/////////////////////////////////////
//  _____   _                      //
// |  __ \ | |                     //
// | |__) || |  __ _  _ __    ___  //
// |  ___/ | | / _` || '_ \  / _ \ //
// | |     | || (_| || | | ||  __/ //
// |_|     |_| \__,_||_| |_| \___| //
//                                 //
/////////////////////////////////////
Plane plane_constructor(in vec3 center, in vec3 half_size);
Plane plane_transform(in Plane plane, in mat4 matrix);

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
SplineMap spline_map_constructor(in Plane base, in Spline spline);
AABB create_encompassing_aabb(in Plane base, in Spline spline);
bool texture_coords(in SplineMap spline_map, in vec3 pos, inout vec3 coords);

// Function Implementations
// Spline
Spline spline_constructor(in vec3 a, in vec3 b, in vec3 c, in vec3 d) {
    Spline spline;
    spline.a = a;
    spline.b = b;
    spline.c = c;
    spline.d = d;
    return spline;
}

Spline spline_with_tangents(in vec3 point1, in vec3 point2, in vec3 tangent1, in vec3 tangent2) {
    const vec3 a = point1 *  2.0 + point2 * -2.0 + tangent1 *  1.0 + tangent2 *  1.0;
    const vec3 b = point1 * -3.0 + point2 *  3.0 + tangent1 * -2.0 + tangent2 * -1.0;
    const vec3 c = tangent1;
    const vec3 d = point1;

    return spline_constructor(a, b, c, d);
}

Spline spline_with_control_points(in vec3 point1, in vec3 point2, in vec3 control1, in vec3 control2) {
    const float tau = 0.2;

    const vec3 a = control1 * -1.0 * tau + point1 * (2.0 - tau) + point2 * (tau - 2.0)       + control2 *  tau;
    const vec3 b = control1 *  2.0 * tau + point1 * (tau - 3.0) + point2 * (3.0 - 2.0 * tau) + control2 * -tau;
    const vec3 c = control1 * -tau + point2 * tau;
    const vec3 d = point1;

    return spline_constructor(a, b, c, d);
}

Spline spline_transform(in Spline spline, in mat4 matrix) {
    return spline_constructor(
        (matrix * vec4(spline.a, 0.0)).xyz,
        (matrix * vec4(spline.b, 0.0)).xyz,
        (matrix * vec4(spline.c, 0.0)).xyz,
        (matrix * vec4(spline.d, 1.0)).xyz
    );
}

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

bool intersect_spline_aabb(in Spline spline, in AABB aabb, inout vec2 ts) {
    const vec3 conversion = -spline.b / (3.0 * spline.a);

    Cubic cubic_min_x = cubic_constructor(spline.a.x, spline.b.x, spline.c.x, spline.d.x - aabb.min.x);
    Cubic cubic_min_y = cubic_constructor(spline.a.y, spline.b.y, spline.c.y, spline.d.y - aabb.min.y);
    Cubic cubic_min_z = cubic_constructor(spline.a.z, spline.b.z, spline.c.z, spline.d.z - aabb.min.z);
    Cubic cubic_max_x = cubic_constructor(spline.a.x, spline.b.x, spline.c.x, spline.d.x - aabb.max.x);
    Cubic cubic_max_y = cubic_constructor(spline.a.y, spline.b.y, spline.c.y, spline.d.y - aabb.max.y);
    Cubic cubic_max_z = cubic_constructor(spline.a.z, spline.b.z, spline.c.z, spline.d.z - aabb.max.z);

    const vec3 t1 = conversion + vec3(
        second_root(cubic_min_x),
        second_root(cubic_min_y),
        second_root(cubic_min_z));
    const vec3 t2 = conversion + vec3(
        second_root(cubic_max_x),
        second_root(cubic_max_y),
        second_root(cubic_max_z));

    ts = vec2(2.0, -2.0);
    bool result = calculate_near_far(spline, aabb, t1, t2, ts);

    if (!result) {
        const vec3 first_t1 = conversion + vec3(
            first_root(cubic_min_x),
            first_root(cubic_min_y),
            first_root(cubic_min_z));
        const vec3 first_t2 = conversion + vec3(
            first_root(cubic_max_x),
            first_root(cubic_max_y),
            first_root(cubic_max_z));

        result = calculate_near_far(spline, aabb, first_t1, first_t2, ts);

        if (!result) {
            const vec3 third_t1 = conversion + vec3(
                third_root(cubic_min_x),
                third_root(cubic_min_y),
                third_root(cubic_min_z));
            const vec3 third_t2 = conversion + vec3(
                third_root(cubic_max_x),
                third_root(cubic_max_y),
                third_root(cubic_max_z));

            result = calculate_near_far(spline, aabb, third_t1, third_t2, ts);
        }
    }

    return result;
}

bool intersect_spline_plane(in Spline spline, in Plane plane, inout float t) {
    const vec3 transformed_min = (plane.inv_matrix * vec4(plane.min, 1.0)).xyz;
    const vec3 transformed_max = (plane.inv_matrix * vec4(plane.max, 1.0)).xyz;
    const Spline transformed_spline = spline_transform(spline, plane.inv_matrix);

    AABB aabb = aabb_constructor(transformed_min, transformed_max);
    vec2 ts;

    const bool result = intersect_spline_aabb(transformed_spline, aabb, ts);
    t = ts.x;

    return result;
}

bool calculate_near_far(in Spline spline, in AABB aabb, in vec3 t1, in vec3 t2, inout vec2 ts) {
    const vec3 it1 = intersected_aabb(spline, aabb, t1);
    const vec3 it2 = intersected_aabb(spline, aabb, t2);

    const vec3 nt1 = t1 * it1 + (1.0 - it1) * MAX_VALUE;
    const vec3 nt2 = t2 * it2 + (1.0 - it2) * MAX_VALUE;

    const vec3 ft1 = t1 * it1 + (1.0 - it1) * MIN_VALUE;
    const vec3 ft2 = t2 * it2 + (1.0 - it2) * MIN_VALUE;

    const vec3 inear = min(nt1, nt2);
    const vec3 ifar  = max(ft1, ft2);

    ts.x = min(ts.x, min(inear.x, min(inear.y, inear.z)));
    ts.y = max(ts.y, max(ifar.x,  max(ifar.y, ifar.z)));

    return ts.x <= ts.y && ts.y >= 0.0;
}

vec3 intersected_aabb(in Spline spline, in AABB aabb, in vec3 t) {
    const vec3 P0 = position_on_spline(spline, t.x);
    const vec3 P1 = position_on_spline(spline, t.y);
    const vec3 P2 = position_on_spline(spline, t.z);

    const vec3 resultT = step(vec3(0.0), t) * step(t, vec3(1.0));
    const vec3 result0 = step(aabb.min - EPSILON, P0) * step(P0, aabb.max + EPSILON);
    const vec3 result1 = step(aabb.min - EPSILON, P1) * step(P1, aabb.max + EPSILON);
    const vec3 result2 = step(aabb.min - EPSILON, P2) * step(P2, aabb.max + EPSILON);

    return vec3(
        resultT.x * result0.x * result0.y * result0.z,
        resultT.y * result1.x * result1.y * result1.z,
        resultT.z * result2.x * result2.y * result2.z);
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

// AABB
AABB aabb_constructor(in vec3 aabb_min, in vec3 aabb_max) {
    AABB aabb;
    aabb.min = min(aabb_min, aabb_max);
    aabb.max = max(aabb_min, aabb_max);

    return aabb;
}

// Plane
Plane plane_constructor(in vec3 center, in vec3 half_size) {
    Plane plane;
    plane.center = center;
    plane.half_size = half_size;
    plane.size = half_size * 2.0;

    const vec3 p1 = center - half_size;
    const vec3 p2 = center + vec3(half_size.x, -half_size.y, abs(half_size.y) * half_size.z + abs(half_size.x) * -half_size.z);
    const vec3 p3 = center + half_size;
    const vec3 v = p2 - p1;
    const vec3 w = p3 - p1;
    plane.normal = normalize(vec3(v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z, v.x * w.y - v.y * w.x));
    plane.span1 = v;
    plane.span2 = w;

    plane.min = center - half_size;
    plane.max = center + half_size;

    const vec3 rotation_axis   = cross(vec3(0.0, 1.0, 0.0), plane.normal);
    const float rotation_angle = acos(dot(vec3(0.0, 1.0, 0.0), plane.normal));

    float cosAngle = cos(rotation_angle);
    float sinAngle = sin(rotation_angle);

    // https://en.wikipedia.org/wiki/Rotation_matrix#Conversion_from_rotation_matrix_and_to_axis%E2%80%93angle
    plane.matrix = mat4(
        cosAngle + rotation_axis.x * rotation_axis.x * (1.0 - cosAngle),
        rotation_axis.x * rotation_axis.y * (1.0 - cosAngle) - rotation_axis.z * sinAngle,
        rotation_axis.x * rotation_axis.z * (1.0 - cosAngle) + rotation_axis.y * sinAngle,
        plane.center.x,

        rotation_axis.y * rotation_axis.x * (1.0 - cosAngle) + rotation_axis.z * sinAngle,
        cosAngle + rotation_axis.y * rotation_axis.y * (1.0 - cosAngle),
        rotation_axis.y * rotation_axis.z * (1.0 - cosAngle) - rotation_axis.x * sinAngle,
        plane.center.y,

        rotation_axis.z * rotation_axis.x * (1.0 - cosAngle) - rotation_axis.y * sinAngle,
        rotation_axis.z * rotation_axis.y * (1.0 - cosAngle) + rotation_axis.x * sinAngle,
        cosAngle + rotation_axis.z * rotation_axis.z * (1.0 - cosAngle),
        plane.center.z,

        0.0, 0.0, 0.0, 1.0
    );

    plane.inv_matrix = inverse(plane.matrix);

    return plane;
}

Plane plane_transform(in Plane plane, in mat4 matrix) {
    return plane_constructor((matrix * vec4(plane.center, 1.0)).xyz, plane.half_size);
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

bool walk_spline_map(in Ray ray, in SplineMap spline_map, in ivec3 size, in float step_size, inout ivec3 texel, inout float t) {
    vec2 ts;
    const bool result = intersect_ray_aabb(ray, spline_map.aabb, ts);

    if (result) {
        for (float i = max(0.0, ts.x); i <= ts.y; i += step_size) {
            const vec3 pos = ray.origin + ray.direction * i;
            vec3 coords;

            if (texture_coords(spline_map, pos, coords)) {
                texel = ivec3(coords * size);

                if (texel.x >= 0 && texel.x < size.x
                &&  texel.y >= 0 && texel.y < size.y
                &&  texel.z >= 0 && texel.z < size.z) {
                    const float color = texelFetch(texVol, texel, 0).r;

                    if (color > 0.25) {
                        t = i;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

// SplineMap
SplineMap spline_map_constructor(in Plane base, in Spline spline) {
    SplineMap spline_map;
    spline_map.base = base;
    spline_map.spline = spline_transform(
        spline_transform(spline, base.matrix),
        mat4(
            1.0, 0.0, 0.0, -base.half_size.x,
            0.0, 1.0, 0.0, -base.half_size.y,
            0.0, 0.0, 1.0, -base.half_size.z,
            0.0, 0.0, 0.0, 1.0
        ));
    spline_map.aabb = create_encompassing_aabb(base, spline);
    spline_map.size_squared = dot(base.size, base.size);

    return spline_map;
}

AABB create_encompassing_aabb(in Plane base, in Spline spline) {
    float extremes[8];
    spline_get_extremes(spline, extremes);

    vec3 position = position_on_spline(spline, extremes[0]);
    vec3 aabb_min = position;
    vec3 aabb_max = position;

    for (int i = 1; i < 8; i++) {
        position = position_on_spline(spline, extremes[i]);
        aabb_min = min(aabb_min, position);
        aabb_max = max(aabb_max, position);
    }

    return aabb_constructor(
        aabb_min,
        aabb_max + base.size
    );
}

bool texture_coords(in SplineMap spline_map, in vec3 pos, inout vec3 coords) {
    const Plane plane = plane_constructor(pos, spline_map.base.size);
    float t;

    if (intersect_spline_plane(spline_map.spline, plane, t)) {
        const vec3 edge1 = position_on_spline(spline_map.spline, t);
        const vec3 edge2 = edge1 + spline_map.base.size;
        const vec3 diff1 = pos - edge1;
        const vec3 diff2 = pos - edge2;

        if (dot(diff1, diff1) > spline_map.size_squared
        ||  dot(diff2, diff2) > spline_map.size_squared) {
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
    if (walk_spline_map(ray, uSplineMap.spline_map, textureSize(texVol, 0), 0.025, texel, t)) {
        // oColor = texelFetch(texVol, texel, 0).rrr;
        oColor = vec3(texel) / vec3(textureSize(texVol, 0));
    } else {
        discard;
    }
}
