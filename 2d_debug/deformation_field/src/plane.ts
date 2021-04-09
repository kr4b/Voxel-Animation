import { Ray } from "./ray.js";
import { add, divide, dot, multiply, scale, subtract, vec2, } from "./vec2.js";

class Plane {
  center: vec2;
  size: vec2;
  normal: vec2;

  constructor(center: vec2, size: vec2) {
    console.assert(size.x == 0.0 || size.y == 0.0);
    this.center = center;
    this.size = size;
    this.normal = vec2(-this.size.y, this.size.x);
  }

  intersect(ray: Ray): number {
    const discriminant = dot(this.normal, ray.dir);
    if (Math.abs(discriminant) < 1e-6) {
      return -1;
    }

    const t = dot(subtract(this.center, ray.origin), this.normal) / discriminant;
    if (t < 0.0) {
      return -1;
    }

    const worldPos = add(ray.origin, scale(ray.dir, t));
    const samplePos = scale(add(divide(subtract(worldPos, this.center), this.size), vec2(1.0, 1.0)), 0.5);
    let uv;
    if (this.size.x == 0.0) {
      uv = samplePos.y;
    } else {
      uv = samplePos.x;
    }

    if (uv < 0.0 || uv > 1.0) {
      return -1;
    }

    return uv;
  }
}

export { Plane };