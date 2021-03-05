import DepressedCubic from "./depressed_cubic";
import { add, copy, divide, multiply, scale, step, subtract, vec2 } from "./vec2";

const { min, max, PI } = Math;
const EPSILON = vec2(1e-6, 1e-6);
const MAX_VALUE = vec2(1e6, 1e6);
const MIN_VALUE = scale(MAX_VALUE, -1);

class Spline {
    a: vec2;
    b: vec2;
    c: vec2;
    d: vec2;

    intersection = false;
    ts    = vec2(0, 0);
    entry = vec2(0, 0);
    exit  = vec2(0, 0);

    constructor(a: vec2, b: vec2, c: vec2, d: vec2) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
    }

    static with_tangents(point1: vec2, point2: vec2, tangent1: vec2, tangent2: vec2): Spline {
        const a = add(scale(point1,  2), scale(point2, -2), scale(tangent1,  1), scale(tangent2,  1));
        const b = add(scale(point1, -3), scale(point2,  3), scale(tangent1, -2), scale(tangent2, -1));
        const c = copy(tangent1);
        const d = copy(point1);

        return new Spline(a, b, c, d);
    }

    static with_control_points(point1: vec2, point2: vec2, control1: vec2, control2: vec2): Spline {
        const tau = 0.2;

        const a = add(scale(control1, -1 * tau), scale(point1, 2 - tau), scale(point2, tau - 2), scale(control2,  tau));
        const b = add(scale(control1,  2 * tau), scale(point1, tau - 3), scale(point2, 3 - tau), scale(control2, -tau));
        const c = add(scale(control1, -tau), scale(point2, tau));
        const d = copy(point1);

        return new Spline(a, b, c, d);
    }

    draw_point_at(ctx: CanvasRenderingContext2D, t: number) {
        const tt = t * t;
        const ttt = tt * t;

        const point = add(scale(this.a, ttt), scale(this.b, tt), scale(this.c, t), this.d);

        ctx.beginPath();
        ctx.arc(
            max(-ctx.canvas.width / 2, min(ctx.canvas.width / 2, point.x)),
            max(-ctx.canvas.height / 2, min(ctx.canvas.height / 2, point.y)),
            0.02, 0.0, 2.0 * PI);
        ctx.fill();
    }

    draw(ctx: CanvasRenderingContext2D, step: number = 0.01) {
        let t = 0, tt, ttt;

        ctx.globalAlpha = 0.2;
        ctx.beginPath();
        ctx.moveTo(this.d.x, this.d.y);
        for (t = step; t < 1; t += step) {
            tt = t * t;
            ttt = tt * t;
            ctx.lineTo(
                this.a.x * ttt + this.b.x * tt + this.c.x * t + this.d.x,
                this.a.y * ttt + this.b.y * tt + this.c.y * t + this.d.y
            );
        }

        ctx.stroke();
        ctx.beginPath()
        ctx.setLineDash([0.05, 0.1]);
        for (t = 1; t < 1; t += step) {
            tt = t * t;
            ttt = tt * t;
            ctx.lineTo(
                this.a.x * ttt + this.b.x * tt + this.c.x * t + this.d.x,
                this.a.y * ttt + this.b.y * tt + this.c.y * t + this.d.y
            );
        }
        ctx.stroke();
        ctx.setLineDash([]);
        ctx.globalAlpha = 1;
    }

    position_on_spline(t: number): vec2 {
	    return add(scale(this.a, t * t * t), scale(this.b, t * t), scale(this.c, t), copy(this.d));
    }

    intersect_spline_aabb(aabbMin: vec2, aabbMax: vec2): void {
        const conversion: vec2 = divide(scale(this.b, -1), scale(this.a, 3));

        const t1 = add(conversion, DepressedCubic.find_roots_static(this.a, this.b, this.c, subtract(this.d, aabbMin)));
        const t2 = add(conversion, DepressedCubic.find_roots_static(this.a, this.b, this.c, subtract(this.d, aabbMax)));

        const ts = vec2(0, 0);
        this.calculate_near_far(t1, t2, aabbMin, aabbMax, ts);

        this.ts = ts;
        this.intersection = ts.x <= ts.y && ts.y >= 0;
        this.entry = this.position_on_spline(ts.x);
        this.exit  = this.position_on_spline(ts.y);
    }

    private calculate_near_far(t1: vec2, t2: vec2, aabbMin: vec2, aabbMax: vec2, ts: vec2): void {
        const it1 = this.intersected_aabb(t1, aabbMin, aabbMax);
        const it2 = this.intersected_aabb(t2, aabbMin, aabbMax);

        const nt1 = add(multiply(t1, it1), multiply(subtract(vec2(1, 1), it1), MAX_VALUE));
        const nt2 = add(multiply(t2, it2), multiply(subtract(vec2(1, 1), it2), MAX_VALUE));

        const ft1 = add(multiply(t1, it1), multiply(subtract(vec2(1, 1), it1), MIN_VALUE));
        const ft2 = add(multiply(t2, it2), multiply(subtract(vec2(1, 1), it2), MIN_VALUE));

        const inear = vec2(min(nt1.x, nt2.x), min(nt1.y, nt2.y));
        const ifar  = vec2(max(nt1.x, nt2.x), max(nt1.y, nt2.y));

        ts.x = min(inear.x, inear.y);
        ts.y = max(ifar.x,  ifar.y);
    }

    private intersected_aabb(t: vec2, aabbMin: vec2, aabbMax: vec2): vec2 {
        const P0 = this.position_on_spline(t.x);
        const P1 = this.position_on_spline(t.y);

        const resultT = multiply(step(vec2(0, 0), t), step(t, vec2(1, 1)));
        const result0 = multiply(step(subtract(aabbMin, EPSILON), P0), step(P0, add(aabbMax, EPSILON)));
        const result1 = multiply(step(subtract(aabbMin, EPSILON), P1), step(P1, add(aabbMax, EPSILON)));
        
        return vec2(
            resultT.x * result0.x * result0.y,
            resultT.y * result1.x * result1.y);
    }
}

export default Spline;