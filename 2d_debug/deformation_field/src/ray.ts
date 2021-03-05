import { add, divide, max, min, mix, scale, subtract, vec2 } from "./vec2"

const VOLUME_STEPS: number = 1024;

class Ray {
  origin: vec2;
  dir:    vec2;

  constructor(origin: vec2, dir: vec2) {
    this.origin = origin;
    this.dir = dir;
  }

  // Intersection of this ray with the given AABB
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
  intersect_ray_sampler<T>(aabbMin: vec2, aabbMax: vec2, samplerSize: number, sampler: Array<boolean>, data: Array<T>): T | null {
    const ts: vec2 = this.intersect_ray_aabb(aabbMin, aabbMax);

    if (ts.x <= ts.y && ts.y > 0.0) {
      if (ts.x < 0.0) {
        ts.x = 0.0;
      }

      const worldEntry: vec2 = add(this.origin, scale(this.dir, ts.x));
      const worldExit:  vec2 = add(this.origin, scale(this.dir, ts.y));

      const vscale: vec2 = subtract(aabbMax, aabbMin);
      const ventry: vec2 = divide(subtract(worldEntry, aabbMin), vscale);
      const vexit:  vec2 = divide(subtract(worldExit,  aabbMax), vscale);

      for (let i = 0; i < VOLUME_STEPS; i++) {
        const ii: number = i / VOLUME_STEPS;

        const samplePos: vec2 = mix(ventry, vexit, ii);
        const index:   number = Math.round(samplePos.y * samplerSize) + Math.round(samplePos.x);
        const voxel:  boolean = sampler[index];

        if (voxel) {
          return data[index];
        }
      }
    }

    return null;
  }
}

export { Ray };