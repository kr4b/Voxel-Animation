import { AABB } from "./aabb.js";
import DepressedCubic from "./depressed_cubic.js";
import { Plane } from "./plane.js";
import { add, copy, divide, mat3, multiply, scale, step, subtract, transform, vec2 } from "./vec2.js";

const { min, max, PI } = Math;
const EPSILON = vec2(1e-2, 1e-2);
const MAX_VALUE = vec2(2.0, 2.0);
const MIN_VALUE = scale(MAX_VALUE, -1);

class Spline {
    a: vec2;
    b: vec2;
    c: vec2;
    d: vec2;

    ts = vec2(0, 0);

    constructor(a: vec2, b: vec2, c: vec2, d: vec2) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
    }

    static with_tangents(point1: vec2, point2: vec2, tangent1: vec2, tangent2: vec2): Spline {
        const a = add(scale(point1, 2), scale(point2, -2), scale(tangent1, 1), scale(tangent2, 1));
        const b = add(scale(point1, -3), scale(point2, 3), scale(tangent1, -2), scale(tangent2, -1));
        const c = copy(tangent1);
        const d = copy(point1);

        return new Spline(a, b, c, d);
    }

    // http://www.cs.cmu.edu/~462/www/projects/assn2/assn2/catmullRom.pdf
    static with_control_points(point1: vec2, point2: vec2, control1: vec2, control2: vec2, tau: number = 0.2): Spline {
        const a = add(scale(control1, -1 * tau), scale(point1, 2 - tau), scale(point2, tau - 2), scale(control2, tau));
        const b = add(scale(control1, 2 * tau), scale(point1, tau - 3), scale(point2, 3 - 2 * tau), scale(control2, -tau));
        const c = add(scale(control1, -tau), scale(point2, tau));
        const d = copy(point1);

        return new Spline(a, b, c, d);
    }

    static transform(spline: Spline, matrix: mat3): Spline {
        const rotation = mat3(
            matrix.m00, matrix.m01, 0,
            matrix.m10, matrix.m11, 0,
            0, 0, 1
        );

        return new Spline(
            transform(spline.a, rotation),
            transform(spline.b, rotation),
            transform(spline.c, rotation),
            transform(spline.d, matrix)
        );
    }

    solve_quadratic(a: number, b: number, c: number): [number, number] {
        // ax^2 + bx + c = 0
        const D = b * b - 4.0 * a * c;

        if (D >= 0) {
            if (a === 0.0) {
                if (b === 0.0) {
                    return [-1, -1];
                }
                return [-c / b, -c / b];
            }
            return [
                (-b + Math.sqrt(D)) / (2.0 * a),
                (-b - Math.sqrt(D)) / (2.0 * a)
            ];
        }

        return [-1, -1];
    }

    get_extremes(): Float32Array {
        // Solve for t: at^2 + bt + c = 0
        const a: vec2 = scale(this.a, 3);
        const b: vec2 = scale(this.b, 2);

        const values: Float32Array = Float32Array.of(
            ...this.solve_quadratic(a.x, b.x, this.c.x),
            ...this.solve_quadratic(a.y, b.y, this.c.y),
            0, 1
        );

        return values;
    }

    draw_point_at(ctx: CanvasRenderingContext2D, t: number): void {
        const tt = t * t;
        const ttt = tt * t;

        const point = add(scale(this.a, ttt), scale(this.b, tt), scale(this.c, t), this.d);

        ctx.beginPath();
        ctx.arc(
            max(-ctx.canvas.width / 2, min(ctx.canvas.width / 2, point.x)),
            max(-ctx.canvas.height / 2, min(ctx.canvas.height / 2, point.y)),
            0.02, 0.0, 2.0 * PI);
        ctx.fill();
        ctx.stroke();
    }

    draw(ctx: CanvasRenderingContext2D, step: number = 0.01): void {
        let t = 0, tt, ttt;

        ctx.beginPath();
        ctx.moveTo(this.d.x, this.d.y);
        for (t = 0; t <= 1 + step; t += step) {
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
        for (t = 1; t <= 1; t += step) {
            tt = t * t;
            ttt = tt * t;
            ctx.lineTo(
                this.a.x * ttt + this.b.x * tt + this.c.x * t + this.d.x,
                this.a.y * ttt + this.b.y * tt + this.c.y * t + this.d.y
            );
        }
        ctx.stroke();
        ctx.setLineDash([]);
    }

    position_on_spline(t: number): vec2 {
        return add(scale(this.a, t * t * t), scale(this.b, t * t), scale(this.c, t), copy(this.d));
    }

    /**
     * Find out if the spline intersects with the AABB.
     * 
     * @param aabb the AABB to intersect
     * @returns true if it intersected, false otherwise
     */
    intersect_spline_aabb(aabb: AABB): boolean {
        const conversion: vec2 = divide(scale(this.b, -1), scale(add(this.a, vec2(1e-6, 1e-6)), 3));

        const cubic_min_x = new DepressedCubic(this.a.x, this.b.x, this.c.x, this.d.x - aabb.min.x);
        const cubic_min_y = new DepressedCubic(this.a.y, this.b.y, this.c.y, this.d.y - aabb.min.y);
        const cubic_max_x = new DepressedCubic(this.a.x, this.b.x, this.c.x, this.d.x - aabb.max.x);
        const cubic_max_y = new DepressedCubic(this.a.y, this.b.y, this.c.y, this.d.y - aabb.max.y);

        const t1 = add(conversion, vec2(cubic_min_x.second_root(), cubic_min_y.second_root()));
        const t2 = add(conversion, vec2(cubic_max_x.second_root(), cubic_max_y.second_root()));

        this.ts = vec2(2.0, -2.0);
        let result = this.calculate_near_far(t1, t2, aabb.min, aabb.max, this.ts);

        if (!result) {
            const first_t1: vec2 = add(conversion, vec2(cubic_min_x.first_root(), cubic_min_y.first_root()));
            const first_t2: vec2 = add(conversion, vec2(cubic_max_x.first_root(), cubic_max_y.first_root()));

            result = this.calculate_near_far(first_t1, first_t2, aabb.min, aabb.max, this.ts);

            if (!result) {
                const third_t1: vec2 = add(conversion, vec2(cubic_min_x.third_root(), cubic_min_y.third_root()));
                const third_t2: vec2 = add(conversion, vec2(cubic_max_x.third_root(), cubic_max_y.third_root()));

                result = this.calculate_near_far(third_t1, third_t2, aabb.min, aabb.max, this.ts);
            }
        }

        return result;
    }

    /**
     * Find out if the spline intersects with the Plane.
     * 
     * @param plane the plane to intersect
     * @returns true if it intersected, false otherwise
     */
    intersect_spline_plane(plane: Plane): [number, number, number] {
        const transformedSpline = Spline.transform(this, plane.inv_matrix);

        const conversion = -transformedSpline.b.y / (3.0 * transformedSpline.a.y);
        const cubic = new DepressedCubic(
            transformedSpline.a.y,
            transformedSpline.b.y,
            transformedSpline.c.y,
            transformedSpline.d.y
        );

        return [
            conversion + cubic.first_root(),
            conversion + cubic.second_root(),
            conversion + cubic.third_root()
        ];
    }

    intersect_spline_plane_transformed(y: number): [boolean, number] {
        const conversion: vec2 = divide(scale(this.b, -1), scale(add(this.a, vec2(1e-6, 1e-6)), 3));
        const cubic = new DepressedCubic(
            this.a.y,
            this.b.y,
            this.c.y,
            this.d.y - y
        );

        let t = conversion.y + cubic.first_root();
        if (t < -EPSILON.x || t > 1.0 + EPSILON.x) {
            t = conversion.y + cubic.second_root();
            if (t < -EPSILON.x || t > 1.0 + EPSILON.x) {
                t = conversion.y + cubic.third_root();
                if (t < -EPSILON.x || t > 1.0 + EPSILON.x) {
                    return [false, t];
                }
            }
        }

        return [true, t];
    }

    intersect_spline_infinite_plane(plane: Plane, ctx: CanvasRenderingContext2D): boolean {
        const transformed_spline = Spline.transform(this, plane.inv_matrix);
        ctx.save();
        ctx.strokeStyle = "crimson";
        transformed_spline.draw(ctx);

        ctx.strokeStyle = "black";
        ctx.beginPath();
        ctx.moveTo(-10, 0);
        ctx.lineTo(10, 0);
        ctx.stroke();
        ctx.restore();

        const that = transformed_spline;

        const conversion: vec2 = divide(scale(that.b, -1), scale(add(that.a, vec2(1e-6, 1e-6)), 3));

        const cubic_x = new DepressedCubic(that.a.x, that.b.x, that.c.x, that.d.x);
        const cubic_y = new DepressedCubic(that.a.y, that.b.y, that.c.y, that.d.y);

        const t1 = add(conversion, vec2(cubic_x.second_root(), cubic_y.second_root()));

        ctx.save();
        ctx.fillStyle = "blue";
        that.draw_point_at(ctx, t1.y);
        ctx.restore();

        console.log("T1", t1);

        return true;

        // this.ts = vec2(2.0, -2.0);
        // let result = this.calculate_near_far(t1, t2, aabb.min, aabb.max, this.ts);

        // if (!result) {
        //     const first_t1: vec2 = add(conversion, vec2(cubic_min_x.first_root(), cubic_min_y.first_root()));
        //     const first_t2: vec2 = add(conversion, vec2(cubic_max_x.first_root(), cubic_max_y.first_root()));

        //     result = this.calculate_near_far(first_t1, first_t2, aabb.min, aabb.max, this.ts);

        //     if (!result) {
        //         const third_t1: vec2 = add(conversion, vec2(cubic_min_x.third_root(), cubic_min_y.third_root()));
        //         const third_t2: vec2 = add(conversion, vec2(cubic_max_x.third_root(), cubic_max_y.third_root()));

        //         result = this.calculate_near_far(third_t1, third_t2, aabb.min, aabb.max, this.ts);
        //     }
        // }

        // return result;
    }

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
    private calculate_near_far(t1: vec2, t2: vec2, aabbMin: vec2, aabbMax: vec2, ts: vec2): boolean {
        const it1 = this.intersected_aabb(t1, aabbMin, aabbMax);
        const it2 = this.intersected_aabb(t2, aabbMin, aabbMax);

        const nt1 = add(multiply(t1, it1), multiply(subtract(vec2(1, 1), it1), MAX_VALUE));
        const nt2 = add(multiply(t2, it2), multiply(subtract(vec2(1, 1), it2), MAX_VALUE));

        const ft1 = add(multiply(t1, it1), multiply(subtract(vec2(1, 1), it1), MIN_VALUE));
        const ft2 = add(multiply(t2, it2), multiply(subtract(vec2(1, 1), it2), MIN_VALUE));

        const inear = vec2(min(nt1.x, nt2.x), min(nt1.y, nt2.y));
        const ifar = vec2(max(ft1.x, ft2.x), max(ft1.y, ft2.y));

        ts.x = min(ts.x, min(inear.x, inear.y));
        ts.y = max(ts.y, max(ifar.x, ifar.y));

        return ts.x <= ts.y && ts.y >= 0;
    }

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
