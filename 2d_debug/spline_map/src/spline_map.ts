import { AABB } from "./aabb.js";
import { Plane } from "./plane.js";
import { Ray } from "./ray.js";
import Spline from "./spline.js";
import { dot, scale, subtract, vec2 } from "./vec2.js";

class SplineMap {
  aabb: AABB;
  base: Plane;
  spline: Spline;

  constructor(aabb: AABB, base: Plane, spline: Spline) {
    this.aabb = aabb;
    this.base = base;
    this.spline = spline;
  }

  intersect_ray(ray: Ray): vec2 {
    return ray.intersect_ray_aabb(this.aabb);
  }

  // Calculate texture coords for a given position in world space
  texture_coords(pos: vec2): vec2 | null {
    // Create base plane at the given position
    // TODO: use infinite plane instead of scaling
    const plane = new Plane(pos, scale(this.base.half_size, 2.0));
    // Get plane spline intersection
    // TODO: This check can probably be skipped when using an infinite plane
    if (this.spline.intersect_spline_plane(plane)) {
      return null;
    }
    const t = this.spline.ts.x;
    // Determine whether the point is in the plane
    const edgePos = this.spline.position_on_spline(t);
    const posVec = subtract(pos, edgePos);
    // TODO: Make this work for any base
    const edgeVec = vec2(scale(this.base.half_size, 2.0).x, 0.0);
    const dotP = dot(posVec, edgeVec);
    if (dotP < 0.0 || dotP > dot(edgeVec, edgeVec)) {
      return null;
    }

    // Calculate and return texture coords
    return subtract(pos, edgePos);
  }
}

export default SplineMap;