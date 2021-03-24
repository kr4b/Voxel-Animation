var abs = Math.abs, acos = Math.acos, cos = Math.cos, PI = Math.PI, pow = Math.pow, sqrt = Math.sqrt;
var sign = function (n) { return +(0 < n) - +(n < 0); };
var DepressedCubic = /** @class */ (function () {
    function DepressedCubic(a, b, c, d) {
        this.fac = 0;
        this.root = 0;
        this.arccos = 0;
        this.p = (3 * a * c - b * b) / (3 * a * a);
        this.q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);
        this.discriminant = 27 * this.q * this.q + 4 * this.p * this.p * this.p;
        this.calculate_default_root();
    }
    DepressedCubic.prototype.calculate_default_root = function () {
        if (this.discriminant > 0) {
            this.root = this.single_root();
        }
        else {
            this.fac = 2 * sqrt(-this.p / 3);
            this.arccos = acos(3 * this.q / (2 * this.p) * sqrt(-3 / this.p)) / 3;
            this.root = this.second_root();
        }
    };
    DepressedCubic.prototype.single_root = function () {
        var D = sqrt(this.q * this.q / 4 + this.p * this.p * this.p / 27);
        var C0 = -0.5 * this.q + D;
        var C1 = -0.5 * this.q - D;
        return sign(C0) * pow(abs(C0), 1 / 3) + sign(C1) * pow(abs(C1), 1 / 3);
    };
    DepressedCubic.prototype.first_root = function () {
        if (this.discriminant <= 0)
            this.root = this.fac * cos(this.arccos);
        return this.root;
    };
    DepressedCubic.prototype.second_root = function () {
        if (this.discriminant <= 0)
            this.root = this.fac * cos(this.arccos - 2 / 3 * PI);
        return this.root;
    };
    DepressedCubic.prototype.third_root = function () {
        if (this.discriminant <= 0)
            this.root = this.fac * cos(this.arccos - 4 / 3 * PI);
        return this.root;
    };
    return DepressedCubic;
}());
export default DepressedCubic;
