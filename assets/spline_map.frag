#version 450

// Constants
#define MAX_SPLINES 4
#define VOLUME_STEPS 1024
#define MAX_SAMPLERS 1
#define M_PI 3.14159265
#define EPSILON 1.0e-2
#define MAX_VALUE 2.0
#define MIN_VALUE -MAX_VALUE
#define USE_TRANSFER_FUNCTION 0

layout( location = 0 ) in vec2 v2fTexCoord;

layout( location = 0 ) out vec4 oColor;

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
layout( binding = 2 ) uniform sampler3D gradientField;

uniform float step_size;
uniform float threshold;
// Transfer function variables, not always set
uniform float stop0x;
uniform float stop1x;

/// Structs
struct Spline {
    vec3 a; // t^3
    vec3 b; // t^2
    vec3 c; // t^1
    vec3 d; // t^0
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

    vec3 color;
    vec3 size;

    float scale;
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

// Get position on the spline for t
vec3 position_on_spline(in Spline spline, in float t);

// Get position on the spline derivative for t
vec3 position_on_spline(in Spline spline, in float t);

// Intersect axis aligned spline with offset xz-axis plane
bool intersect_transformed_spline_plane(in Spline spline, in float offset, inout float t);

// Intersect spline with arbitrary plane
void intersect_spline_plane(in Spline spline, in Plane plane, inout vec3 ts);

// Transform spline with transformation matrix
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

// Solve up to a cubic equation
bool solve_polynomial(in float a, in float b, in float c, in float d, out vec3 roots);

//////////////////////////////////////
//   _____   _                      //
//  |  __ \ | |                     //
//  | |__) || |  __ _  _ __    ___  //
//  |  ___/ | | / _` || '_ \  / _ \ //
//  | |     | || (_| || | | ||  __/ //
//  |_|     |_| \__,_||_| |_| \___| //
//                                  //
//////////////////////////////////////

// Construct plane
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

// Construct ray
Ray ray_constructor(in vec3 origin, in vec3 direction);

// Construct a ray from the fragment coordinates
Ray get_frag_ray(in vec2 frag_coord);

// Intersect of ray with line
// Note that it is assumed that the ray intersects the line
bool intersect_ray_line_segment(in Ray ray, in vec3 point1, in vec3 point2, inout float t);

// Intersect ray with surface between two equal splines displaced from each other by an offset
void find_ray_spline_intersection(in Ray ray, in Spline spline, in float t, in vec3 offset, inout vec2 ret);

// Intersect ray with finite plane
bool intersect_ray_plane(in Ray ray, in Plane plane, inout float t);

// Intersect ray with spline map
bool intersect_ray_spline_map(in Ray ray, in SplineMap spline_map, inout vec2 ts);

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

// Convert world coordinate to texture coordinate
bool texture_coords(in SplineMap spline_map, in vec3 pos, out vec3 coords, out vec3 raw_coords);

// Walk/march ray over spline map
#if USE_TRANSFER_FUNCTION
bool walk_spline_map(in SplineMap spline_map, in Ray ray, in ivec3 size, in float step_size, inout ivec3 texel, inout float t, inout vec3 normal, inout vec4 color);
#else
bool walk_spline_map(in SplineMap spline_map, in Ray ray, in ivec3 size, in float step_size, inout ivec3 texel, inout float t, inout vec3 normal);
#endif

//////////////////////////////////////////////////////////////////////////////
//  __  __  _                  _  _                                         //
// |  \/  |(_)                | || |                                        //
// | \  / | _  ___   ___  ___ | || |  __ _  _ __    ___   ___   _   _  ___  //
// | |\/| || |/ __| / __|/ _ \| || | / _` || '_ \  / _ \ / _ \ | | | |/ __| //
// | |  | || |\__ \| (__|  __/| || || (_| || | | ||  __/| (_) || |_| |\__ \ //
// |_|  |_||_||___/ \___|\___||_||_| \__,_||_| |_| \___| \___/  \__,_||___/ //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

// Generate gradient normal for given texel
vec3 gradient_normal(in Spline spline, in vec3 texel);

vec4 transfer_function(in float value);

// Function Implementations
// Spline
vec3 position_on_spline(in Spline spline, in float t) {
    return spline.a * t * t * t + spline.b * t * t + spline.c * t + spline.d;
}

vec3 position_on_spline_prime(in Spline spline, in float t) {
    return 3.0 * spline.a * t * t + 2.0 * spline.b * t + spline.c;
}

bool intersect_transformed_spline_plane(in Spline spline, in float offset, inout float t) {
    vec3 roots;
    if (!solve_polynomial(spline.a.y, spline.b.y, spline.c.y, spline.d.y - offset, roots)) {
        return false;
    }

    t = roots.x;
    if (t < 0.0 || t > 1.0) {
        t = roots.y;
        if (t < 0.0 || t > 1.0) {
            t = roots.z;
        }
    }

    return t >= 0.0 && t <= 1.0;
}

void intersect_spline_plane(in Spline spline, in Plane plane, inout vec3 ts) {
    const Spline transformed_spline = transform_spline(spline, plane.inv_matrix);

    solve_polynomial(
        transformed_spline.a.y,
        transformed_spline.b.y,
        transformed_spline.c.y,
        transformed_spline.d.y,
        ts
    );
}

Spline transform_spline(in Spline spline, in mat4 matrix) {
    Spline transformed;

    // Transform all spline constants to transform the spline
    transformed.a = (matrix * vec4(spline.a, 0.0)).xyz;
    transformed.b = (matrix * vec4(spline.b, 0.0)).xyz;
    transformed.c = (matrix * vec4(spline.c, 0.0)).xyz;
    transformed.d = (matrix * vec4(spline.d, 1.0)).xyz;

    return transformed;
}

// Cubic
bool solve_polynomial(in float a, in float b, in float c, in float d, out vec3 roots) {
    if (a != 0.0) {
        // Cubic
        // https://en.wikipedia.org/wiki/Cubic_equation
        const float conversion = -b / (3.0 * a);
        const float p = (3.0 * a * c - b * b) / (3.0 * a * a);
        const float q = (2.0 * b * b * b - 9.0 * a * b * c + 27.0 * a * a * d) / (27.0 * a * a * a);

        const float discriminant = 27.0 * q * q + 4.0 * p * p * p;
        
        if (discriminant > 0.0) {
            const float D = sqrt(q * q / 4.0 + p * p * p / 27.0);
            const float C0 = -0.5 * q + D;
            const float C1 = -0.5 * q - D;

            roots = vec3(sign(C0) * pow(abs(C0), 0.333333) + sign(C1) * pow(abs(C1), 0.333333));
        }
        else if (discriminant < 0.0) {
            const float fac = 2.0 * sqrt(-p / 3.0);
            const float arccos = acos(3.0 * q / (2.0 * p) * sqrt(-3.0 / p)) / 3.0;

            roots = vec3(
                fac * cos(arccos),
                fac * cos(arccos - 2.0 / 3.0 * M_PI),
                fac * cos(arccos - 4.0 / 3.0 * M_PI)
            );
        }
        else if (p == 0.0) {
            roots = vec3(0.0);
        }
        else {
            roots = vec3(
                3.0 * q / p,
                -0.5 * 3.0 * q / p,
                -1.0
            );
        }

        roots += conversion;

        return true;
    }

    if (b != 0.0) {
        // Quadratic
        const float discriminant = c * c - 4 * b * d;

        if (discriminant < 0.0) {
            roots = vec3(-1.0);
            return false;
        }

        roots = vec3(
            0.5 * (-c + sqrt(discriminant)) / b,
            0.5 * (-c - sqrt(discriminant)) / b,
            -1.0
        );

        return true;
    }

    if (c != 0.0) {
        roots = vec3(
            -d / c,
            -1.0,
            -1.0
        );

        return true;
    }

    return false;
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

    mat4 matrix = mat4(
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
    );

    float abs_det = abs(determinant(matrix));
    if (isnan(abs_det) || abs_det <= 1e-9) {
        matrix = mat4(
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        );
    }

    plane.inv_matrix = inverse(matrix);

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

Ray get_frag_ray(in vec2 frag_coord) {
    const vec4 hray = vec4(frag_coord * 2.0 - vec2(1.0), 1.0, 1.0);
    const vec4 wray = uCamera.inverseProjCamera * hray;

    const vec3 origin = uCamera.cameraWorldPos;
    const vec3 direction = normalize( wray.xyz / wray.w - origin );

    return ray_constructor(origin, direction);
}

#if USE_TRANSFER_FUNCTION
bool walk_spline_map(in SplineMap spline_map, in Ray ray, in ivec3 size, inout ivec3 texel, inout float t, inout vec3 normal, inout vec4 color) {
    bool found_intersection = false;
    color.a = 1.0;
#else
bool walk_spline_map(in SplineMap spline_map, in Ray ray, in ivec3 size, inout ivec3 texel, inout float t, inout vec3 normal) {
#endif
    vec2 ts;
    const bool result = intersect_ray_spline_map(ray, spline_map, ts);

    if (result) {
        float i = max(0.0, ts.x);
        for (int steps = 0; steps < 1000 && i <= ts.y; steps++) {
            const vec3 pos = ray.origin + ray.direction * i;
            vec3 coords, raw_coords;

            if (texture_coords(spline_map, pos, coords, raw_coords)) {
                // Clamp texture coords here, texelfetch does not support wrap mode
                texel = ivec3(coords * size);
                const float value = texelFetch(texVol, texel, 0).r;

                // Intersection
                if (value > threshold) {
#if USE_TRANSFER_FUNCTION
                    vec4 voxel_color = transfer_function(value);
                    if (voxel_color.a > 0.0) {
                        color.xyz += voxel_color.xyz * voxel_color.a * color.a;
                        color.a *= 1.0 - voxel_color.a;
                    }

                    if (!found_intersection) {
                        found_intersection = true;
#endif
                        t = i;
                        normal = gradient_normal(spline_map.transformed_spline, coords);
#if USE_TRANSFER_FUNCTION
                    }
#else
                    return true;
#endif
                }

                // Determine the distance in the distance field and apply deformation correction
                // Deformation correction is determined by the deformation difference between pos and pos + dist
                const float dist = texelFetch(distanceField, texel, 0).r;
                vec3 new_coords, new_raw_coords;
                if (texture_coords(spline_map, pos + dist * spline_map.scale * ray.direction, new_coords, new_raw_coords)) {
                    const vec3 dist_vec = new_coords - coords;

                    // Difference in deformation (xz-plane)
                    const vec3 deform = (new_raw_coords - raw_coords) * sign(vec3(dist_vec.x, 0, dist_vec.z));
                    const vec3 new_dist_vec = abs(dist_vec) + deform;

                    // Dubious mathematics to ensure negative distance is substracted in the length calculation
                    const vec3 new_dist_vec_squared = new_dist_vec * new_dist_vec * sign(new_dist_vec);
                    const float new_dist_squared = dot(new_dist_vec_squared, vec3(1.0));
                    if (new_dist_squared > 0) {
                        i += max(sqrt(new_dist_squared), step_size);
                        continue;
                    }
                }
            }

            i += step_size;
        }
    }

#if USE_TRANSFER_FUNCTION
    return found_intersection;
#else
    return false;
#endif
}

bool intersect_ray_line_segment(in Ray ray, in vec3 point1, in vec3 point2, inout float t) {
    // https://mathworld.wolfram.com/Line-LineIntersection.html
    const vec3 line_origin = point1;
    const vec3 line_direction = point2 - line_origin;

    if (dot(ray.origin - line_origin, cross(line_direction, ray.direction)) == 0) {
        return false;
    }

    const vec3 plane = cross(line_direction, ray.direction);
    float s = dot(cross(ray.origin - line_origin, ray.direction), plane) / dot(plane, plane);
    float r = dot(cross(ray.origin - line_origin, line_direction), plane) / dot(plane, plane);

    if (s < 0.0 || s > 1.0 || r < 0.0) {
        return false;
    }

    t = r;

    return true;
}

void find_ray_spline_intersection(in Ray ray, in Spline spline, in float t, in vec3 offset, inout vec2 ret) {
    if (t >= 0.0 && t <= 1.0) {
        const vec3 pos1 = position_on_spline(spline, t);
        const vec3 pos2 = pos1 + offset;

        float _t;
        if (intersect_ray_line_segment(ray, pos1, pos2, _t)) {
            ret.x = min(ret.x, _t);
            ret.y = max(ret.y, _t);
        }
    }
}

bool intersect_ray_plane(in Ray ray, in Plane plane, inout float t) {
    const float del = dot(plane.normal, ray.direction);

    if (del == 0.0) return false; // Could also be an infinite amount of intersections
    
    // Find intersection of ray with plane
    t = dot(plane.normal, (plane.point - ray.origin)) / del;

    const vec3 intersection = ray.origin + t * ray.direction;
    const vec3 projection = intersection - plane.point;
    const float scalar1 = dot(projection, plane.span1);
    const float scalar2 = dot(projection, plane.span2);
    // TODO: Pre-compute?
    const float length1 = dot(plane.span1, plane.span1);
    const float length2 = dot(plane.span2, plane.span2);

    // Check if point is inside bounds of the plane/rectangle
    return (
        scalar1 >= 0 && scalar1 <= length1 &&
        scalar2 >= 0 && scalar2 <= length2
    );
}

bool intersect_ray_spline_map(in Ray ray, in SplineMap spline_map, inout vec2 ts) {
    ts = vec2(10.0, -10.0);

    const vec3 span1 = spline_map.base.span1;
    const vec3 span2 = spline_map.base.span2;

    // Project ray on planes orthogonal to spanning vectors of the base
    const Plane plane1 = plane_constructor(ray.origin, ray.direction, span1);
    const Plane plane2 = plane_constructor(ray.origin, ray.direction, span2);

    vec3 intermediate_ts;

    // Try ray intersections for all sides of the spline map
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

    return ts.x <= ts.y && ts.x >= 0.0;
}

// SplineMap
bool texture_coords(in SplineMap spline_map, in vec3 pos, out vec3 coords, out vec3 raw_coords) {
    const vec4 p = spline_map.base.inv_matrix * vec4(pos, 1.0);
    float t;

    // Interpolate coordinates on base translated to spline intersection point
    if (intersect_transformed_spline_plane(spline_map.transformed_spline, p.y, t)) {
        const vec3 edge = position_on_spline(spline_map.transformed_spline, t);
        const vec3 diff = p.xyz - edge;

        const vec3 tspan1 = (spline_map.base.inv_matrix * vec4(spline_map.base.span1, 1.0)).xyz;
        const vec3 tspan2 = (spline_map.base.inv_matrix * vec4(spline_map.base.span2, 1.0)).xyz;

        const float xComp = diff.x / length(tspan1);
        const float zComp = diff.z / length(tspan2);
        const float yComp = 1.0 - p.y / spline_map.size.y;

        raw_coords = edge;
        coords = vec3(xComp, yComp, zComp);
        return true;
    }

    return false;
}

// Misc
bool has_voxel(in ivec3 texel, in ivec3 offset) {
    const float color = texelFetch(texVol, texel + offset, 0).r;
    return color > threshold;
}

vec3 gradient_normal(in Spline spline, in vec3 coord) {
    const vec3 slope = position_on_spline_prime(spline, coord.y);
    const float xangle = atan(slope.x);
    const float zangle = atan(slope.z);
    const vec3 normal = vec3(
        texture(distanceField, coord - vec3(EPSILON, 0.0, 0.0)).r - texture(distanceField, coord + vec3(EPSILON, 0.0, 0.0)).r,
        texture(distanceField, coord - vec3(0.0, EPSILON, 0.0)).r - texture(distanceField, coord + vec3(0.0, EPSILON, 0.0)).r,
        texture(distanceField, coord - vec3(0.0, 0.0, EPSILON)).r - texture(distanceField, coord + vec3(0.0, 0.0, EPSILON)).r
    );
    return normalize(vec3(
        normal.x * cos(xangle),
        normal.y + normal.x * sin(xangle) + normal.z * sin(zangle),
        normal.z * cos(zangle)
    ));
}

vec4 transfer_function(in float value) {
    if (value >= 0.314) {
        return vec4(1.0, 1.0, 1.0, 0.8);
    }
    if (value >= 0.2635) {
        vec3 red = vec3(0.95, 0.1, 0.35); // [0.29, ->]
        vec3 blue = vec3(0.2, 0.1, 0.7); // [0.27, 0.29]

        float alpha = clamp((value - 0.2635) / (0.314 - 0.2635), 0.0, 1.0);
        return vec4(mix(blue, red, alpha), alpha * 0.4 + 0.1);
    }
    if (value >= stop0x) {
        vec3 blue = vec3(0.2, 0.1, 0.7); // [0.26, ->]
        vec3 yellow = vec3(0.6, 0.65, 0.05); // [0.2, 0.26]

        float alpha = clamp((value - stop0x) / (0.2635 - stop0x), 0.0, 1.0);
        return vec4(mix(yellow, blue, alpha), alpha * 0.085 + 0.1);
    }
    return vec4(1.0, 1.0, 1.0, 0.001);
}

void main() {
    oColor = vec4(0.0);

    Ray ray = get_frag_ray(gl_FragCoord.xy * uCamera.reciprocalWindowSize);

    bool intersect = false;
    ivec3 texel;
    float t;
    vec3 normal;
#if USE_TRANSFER_FUNCTION
    vec4 color;

    if (walk_spline_map(uSplineMap.spline_map, ray, textureSize(texVol, 0), texel, t, normal, color)) {
        const float light = dot(normalize(vec3(5.0, 150.0, 20.0) - (ray.origin + t * ray.direction)), normal);
        oColor = vec4(color.rgb * (max(0.0, light) * 0.7 + 0.3), 1.0);
#else
    if (walk_spline_map(uSplineMap.spline_map, ray, textureSize(texVol, 0), texel, t, normal)) {
        const float light = dot(normalize(vec3(5.0, 150.0, 20.0) - (ray.origin + t * ray.direction)), normal);
        oColor = vec4(uSplineMap.spline_map.color * (max(0.0, light) * 0.7 + 0.3), 1.0);
#endif
        // TODO: Depth hack, maybe base it on projection near/far planes
        gl_FragDepth = t / 100.0;
        // oColor = normal * 0.5 + 0.5;
        // oColor *= vec3(texel) / vec3(textureSize(texVol, 0));
        intersect = true;
    }

    if (true) {
        const vec3 floorNormal = vec3(0.0, -1.0, 0.0);
        const vec3 floorPoint = vec3(0.0, -1.0, 0.0);
        const float d = dot(floorNormal, ray.direction);
        if (d >= 10e-6) {
            const float t2 = dot(floorNormal, (floorPoint - ray.origin)) / d;
            if (t2 >= 10e-6 && (!intersect || t2 < t)) {
                oColor = vec4(0.76, 0.6, 0.42, 1.0);
                gl_FragDepth = 0.99;
                intersect = true;
            }
        } 
    }

    if (!intersect) {
        gl_FragDepth = 1.0;
    }
}
