import DepressedCubic from "./depressed_cubic.js";
import { add, copy, divide, multiply, scale, step, subtract, vec2 } from "./vec2.js";
var min = Math.min, max = Math.max, PI = Math.PI;
var EPSILON = vec2(1e-6, 1e-6);
var MAX_VALUE = vec2(1e6, 1e6);
var MIN_VALUE = scale(MAX_VALUE, -1);
var Spline = /** @class */ (function () {
    function Spline(a, b, c, d) {
        this.color = [0, 0, 0];
        this.ts = vec2(0, 0);
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
    }
    Spline.with_tangents = function (point1, point2, tangent1, tangent2) {
        var a = add(scale(point1, 2), scale(point2, -2), scale(tangent1, 1), scale(tangent2, 1));
        var b = add(scale(point1, -3), scale(point2, 3), scale(tangent1, -2), scale(tangent2, -1));
        var c = copy(tangent1);
        var d = copy(point1);
        return new Spline(a, b, c, d);
    };
    Spline.with_control_points = function (point1, point2, control1, control2) {
        var tau = 0.2;
        var a = add(scale(control1, -1 * tau), scale(point1, 2 - tau), scale(point2, tau - 2), scale(control2, tau));
        var b = add(scale(control1, 2 * tau), scale(point1, tau - 3), scale(point2, 3 - tau), scale(control2, -tau));
        var c = add(scale(control1, -tau), scale(point2, tau));
        var d = copy(point1);
        return new Spline(a, b, c, d);
    };
    Spline.prototype.draw_point_at = function (ctx, t) {
        var tt = t * t;
        var ttt = tt * t;
        var point = add(scale(this.a, ttt), scale(this.b, tt), scale(this.c, t), this.d);
        ctx.beginPath();
        ctx.arc(max(-ctx.canvas.width / 2, min(ctx.canvas.width / 2, point.x)), max(-ctx.canvas.height / 2, min(ctx.canvas.height / 2, point.y)), 0.02, 0.0, 2.0 * PI);
        ctx.fill();
        ctx.stroke();
    };
    Spline.prototype.draw = function (ctx, step) {
        if (step === void 0) { step = 0.01; }
        var t = 0, tt, ttt;
        ctx.globalAlpha = 0.5;
        ctx.strokeStyle = "rgb(" + this.color[0] + "," + this.color[1] + "," + this.color[2] + ")";
        ctx.beginPath();
        ctx.moveTo(this.d.x, this.d.y);
        for (t = step; t < 1; t += step) {
            tt = t * t;
            ttt = tt * t;
            ctx.lineTo(this.a.x * ttt + this.b.x * tt + this.c.x * t + this.d.x, this.a.y * ttt + this.b.y * tt + this.c.y * t + this.d.y);
        }
        ctx.stroke();
        ctx.beginPath();
        ctx.setLineDash([0.05, 0.1]);
        for (t = 1; t < 1; t += step) {
            tt = t * t;
            ttt = tt * t;
            ctx.lineTo(this.a.x * ttt + this.b.x * tt + this.c.x * t + this.d.x, this.a.y * ttt + this.b.y * tt + this.c.y * t + this.d.y);
        }
        ctx.stroke();
        ctx.setLineDash([]);
        ctx.strokeStyle = "black";
        ctx.globalAlpha = 1;
    };
    Spline.prototype.position_on_spline = function (t) {
        return add(scale(this.a, t * t * t), scale(this.b, t * t), scale(this.c, t), copy(this.d));
    };
    Spline.prototype.set_color = function (color) {
        this.color = color;
    };
    /**
     * Find out if the spline intersects with the AABB.
     *
     * @param aabb the AABB of this
     * @returns true if it intersected, false otherwise
     */
    Spline.prototype.intersect_spline_aabb = function (aabb) {
        var conversion = divide(scale(this.b, -1), scale(this.a, 3));
        var t1 = add(conversion, DepressedCubic.find_roots_static(this.a, this.b, this.c, subtract(this.d, aabb.min)));
        var t2 = add(conversion, DepressedCubic.find_roots_static(this.a, this.b, this.c, subtract(this.d, aabb.max)));
        this.calculate_near_far(t1, t2, aabb.min, aabb.max, this.ts);
        return this.ts.x <= this.ts.y && this.ts.y >= 0;
    };
    /**
     * Calculates the closest to and furthest from 0 intersection points with the aabb.
     *
     * @param t1 the first two `t` values, usually computed by intersecting with the
     * minimum corner of the aabb
     * @param t2 the second two `t` values, usually computed by intersecting with the
     * maximum corner of the aabb
     * @param aabbMin the minimum bounds of the aabb
     * @param aabbMax the maximum bounds of the aabb
     * @param ts object in which the results are stored, `x` will contain the nearest
     * intersection `t`, `y` the furthest
     */
    Spline.prototype.calculate_near_far = function (t1, t2, aabbMin, aabbMax, ts) {
        var it1 = this.intersected_aabb(t1, aabbMin, aabbMax);
        var it2 = this.intersected_aabb(t2, aabbMin, aabbMax);
        var nt1 = add(multiply(t1, it1), multiply(subtract(vec2(1, 1), it1), MAX_VALUE));
        var nt2 = add(multiply(t2, it2), multiply(subtract(vec2(1, 1), it2), MAX_VALUE));
        var ft1 = add(multiply(t1, it1), multiply(subtract(vec2(1, 1), it1), MIN_VALUE));
        var ft2 = add(multiply(t2, it2), multiply(subtract(vec2(1, 1), it2), MIN_VALUE));
        var inear = vec2(min(nt1.x, nt2.x), min(nt1.y, nt2.y));
        var ifar = vec2(max(ft1.x, ft2.x), max(ft1.y, ft2.y));
        ts.x = min(inear.x, inear.y);
        ts.y = max(ifar.x, ifar.y);
    };
    /**
     * Determines if the point at time `t` on the spline intersects with the
     * aabb described by a min and max corner.
     * A return value of 0 indicates no intersection, while 1 indicates that
     * there is an intersection.
     *
     * @param t interpolation values, range from 0 to 1
     * @param aabbMin the minimum bounds of the aabb
     * @param aabbMax the maximum bounds of the aabb
     * @returns a vector with 0 or 1 for each of the two `t` values.
     */
    Spline.prototype.intersected_aabb = function (t, aabbMin, aabbMax) {
        var P0 = this.position_on_spline(t.x);
        var P1 = this.position_on_spline(t.y);
        var resultT = multiply(step(vec2(0, 0), t), step(t, vec2(1, 1)));
        var result0 = multiply(step(subtract(aabbMin, EPSILON), P0), step(P0, add(aabbMax, EPSILON)));
        var result1 = multiply(step(subtract(aabbMin, EPSILON), P1), step(P1, add(aabbMax, EPSILON)));
        return vec2(resultT.x * result0.x * result0.y, resultT.y * result1.x * result1.y);
    };
    return Spline;
}());
export default Spline;
