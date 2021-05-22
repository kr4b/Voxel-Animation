import { add, divide, dot, scale, subtract, vec2 } from "./vec2.js";
var Plane = /** @class */ (function () {
    function Plane(center, size) {
        console.assert(size.x == 0.0 || size.y == 0.0);
        this.center = center;
        this.size = size;
        this.normal = vec2(-this.size.y, this.size.x);
    }
    Plane.prototype.intersect = function (ray) {
        var discriminant = dot(this.normal, ray.dir);
        if (Math.abs(discriminant) < 1e-6) {
            return [-1, -1];
        }
        var t = dot(subtract(this.center, ray.origin), this.normal) / discriminant;
        if (t < 0.0) {
            return [-1, -1];
        }
        var worldPos = add(ray.origin, scale(ray.dir, t));
        var samplePos = scale(add(divide(subtract(worldPos, this.center), this.size), vec2(1.0, 1.0)), 0.5);
        var uv;
        if (this.size.x == 0.0) {
            uv = samplePos.y;
        }
        else {
            uv = samplePos.x;
        }
        if (uv < 0.0 || uv > 1.0) {
            return [-1, -1];
        }
        return [uv, t];
    };
    return Plane;
}());
export { Plane };
