import { AABB } from "./aabb.js";
import { vec2 } from "./vec2.js";
var SQUARE_MAP_POSITIVE_X = 0;
var SQUARE_MAP_NEGATIVE_X = 1;
var SQUARE_MAP_POSITIVE_Y = 2;
var SQUARE_MAP_NEGATIVE_Y = 3;
var PlaneSampler = /** @class */ (function () {
    function PlaneSampler(min, max, original, textures) {
        this.planes = new Array(4);
        this.textures = textures;
        if (this.textures.some(function (t) { return t.length != textures[0].length; }))
            throw "Textures should all be of the same size";
        this.init_planes(min, max, original);
    }
    PlaneSampler.prototype.init_planes = function (min, max, original) {
        for (var face = 0; face < this.planes.length; face++) {
            switch (face) {
                case SQUARE_MAP_POSITIVE_X:
                    this.planes[face] = new AABB(vec2(original.max.x, min.y), vec2(original.max.x, max.y));
                    break;
                case SQUARE_MAP_NEGATIVE_X:
                    this.planes[face] = new AABB(vec2(original.min.x, min.y), vec2(original.min.x, max.y));
                    break;
                case SQUARE_MAP_POSITIVE_Y:
                    this.planes[face] = new AABB(vec2(min.x, original.max.y), vec2(max.x, original.max.y));
                    break;
                case SQUARE_MAP_NEGATIVE_Y:
                    this.planes[face] = new AABB(vec2(min.x, original.min.y), vec2(max.x, original.min.y));
                    break;
                default:
                    console.warn("???");
            }
        }
    };
    PlaneSampler.prototype.draw = function (ctx) {
        var width = 2.0;
        ctx.lineWidth *= width;
        this.planes.forEach(function (plane, i) {
            ctx.strokeStyle = "hsl(" + 360 / 5 * i + ", 100%, 50%)";
            ctx.beginPath();
            ctx.moveTo(plane.min.x, plane.min.y);
            ctx.lineTo(plane.max.x, plane.max.y);
            ctx.stroke();
        });
        ctx.lineWidth /= width;
    };
    return PlaneSampler;
}());
export { PlaneSampler };
