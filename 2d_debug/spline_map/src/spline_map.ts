import { AABB } from "./aabb.js";
import DepressedCubic from "./depressed_cubic.js";
import { Plane } from "./plane.js";
import { Ray } from "./ray.js";
import SplineChain from "./spline_chain.js";
import { add, dist, divide, dot, length, mat3, max, min, multiply, scale, subtract, transform, vec2 } from "./vec2.js";

class SplineChainMap {
    aabb: AABB;
    base: Plane;
    bottom_base: Plane;
    spline: SplineChain;
    furthest_spline: SplineChain;

    // Not the encompassing AABB
    width: number;
    height: number;

    constructor(base: Plane, spline: SplineChain) {
        this.base = base;
        this.spline = SplineChain.transform(SplineChain.transform(spline, this.base.matrix), mat3(1, 0, -this.base.half_size.x, 0, 1, -this.base.half_size.y, 0, 0, 1));
        this.furthest_spline = SplineChain.transform(this.spline, mat3(1, 0, this.base.size.x, 0, 1, this.base.size.y, 0, 0, 1));

        // Create the encompassing AABB
        const extremes = Array.from(this.spline.get_extremes().filter(t => t >= 0 && t <= 1)).map(t => this.spline.position_on_chain(t));
        this.aabb = new AABB(
            min(...extremes),
            add(this.base.size, max(...extremes)),
        );

        this.width = length(this.base.size);
        this.height = dist(this.spline.position_on_chain(1), this.spline.position_on_chain(0));
        this.bottom_base = Plane.transform(this.base, mat3(1, 0, 0, 0, 1, this.height, 0, 0, 1));
    }

    draw(ctx: CanvasRenderingContext2D) {
        ctx.strokeStyle = "slategray"
        this.base.draw(ctx);

        ctx.strokeStyle = "aquamarine"
        this.spline.draw(ctx);

        ctx.strokeStyle = "salmon";
        this.aabb.draw(ctx);

        ctx.globalAlpha = 0.4;
        ctx.strokeStyle = "aquamarine"
        SplineChain.transform(this.spline, mat3(1, 0, this.base.size.x, 0, 1, this.base.size.y, 0, 0, 1)).draw(ctx);

        ctx.strokeStyle = "slategray"
        Plane.transform(this.base, mat3(1, 0, this.base.normal.x * this.height, 0, 1, this.base.normal.y * this.height, 0, 0, 1)).draw(ctx);
        ctx.globalAlpha = 1.0;

        // ctx.strokeStyle = "salmon";
        // this.aabb.draw(ctx);
        // ctx.strokeStyle = "olive";
        // this.spline.draw(ctx);
        // ctx.save();
        // ctx.translate(this.base.size.x, 0.0);
        // this.spline.draw(ctx);
        // ctx.restore();

        // ctx.beginPath();
        // ctx.moveTo(this.base.center.x - this.base.half_size.x, this.base.center.y - this.base.half_size.y);
        // ctx.lineTo(this.base.center.x + this.base.half_size.x, this.base.center.y - this.base.half_size.y);
        // ctx.moveTo(this.base.center.x - this.base.half_size.x, this.base.center.y + this.base.size.x);
        // ctx.lineTo(this.base.center.x + this.base.half_size.x, this.base.center.y + this.base.size.x);
        // ctx.stroke();
    }

    // TODO: Use more efficient way of getting the intersection points
    intersect_ray_alt(ray: Ray): vec2 {
        const plane = new Plane(ray.origin, ray.dir);

        const transformed0 = SplineChain.transform(this.spline, plane.inv_matrix);
        const transformed1 = SplineChain.transform(this.furthest_spline, plane.inv_matrix);

        // Should calculate the correct index like in the shader, but there is no y_bounds...
        const t0 = ray.intersect_ray_spline(transformed0.splines[0], this.spline.splines[0]);
        const t1 = ray.intersect_ray_spline(transformed1.splines[0], this.furthest_spline.splines[0]);

        const tb0 = ray.intersect_ray_plane(this.base);
        const tb1 = ray.intersect_ray_plane(this.bottom_base);

        const results = [ ...t0, ...t1, tb0, tb1 ];

        const tnear = Math.min(...results);
        const tfar = Math.min(...results.map(t => t === tnear ? 20 : t));

        const ts = vec2(
            tnear,
            tfar
        );

        if (tnear > 10 || tfar > 10) {
            return vec2(1.0, 0.0);
        }

        return ts;
    }

    /// Calculate texture coords for a given position in world space
    texture_coords(pos: vec2): vec2 | null {
        // Create base plane at the given position
        // TODO: use infinite plane instead of scaling
        const plane = new Plane(pos, this.base.size);
        // Get plane spline intersection
        // TODO: This check can probably be skipped when using an infinite plane
        if (!this.spline.intersect_spline_plane(plane)) {
            return null;
        }
        const t = this.spline.ts.x;
        // Determine whether the point is in the plane
        const edgePos1 = this.spline.position_on_chain(t);
        const diff1 = subtract(edgePos1, pos);
        const edgePos2 = add(edgePos1, this.base.size);
        const diff2 = subtract(edgePos2, pos);
        const sizeSquared = dot(this.base.size, this.base.size);
        if (dot(diff1, diff1) > sizeSquared || dot(diff2, diff2) > sizeSquared) {
            return null;
        }

        // Calculate and return texture coords
        // x component is distance to base spline, scaled by base length
        // y component is currently asumed to be t
        const xComp = length(subtract(pos, edgePos1)) / length(this.base.size);
        return vec2(xComp, t);
    }
}

export default SplineChainMap;
