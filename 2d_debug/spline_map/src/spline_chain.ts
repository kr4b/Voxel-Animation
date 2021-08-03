import { AABB } from "./aabb.js";
import { Plane } from "./plane.js";
import Spline from "./spline.js";
import { add, mat3, scale, subtract, transform, vec2 } from "./vec2.js";

class SplineChain {
    splines: Spline[];
    private amount: number;

    ts = vec2(0, 0);

    constructor(splines: Spline[]) {
        this.splines = splines;
        this.amount = splines.length;
    }

    static from_points(points: vec2[], tau: number = 0.2): SplineChain {
        points.unshift(points[0]);
        points.push(points[points.length - 1]);

        const splines = [];
        for (let index = 1; index < points.length - 2; index++) {
            splines.push(
                Spline.with_control_points(
                    points[index],
                    points[index + 1],
                    points[index - 1],
                    points[index + 2],
                    tau
                )
            );
        }

        return new SplineChain(splines);
    }

    static from_points_with_outer_tangents(first_tangent: vec2, last_tangent: vec2, points: vec2[], tau: number = 0.2): SplineChain {
        points.unshift(subtract(points[0], first_tangent));
        points.push(add(points[points.length - 1], last_tangent));

        const splines = [];
        for (let index = 1; index < points.length - 2; index++) {
            splines.push(
                Spline.with_control_points(
                    points[index],
                    points[index + 1],
                    points[index - 1],
                    points[index + 2],
                    tau
                )
            );
        }

        return new SplineChain(splines);
    }

    static from_points_with_tangents(points: vec2[], tangents: vec2[]): SplineChain {
        if (points.length !== tangents.length) throw "Unequal amount of points and tangents";

        const splines = [];
        for (let index = 0; index < points.length - 1; index++) {
            splines.push(
                Spline.with_tangents(
                    points[index],
                    points[index + 1],
                    tangents[index],
                    tangents[index + 1]
                )
            );
        }

        return new SplineChain(splines);
    }

    static transform(spline_chain: SplineChain, matrix: mat3): SplineChain {
        return new SplineChain(spline_chain.splines.map(spline => Spline.transform(spline, matrix)));
    }

    get_extremes(): Float32Array {
        const results: number[] = [];
        
        for (let i = 0; i < this.splines.length; i++) {
            this.splines[i].get_extremes().forEach(value => results.push((value + i) / this.amount));
        }

        return Float32Array.from(results);
    }

    intersect_spline_plane(plane: Plane): boolean {
        const transformed_min = transform(plane.min, plane.inv_matrix);
        const transformed_max = transform(plane.max, plane.inv_matrix);

        let intersection: boolean = false;

        for (let i = 0; i < this.splines.length; i++) {
            const transformed_spline = Spline.transform(this.splines[i], plane.inv_matrix);
            intersection ||= transformed_spline.intersect_spline_aabb(new AABB(transformed_min, transformed_max));
            this.ts = scale(add(transformed_spline.ts, vec2(i, i)), 1.0 / this.amount);

            if (intersection) break;
        }

        return intersection;
    }

    intersect_spline_chain_plane_transformed(y: number): [boolean, number] {
        return this.splines[0].intersect_spline_plane_transformed(y);
    }

    draw_point_at(ctx: CanvasRenderingContext2D, t: number): void {
        const point = this.position_on_chain(t);

        ctx.beginPath();
        ctx.arc(
            Math.max(-ctx.canvas.width / 2, Math.min(ctx.canvas.width / 2, point.x)),
            Math.max(-ctx.canvas.height / 2, Math.min(ctx.canvas.height / 2, point.y)),
            0.02, 0.0, 2.0 * Math.PI);
        ctx.fill();
        ctx.stroke();
    }

    draw(ctx: CanvasRenderingContext2D, step: number = 0.01): void {
        for (const spline of this.splines) {
            spline.draw(ctx, step);
        }
    }

    position_on_chain(t: number): vec2 {
        t = Math.max(0.0, Math.min(t, 1.0 - 1e-4));
        const index: number = Math.floor(t * this.amount);
        const t_prime: number = (t - index / this.amount) * this.amount;

        return this.splines[index].position_on_spline(t_prime);
    }
}

export default SplineChain;
