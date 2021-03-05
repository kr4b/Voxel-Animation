import { Sampler } from "./sampler.js";
import Spline from "./spline.js";
import { add, divide, max, min, mix, scale, subtract, vec2 } from "./vec2.js"

const VOLUME_STEPS: number = 1024;

class Ray {
  origin: vec2;
  dir:    vec2;

  constructor(origin: vec2, dir: vec2) {
    this.origin = origin;
    this.dir    = dir;
  }

  draw_point_at(ctx: CanvasRenderingContext2D, t: number): void {
    const point: vec2 = add(this.origin, scale(this.dir, t));

    ctx.beginPath();
    ctx.arc(
      Math.max(-ctx.canvas.width / 2, Math.min(ctx.canvas.width / 2, point.x)),
      Math.max(-ctx.canvas.height / 2, Math.min(ctx.canvas.height / 2, point.y)),
      0.02, 0.0, 2.0 * Math.PI);
    ctx.fill();
  }

  draw(ctx: CanvasRenderingContext2D): void {
    const end: vec2 = add(this.origin, this.dir);

    ctx.globalAlpha = 0.05;
    ctx.beginPath();
    ctx.moveTo(this.origin.x, this.origin.y);
    ctx.lineTo(end.x, end.y);
    ctx.stroke();
    ctx.globalAlpha = 1;
  }

  /// Intersection of this ray with the given AABB
  intersect_ray_aabb(aabbMin: vec2, aabbMax: vec2): vec2 {
    const t1: vec2 = divide(subtract(aabbMin, this.origin), this.dir);
    const t2: vec2 = divide(subtract(aabbMax, this.origin), this.dir);
    
    const mins: vec2 = min(t1, t2);
    const maxs: vec2 = max(t1, t2);

    const near: number = Math.max(mins.x, mins.y);
    const far:  number = Math.min(maxs.x, maxs.y);
    return vec2( near, far );
  }

  /// Intersection of this ray with the given sampler and AABB
  intersect_ray_sampler<T>(sampler: Sampler<T>): Spline | null {
    const ts: vec2 = this.intersect_ray_aabb(sampler.aabbMin, sampler.aabbMax);

    if (ts.x <= ts.y && ts.y >= 0.0) {
      if (ts.x < 0.0) {
        ts.x = 0.0;
      }

      const worldEntry: vec2 = add(this.origin, scale(this.dir, ts.x));
      const worldExit:  vec2 = add(this.origin, scale(this.dir, ts.y));

      const vscale: vec2 = subtract(sampler.aabbMax, sampler.aabbMin);
      const ventry: vec2 = divide(subtract(worldEntry, sampler.aabbMin), vscale);
      const vexit:  vec2 = divide(subtract(worldExit,  sampler.aabbMax), vscale);

      // Walk the ray from entry to exit to determine the intersection point
      for (let i = 0; i < VOLUME_STEPS; i++) {
        const ii: number = i / VOLUME_STEPS;

        const samplePos: vec2 = mix(ventry, vexit, ii);
        const sample: Spline | null = sampler.get(this, samplePos);

        if (sample != null) {
          return sample;
        }
      }
    }

    return null;
  }
}

export { Ray };