#version 450

// Constants
#define MAX_SPLINES 4
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
layout( binding = 1 ) uniform sampler3D distanceField;

uniform float step_size;
uniform float threshold;
uniform uint max_distance;

/// Structs
struct Spline {
    vec3 a; // t^3
    vec3 b; // t^2
    vec3 c; // t^1
    vec3 d; // t^0
    vec3 y_bounds;
};

struct Cubic {
    float p;
    float q;
    float root;
    float discriminant;
    float fac;
    float arccos;
};

struct Plane {
    vec3 point;
    vec3 span1;
    vec3 span2;
    vec3 normal;

    mat4 inv_matrix;
};

struct Ray {
	vec3 origin;
	vec3 direction;
    vec3 inv_dir;
};

struct SplineMap {
    Plane base;
    Plane opposite_base;
    Spline spline;
    Spline opposite_spline;
    Spline transformed_spline;

    float width;
    float height;
    float depth;
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
vec3 position_on_spline(in Spline spline, in float t);
bool intersect_transformed_spline_plane(in Spline spline, in vec4 p, inout float t);
void intersect_spline_plane(in Spline spline, in Plane plane, inout vec3 ts);
Spline transform_spline(in Spline spline, in mat4 matrix);

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

//////////////////////////////////////
//   _____   _                      //
//  |  __ \ | |                     //
//  | |__) || |  __ _  _ __    ___  //
//  |  ___/ | | / _` || '_ \  / _ \ //
//  | |     | || (_| || | | ||  __/ //
//  |_|     |_| \__,_||_| |_| \___| //
//                                  //
//////////////////////////////////////
Plane plane_constructor(in vec3 point, in vec3 span1, in vec3 span2);

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
bool walk_spline_map(in Ray ray, in SplineMap spline_map, in ivec3 size, in float step_size, inout ivec3 texel, inout float t);
bool intersect_ray_line_segment(in Ray ray, in vec3 point1, in vec3 point2, inout float t);
void find_ray_spline_intersection(in Ray ray, in Spline spline, in float t, in vec3 offset, inout vec2 ret);
bool intersect_ray_spline_map(in Ray ray, in SplineMap spline_map, inout vec2 ts);
bool intersect_ray_plane(in Ray ray, in Plane plane, inout float t);

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
vec3 position_on_spline(in Spline spline, in float t) {
    return spline.a * t * t * t + spline.b * t * t + spline.c * t + spline.d;
}

bool intersect_transformed_spline_plane(in Spline spline, in vec4 p, inout float t) {
    const vec3 conversion = -spline.b / (3.0 * spline.a);
    Cubic cubic = cubic_constructor(
        spline.a.y,
        spline.b.y,
        spline.c.y,
        spline.d.y - p.y
    );

    t = conversion.y + first_root(cubic);
    if (t < 0.0 || t > 1.0) {
        t = conversion.y + second_root(cubic);
        if (t < 0.0 || t > 1.0) {
            t = conversion.y + third_root(cubic);
            if (t < 0.0 || t > 1.0) {
                return false;
            }
        }
    }

    return true;
}

void intersect_spline_plane(in Spline spline, in Plane plane, inout vec3 ts) {
    const Spline transformed_spline = transform_spline(spline, plane.inv_matrix);

    const float conversion = -transformed_spline.b.y / (3.0 * transformed_spline.a.y);
    Cubic cubic = cubic_constructor(
        transformed_spline.a.y,
        transformed_spline.b.y,
        transformed_spline.c.y,
        transformed_spline.d.y
    );

    ts.x = conversion + first_root(cubic);
    ts.y = conversion + second_root(cubic);
    ts.z = conversion + third_root(cubic);
}

Spline transform_spline(in Spline spline, in mat4 matrix) {
    Spline transformed;

    transformed.a = (matrix * vec4(spline.a, 0.0)).xyz;
    transformed.b = (matrix * vec4(spline.b, 0.0)).xyz;
    transformed.c = (matrix * vec4(spline.c, 0.0)).xyz;
    transformed.d = (matrix * vec4(spline.d, 1.0)).xyz;

    return transformed;
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

// Plane
Plane plane_constructor(in vec3 point, in vec3 span1, in vec3 span2) {
    Plane plane;
    plane.point = point;
    plane.span1 = span1;
    plane.span2 = span2;
    plane.normal = normalize(cross(span1, span2));

    const vec3 raxis  = normalize(vec3(plane.normal.z, 0.0, -plane.normal.x));
    const float rangle = acos(plane.normal.y);
    const float cangle = cos(rangle);
    const float sangle = sin(rangle);

    plane.inv_matrix = inverse(mat4(
        cangle + raxis.x * raxis.x * (1.0 - cangle),
        raxis.y * raxis.x * (1.0 - cangle) + raxis.z * sangle,
        raxis.z * raxis.x * (1.0 - cangle) - raxis.y * sangle,
        0.0,

        raxis.x * raxis.y * (1.0 - cangle) - raxis.z * sangle,
        cangle + raxis.y * raxis.y * (1.0 - cangle),
        raxis.z * raxis.y * (1.0 - cangle) + raxis.x * sangle,
        0.0,

        raxis.x * raxis.z * (1.0 - cangle) + raxis.y * sangle,
        raxis.y * raxis.z * (1.0 - cangle) - raxis.x * sangle,
        cangle + raxis.z * raxis.z * (1.0 - cangle),
        0.0,

        point.x,
        point.y,
        point.z,
        1.0
    ));

    return plane;
}

// Ray
Ray ray_constructor(in vec3 origin, in vec3 direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = normalize(direction);
    ray.inv_dir = 1.0 / ray.direction;

    return ray;
}

bool walk_spline_map(in Ray ray, in SplineMap spline_map, in ivec3 size, inout ivec3 texel, inout float t) {
    vec2 ts;
    const bool result = intersect_ray_spline_map(ray, spline_map, ts);

    if (result) {
        for (float i = max(0.0, ts.x); i <= ts.y; i += step_size) {
            const vec3 pos = ray.origin + ray.direction * i;
            vec3 coords;

            if (texture_coords(spline_map, pos, coords)) {
                texel = ivec3(coords * size);
                const float color = texelFetch(texVol, texel, 0).r;
                const float dist = texelFetch(distanceField, texel, 0).r;

                if (dist <= max_distance) {
                    t = i;
                    return true;
                }
            }
        }
    }

    return false;
}

bool intersect_ray_line_segment(in Ray ray, in vec3 point1, in vec3 point2, inout float t) {
    const vec3 dpoint = point1 - point2;

    const float dxy = ray.direction.y * dpoint.x - ray.direction.x * dpoint.y;
    const float dxz = ray.direction.z * dpoint.x - ray.direction.x * dpoint.z;
    const float dyz = ray.direction.z * dpoint.y - ray.direction.y * dpoint.z;

    if (dxy == 0.0 && dxz == 0.0 && dyz == 0.0) return false;

    float result;
    if (abs(dxy) > abs(dxz) && abs(dxy) > abs(dyz)) result = (ray.direction.y * (point1.x - ray.origin.x) - ray.direction.x * (point1.y - ray.origin.y)) / dxy;
    if (abs(dxz) > abs(dyz) && abs(dxz) > abs(dxy)) result = (ray.direction.z * (point1.x - ray.origin.x) - ray.direction.x * (point1.z - ray.origin.z)) / dxz;
    if (abs(dyz) > abs(dxy) && abs(dyz) > abs(dxz)) result = (ray.direction.z * (point1.y - ray.origin.y) - ray.direction.y * (point1.z - ray.origin.z)) / dyz;

    if (result < 0.0 || result > 1.0) return false;

    const vec3 intersection = point1 * (1.0 - result) + point2 * result;
    t = (intersection.x - ray.origin.x) / ray.direction.x;
    return true;
}

void find_ray_spline_intersection(
    in Ray ray, in Spline spline, in float t,
    in vec3 offset, inout vec2 ret)
{
    if (t >= 0.0 - EPSILON && t <= 1.0 + EPSILON) {
        const vec3 pos1 = position_on_spline(spline, t);
        const vec3 pos2 = pos1 + offset;

        float _t;
        if (intersect_ray_line_segment(ray, pos1, pos2, _t)) {
            ret.x = min(ret.x, _t);
            ret.y = max(ret.y, _t);
        }
    }
}

bool intersect_ray_spline_map(in Ray ray, in SplineMap spline_map, inout vec2 ts) {
    ts = vec2(10.0, -10.0);

    const vec3 span1 = spline_map.base.span1;
    const vec3 span2 = spline_map.base.span2;

    const Plane plane1 = plane_constructor(ray.origin, normalize(ray.direction * (1.0 - span1)), span1);
    const Plane plane2 = plane_constructor(ray.origin, normalize(ray.direction * (1.0 - span2)), span2);

    vec3 intermediate_ts;

    intersect_spline_plane(spline_map.spline, plane1, intermediate_ts);
    find_ray_spline_intersection(ray, spline_map.spline, intermediate_ts.x, span1, ts);
    find_ray_spline_intersection(ray, spline_map.spline, intermediate_ts.y, span1, ts);
    find_ray_spline_intersection(ray, spline_map.spline, intermediate_ts.z, span1, ts);

    intersect_spline_plane(spline_map.opposite_spline, plane1, intermediate_ts);
    find_ray_spline_intersection(ray, spline_map.opposite_spline, intermediate_ts.x, -span1, ts);
    find_ray_spline_intersection(ray, spline_map.opposite_spline, intermediate_ts.y, -span1, ts);
    find_ray_spline_intersection(ray, spline_map.opposite_spline, intermediate_ts.z, -span1, ts);

    intersect_spline_plane(spline_map.spline, plane2, intermediate_ts);
    find_ray_spline_intersection(ray, spline_map.spline, intermediate_ts.x, span2, ts);
    find_ray_spline_intersection(ray, spline_map.spline, intermediate_ts.y, span2, ts);
    find_ray_spline_intersection(ray, spline_map.spline, intermediate_ts.z, span2, ts);

    intersect_spline_plane(spline_map.opposite_spline, plane2, intermediate_ts);
    find_ray_spline_intersection(ray, spline_map.opposite_spline, intermediate_ts.x, -span2, ts);
    find_ray_spline_intersection(ray, spline_map.opposite_spline, intermediate_ts.y, -span2, ts);
    find_ray_spline_intersection(ray, spline_map.opposite_spline, intermediate_ts.z, -span2, ts);

    float t;
    if (intersect_ray_plane(ray, spline_map.base, t)) {
        ts.x = min(ts.x, t);
        ts.y = max(ts.y, t);
    }

    if (intersect_ray_plane(ray, spline_map.opposite_base, t)) {
        ts.x = min(ts.x, t);
        ts.y = max(ts.y, t);
    }

    return ts.x < ts.y;
}

bool intersect_ray_plane(in Ray ray, in Plane plane, inout float t) {
    const float del = dot(plane.normal, ray.direction);

    if (del == 0.0) return false; // Could also be an infinite amount of intersections
    
    t = dot(plane.normal, (plane.point - ray.origin)) / del;

    const vec3 intersection = ray.origin + t * ray.direction;
    const vec3 pmin = plane.point;
    const vec3 pmax = plane.point + plane.span1 + plane.span2;

    return (
        intersection.x >= pmin.x - EPSILON &&
        intersection.y >= pmin.y - EPSILON &&
        intersection.z >= pmin.z - EPSILON &&
        intersection.x <= pmax.x + EPSILON &&
        intersection.y <= pmax.y + EPSILON &&
        intersection.z <= pmax.z + EPSILON
    );
}

// SplineMap
bool texture_coords(in SplineMap spline_map, in vec3 pos, inout vec3 coords) {
    const vec4 p = spline_map.base.inv_matrix * vec4(pos, 1.0);
    float t;

    if (intersect_transformed_spline_plane(spline_map.transformed_spline, p, t)) {
        const vec3 edge = position_on_spline(spline_map.transformed_spline, t);
        const vec3 diff = edge - p.xyz;
        const float xComp = diff.x / spline_map.width;
        const float yComp = 1.0 - p.y / spline_map.height;
        const float zComp = diff.z / spline_map.depth;

        coords = abs(vec3(xComp, yComp, zComp));
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
        oColor = vec3(texel) / vec3(textureSize(texVol, 0));
    } else {
        discard;
    }
}
