#version 450

// Constants
#define MAX_SPLINES 4
#define VOLUME_STEPS 1024
#define MAX_SAMPLERS 1
#define M_PI 3.14159265
#define EPSILON 1.0e-2
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
layout( binding = 2 ) uniform sampler3D gradientField;

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

// Construct cubic equation
Cubic cubic_constructor(in float a, in float b, in float c, in float d);

// Calculate roots
// https://en.wikipedia.org/wiki/Cubic_equation#Cardano's_formula
// https://en.wikipedia.org/wiki/Cubic_equation#Trigonometric_solution_for_three_real_roots
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
Ray get_frag_ray(in vec2 frag_coord, out vec3 origin, out vec3 direction);

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
bool walk_spline_map(in SplineMap spline_map, in Ray ray, in ivec3 size, in float step_size, inout ivec3 texel, inout float t, inout vec3 normal);

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
vec3 gradient_normal(in vec3 texel);

// Function Implementations
// Spline
vec3 position_on_spline(in Spline spline, in float t) {
    return spline.a * t * t * t + spline.b * t * t + spline.c * t + spline.d;
}

bool intersect_transformed_spline_plane(in Spline spline, in float offset, inout float t) {
    const vec3 conversion = -spline.b / (3.0 * spline.a);
    Cubic cubic = cubic_constructor(
        spline.a.y,
        spline.b.y,
        spline.c.y,
        spline.d.y - offset
    );

    // Try all roots
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

    // Return all roots
    ts.x = conversion + first_root(cubic);
    ts.y = conversion + second_root(cubic);
    ts.z = conversion + third_root(cubic);
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

Ray get_frag_ray(in vec2 frag_coord, out vec3 origin, out vec3 direction) {
	const vec4 hray = vec4(frag_coord * 2.0 - vec2(1.0), 1.0, 1.0);
	const vec4 wray = uCamera.inverseProjCamera * hray;

	origin = uCamera.cameraWorldPos;
	direction = normalize( wray.xyz / wray.w - origin );

    return ray_constructor(origin, direction);
}

bool walk_spline_map(in SplineMap spline_map, in Ray ray, in ivec3 size, inout ivec3 texel, inout float t, inout vec3 normal) {
    vec2 ts;
    const bool result = intersect_ray_spline_map(ray, spline_map, ts);

    if (result) {
        float i = max(0.0, ts.x);
        while (i <= ts.y) {
            const vec3 pos = ray.origin + ray.direction * i;
            vec3 coords, raw_coords;

            if (texture_coords(spline_map, pos, coords, raw_coords)) {
                texel = ivec3(coords * size);
                const float color = texelFetch(texVol, texel, 0).r;

                // Intersection
                if (color > threshold) {
                    t = i;
                    normal = gradient_normal(coords);
                    return true;
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

void find_ray_spline_intersection(in Ray ray, in Spline spline, in float t, in vec3 offset, inout vec2 ret) {
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
        scalar1 > 0 && scalar1 < length1 &&
        scalar2 > 0 && scalar2 < length2
    );
}

bool intersect_ray_spline_map(in Ray ray, in SplineMap spline_map, inout vec2 ts) {
    ts = vec2(10.0, -10.0);

    const vec3 span1 = spline_map.base.span1;
    const vec3 span2 = spline_map.base.span2;

    // Project ray on planes orthogonal to spanning vectors of the base
    const Plane plane1 = plane_constructor(ray.origin, normalize(ray.direction - dot(ray.direction, span1) * span1), span1);
    const Plane plane2 = plane_constructor(ray.origin, normalize(ray.direction - dot(ray.direction, span2) * span2), span2);

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

    return ts.x < ts.y;
}

// SplineMap
bool texture_coords(in SplineMap spline_map, in vec3 pos, out vec3 coords, out vec3 raw_coords) {
    const vec4 p = spline_map.base.inv_matrix * vec4(pos, 1.0);
    float t;

    // Interpolate coordinates on base translated to spline intersection point
    if (intersect_transformed_spline_plane(spline_map.transformed_spline, p.y, t)) {
        const vec3 edge = position_on_spline(spline_map.transformed_spline, t);
        const vec3 diff = p.xyz - edge;

        const float xComp = diff.x / spline_map.width;
        const float yComp = 1.0 - p.y / spline_map.height;
        const float zComp = diff.z / spline_map.depth;

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

vec3 gradient_normal(in vec3 coord) {
    // return texture(gradientField, coord).rgb * 2.0 - 1.0;
    return normalize(vec3(
        texture(distanceField, coord - vec3(EPSILON, 0.0, 0.0)).r - texture(distanceField, coord + vec3(EPSILON, 0.0, 0.0)).r,
        texture(distanceField, coord - vec3(0.0, EPSILON, 0.0)).r - texture(distanceField, coord + vec3(0.0, EPSILON, 0.0)).r,
        texture(distanceField, coord - vec3(0.0, 0.0, EPSILON)).r - texture(distanceField, coord + vec3(0.0, 0.0, EPSILON)).r
    ));
}

void main() {
    oColor = vec3(0.0);

    vec3 origin;
    vec3 direction;
    Ray ray = get_frag_ray(gl_FragCoord.xy * uCamera.reciprocalWindowSize, origin, direction);

    ivec3 texel;
    float t;
    vec3 normal;
    if (walk_spline_map(uSplineMap.spline_map, ray, textureSize(texVol, 0), texel, t, normal)) {
        const float light = dot(normalize(vec3(1.0, 4.0, 2.0)), normal);
        oColor = vec3(max(0.0, light) * 0.7 + 0.3);
        // oColor = normal * 0.5 + 0.5;
        oColor *= vec3(texel) / vec3(textureSize(texVol, 0));
    } else {
        discard;
    }
}
