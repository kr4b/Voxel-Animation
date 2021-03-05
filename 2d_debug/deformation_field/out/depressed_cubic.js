import { vec2 } from "./vec2.js";
var abs = Math.abs, acos = Math.acos, cos = Math.cos, PI = Math.PI, pow = Math.pow, sqrt = Math.sqrt;
var sign = function (n) { return +(0 < n) - +(n < 0); };
var DepressedCubic = /** @class */ (function () {
    function DepressedCubic(a, b, c, d) {
        this.p = (3 * a * c - b * b) / (3 * a * a);
        this.q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);
        this.discriminant = 27 * this.q * this.q + 4 * this.p * this.p * this.p;
    }
    DepressedCubic.prototype.find_roots = function () {
        if (this.discriminant > 0) {
            return this.single_root();
        }
        else {
            var fac = 2 * sqrt(-this.p / 3);
            var arccos = acos(3 * this.q / (2 * this.p) * sqrt(-3 / this.p)) / 3;
            return this.second_root(fac, arccos);
        }
    };
    DepressedCubic.prototype.single_root = function () {
        var D = sqrt(this.q * this.q / 4 + this.p * this.p * this.p / 27);
        var C0 = -0.5 * this.q + D;
        var C1 = -0.5 * this.q - D;
        return sign(C0) * pow(abs(C0), 1 / 3) + sign(C1) * pow(abs(C1), 1 / 3);
    };
    DepressedCubic.prototype.first_root = function (fac, arccos) {
        return fac * cos(arccos);
    };
    DepressedCubic.prototype.second_root = function (fac, arccos) {
        return fac * cos(arccos - 2 / 3 * PI);
    };
    DepressedCubic.prototype.third_root = function (fac, arccos) {
        return fac * cos(arccos - 4 / 3 * PI);
    };
    DepressedCubic.find_roots_static = function (a, b, c, d) {
        return vec2(new DepressedCubic(a.x, b.x, c.x, d.x).find_roots(), new DepressedCubic(a.y, b.y, c.y, d.y).find_roots());
    };
    return DepressedCubic;
}());
export default DepressedCubic;
