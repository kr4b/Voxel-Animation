import { add, divide, max, min, mix, scale, subtract, vec2 } from "./vec2";
var VOLUME_STEPS = 1024;
var Ray = /** @class */ (function () {
    function Ray(origin, dir) {
        this.origin = origin;
        this.dir = dir;
    }
    // Intersection of this ray with the given AABB
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
    Ray.prototype.intersect_ray_sampler = function (aabbMin, aabbMax, samplerSize, sampler, data) {
        var ts = this.intersect_ray_aabb(aabbMin, aabbMax);
        if (ts.x <= ts.y && ts.y > 0.0) {
            if (ts.x < 0.0) {
                ts.x = 0.0;
            }
            var worldEntry = add(this.origin, scale(this.dir, ts.x));
            var worldExit = add(this.origin, scale(this.dir, ts.y));
            var vscale = subtract(aabbMax, aabbMin);
            var ventry = divide(subtract(worldEntry, aabbMin), vscale);
            var vexit = divide(subtract(worldExit, aabbMax), vscale);
            for (var i = 0; i < VOLUME_STEPS; i++) {
                var ii = i / VOLUME_STEPS;
                var samplePos = mix(ventry, vexit, ii);
                var index = Math.round(samplePos.y * samplerSize) + Math.round(samplePos.x);
                var voxel = sampler[index];
                if (voxel) {
                    return data[index];
                }
            }
        }
        return null;
    };
    return Ray;
}());
export { Ray };
