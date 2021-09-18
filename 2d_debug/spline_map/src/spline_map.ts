import { AABB } from "./aabb.js";
import { Plane } from "./plane.js";
import Spline from "./spline.js";
import { add, dist, dot, length, mat3, max, min, subtract, transform, vec2 } from "./vec2.js";

class SplineMap {
    aabb: AABB;
    base: Plane;
    bottom_base: Plane;
    spline: Spline;
    furthest_spline: Spline;

    // Not the encompassing AABB
    width: number;
    height: number;

    constructor(base: Plane, spline: Spline) {
        this.base = base;
        this.spline = Spline.transform(Spline.transform(spline, this.base.matrix), mat3(1, 0, -this.base.half_size.x, 0, 1, -this.base.half_size.y, 0, 0, 1));
        this.furthest_spline = Spline.transform(this.spline, mat3(1, 0, this.base.size.x, 0, 1, this.base.size.y, 0, 0, 1));

        // Create the encompassing AABB
        const extremes = Array.from(this.spline.get_extremes().filter(t => t >= 0 && t <= 1)).map(t => this.spline.position_on_spline(t));
        this.aabb = new AABB(
            min(...extremes),
            add(this.base.size, max(...extremes)),
        );

        this.width = length(this.base.size);
        this.height = dist(this.spline.position_on_spline(1), this.spline.position_on_spline(0));
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
        Spline.transform(this.spline, mat3(1, 0, this.base.size.x, 0, 1, this.base.size.y, 0, 0, 1)).draw(ctx);

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

    // TODO: U

    /// Calculate texture coords for a given position in world space
    texture_coords(pos: vec2): vec2 | null {
        // Create base plane at the given position
        // TODO: use infinite plane instead of scaling
        const plane = new Plane(pos, this.base.size);
        // Get plane spline intersection
        // TODO: This check can probably be skipped when using an infinite plane

        const [t1, t2, t3] = this.spline.intersect_spline_plane(plane);
        const t = t1 < 0.0 || t1 > 1.0 ? (t2 < 0.0 || t2 > 1.0 ? (t3 < 0.0 || t3 > 1.0 ? -1: t3) : t2) : t1;
        if (t < 0.0 || t > 1.0) {
            return null;
        }
        // Determine whether the point is in the plane
        const edgePos1 = this.spline.position_on_spline(t);
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

export default SplineMap;
