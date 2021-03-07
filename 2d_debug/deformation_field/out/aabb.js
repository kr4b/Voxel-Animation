import { subtract, min, max } from "./vec2.js";
var AABB = /** @class */ (function () {
    function AABB(aabbMin, aabbMax) {
        this.min = min(aabbMin, aabbMax);
        this.max = max(aabbMin, aabbMax);
    }
    AABB.prototype.size = function () {
        return subtract(this.max, this.min);
    };
    return AABB;
}());
export { AABB };
