import { Plane } from "./plane.js";
import { add, length, mix, multiply, norm, scale, subtract, vec2 } from "./vec2.js";
var SQUARE_MAP_POSITIVE_X = 0;
var SQUARE_MAP_NEGATIVE_X = 1;
var SQUARE_MAP_POSITIVE_Y = 2;
var SQUARE_MAP_NEGATIVE_Y = 3;
var PLANE_COLORS = [
    [52, 152, 219],
    [155, 89, 182],
    [46, 204, 113],
    [241, 196, 15]
];
var PlaneSampler = /** @class */ (function () {
    function PlaneSampler(min, max, original, textures, builder) {
        this.planes = new Array(4);
        this.textures = textures;
        this.builder = builder;
        if (this.textures.some(function (t) { return t.length != textures[0].length; }))
            throw "Textures should all be of the same size";
        this.init_planes(min, max, original);
    }
    PlaneSampler.prototype.init_planes = function (min, max, original) {
        var size = scale(vec2(max.x - min.x, max.y - min.y), 0.5);
        for (var face = 0; face < this.planes.length; face++) {
            switch (face) {
                case SQUARE_MAP_POSITIVE_X:
                    this.planes[face] = new Plane(vec2(original.max.x, min.y + size.y), vec2(0, size.y));
                    break;
                case SQUARE_MAP_NEGATIVE_X:
                    this.planes[face] = new Plane(vec2(original.min.x, min.y + size.y), vec2(0, size.y));
                    break;
                case SQUARE_MAP_POSITIVE_Y:
                    this.planes[face] = new Plane(vec2(min.x + size.x, original.max.y), vec2(size.x, 0));
                    break;
                case SQUARE_MAP_NEGATIVE_Y:
                    this.planes[face] = new Plane(vec2(min.x + size.x, original.min.y), vec2(size.x, 0));
                    break;
                default:
                    console.warn("???");
            }
        }
    };
    PlaneSampler.prototype.intersect = function (ray) {
        var _this = this;
        var final = [-1, vec2(0, 0)];
        this.planes
            .map(function (plane, i) { return [plane, i]; })
            .sort(function (a, b) { return length(a[0].center, ray.origin) - length(b[0].center, ray.origin); })
            .forEach(function (_a) {
            var plane = _a[0], i = _a[1];
            var result = plane.intersect(ray);
            if (result != -1 && final[0] < 0) {
                var tex_len = _this.textures[i].length;
                var tex_space_result = result * (tex_len - 1);
                var major = Math.floor(tex_space_result);
                var minor = tex_space_result - major;
                final = [i, mix(_this.textures[i][major], _this.textures[i][major + 1], minor)];
            }
        });
        if (final[0] >= 0) {
            return this.builder(ray, final[0], final[1], PLANE_COLORS[final[0]]);
        }
        return null;
    };
    // get(ray: Ray, samplePos: vec2): Spline | null {
    //     const x: number = Math.round((this.size - 1) * samplePos.x);
    //     const y: number = Math.round((this.size - 1) * samplePos.y);
    //     const index: number = y * this.size + x;
    //     if (index < 0 || index >= this.data.length || !this.sampler[index]) {
    //         return null;
    //     }
    //     return this.make_spline(ray, this.data[index], this.colors[index]);
    // }
    PlaneSampler.prototype.draw = function (ctx) {
        var width = 2.0;
        ctx.lineWidth *= width;
        this.planes.forEach(function (plane, i) {
            var normal = norm(plane.normal);
            var min = subtract(plane.center, multiply(plane.size, vec2(normal.y, normal.x)));
            var max = add(plane.center, multiply(plane.size, vec2(normal.y, normal.x)));
            ctx.strokeStyle = "rgb(" + PLANE_COLORS[i][0] + ", " + PLANE_COLORS[i][1] + ", " + PLANE_COLORS[i][2] + ")";
            ctx.beginPath();
            ctx.moveTo(min.x, min.y);
            ctx.lineTo(max.x, max.y);
            ctx.stroke();
        });
        ctx.lineWidth /= width;
    };
    return PlaneSampler;
}());
export { PlaneSampler };
