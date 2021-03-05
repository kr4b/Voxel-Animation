import { add, divide, max, min, mix, scale, subtract, vec2 } from "./vec2.js";
var VOLUME_STEPS = 1024;
var Ray = /** @class */ (function () {
    function Ray(origin, dir) {
        this.origin = origin;
        this.dir = dir;
    }
    /// Intersection of this ray with the given AABB
    Ray.prototype.intersect_ray_aabb = function (aabbMin, aabbMax) {
        var t1 = divide(subtract(aabbMin, this.origin), this.dir);
        var t2 = divide(subtract(aabbMax, this.origin), this.dir);
        var mins = min(t1, t2);
        var maxs = max(t1, t2);
        var near = Math.max(mins.x, mins.y);
        var far = Math.min(maxs.x, maxs.y);
        return vec2(near, far);
    };
    /// Intersection of this ray with the given sampler and AABB
    Ray.prototype.intersect_ray_sampler = function (sampler) {
        var ts = this.intersect_ray_aabb(sampler.aabbMin, sampler.aabbMax);
        if (ts.x <= ts.y && ts.y > 0.0) {
            if (ts.x < 0.0) {
                ts.x = 0.0;
            }
            var worldEntry = add(this.origin, scale(this.dir, ts.x));
            var worldExit = add(this.origin, scale(this.dir, ts.y));
            var vscale = subtract(sampler.aabbMax, sampler.aabbMin);
            var ventry = divide(subtract(worldEntry, sampler.aabbMin), vscale);
            var vexit = divide(subtract(worldExit, sampler.aabbMax), vscale);
            // Walk the ray from entry to exit to determine the intersection point
            for (var i = 0; i < VOLUME_STEPS; i++) {
                var ii = i / VOLUME_STEPS;
                var samplePos = mix(ventry, vexit, ii);
                var sample = sampler.get(samplePos);
                if (sample != null) {
                    return sample;
                }
            }
        }
        return null;
    };
    return Ray;
}());
export { Ray };
