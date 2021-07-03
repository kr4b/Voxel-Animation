import { AABB } from "./aabb.js";
import { Plane } from "./plane.js";
import { Ray } from "./ray.js";
import Spline from "./spline.js";
import { add, dot, multiply, scale, subtract, vec2 } from "./vec2.js";

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
    const plane = new Plane(pos, this.base.size);
    // Get plane spline intersection
    // TODO: This check can probably be skipped when using an infinite plane
    if (this.spline.intersect_spline_plane(plane)) {
      return null;
    }
    const t = this.spline.ts.x;
    // Determine whether the point is in the plane
    const edgePos1 = this.spline.position_on_spline(t);
    const diff1 = multiply(subtract(edgePos1, pos), subtract(edgePos1, pos));
    const edgePos2 = add(edgePos1, this.base.size);
    const diff2 = multiply(subtract(edgePos2, pos), subtract(edgePos2, pos));
    const sizeSquared = multiply(this.base.size, this.base.size);
    if (diff1 > sizeSquared || diff2 > sizeSquared) {
      return null;
    }

    // Calculate and return texture coords
    return subtract(pos, edgePos1);
  }
}

export default SplineMap;