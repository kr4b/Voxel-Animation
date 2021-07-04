import { AABB } from "./aabb.js";
import SplineMap from "./spline_map.js";
import { add, divide, max, min, mix, scale, subtract, vec2 } from "./vec2.js";

const VOLUME_STEPS: number = 1024;

class Ray {
    origin: vec2;
    dir: vec2;

    constructor(origin: vec2, dir: vec2) {
        this.origin = origin;
        this.dir = dir;
    }

    draw_point_at(ctx: CanvasRenderingContext2D, t: number): void {
        const point: vec2 = add(this.origin, scale(this.dir, t));

        ctx.beginPath();
        ctx.arc(
            Math.max(-ctx.canvas.width / 2, Math.min(ctx.canvas.width / 2, point.x)),
            Math.max(-ctx.canvas.height / 2, Math.min(ctx.canvas.height / 2, point.y)),
            0.02, 0.0, 2.0 * Math.PI);
        ctx.fill();
    }

    draw(ctx: CanvasRenderingContext2D): void {
        const end: vec2 = add(this.origin, this.dir);

        ctx.globalAlpha = 0.2;
        ctx.beginPath();
        ctx.moveTo(this.origin.x, this.origin.y);
        ctx.lineTo(end.x, end.y);
        ctx.stroke();
        ctx.globalAlpha = 1;
    }

    /// Intersection of this ray with the given AABB
    intersect_ray_aabb(aabb: AABB): vec2 {
        const t1: vec2 = divide(subtract(aabb.min, this.origin), this.dir);
        const t2: vec2 = divide(subtract(aabb.max, this.origin), this.dir);

        const mins: vec2 = min(t1, t2);
        const maxs: vec2 = max(t1, t2);

        const near: number = Math.max(mins.x, mins.y);
        const far: number = Math.min(maxs.x, maxs.y);
        return vec2(near, far);
    }

    /// Walks the ray through the given spline map
    walk_spline_map(spline_map: SplineMap, step: number): [vec2, number] | null {
        const { x: t1, y: t2 } = spline_map.intersect_ray(this);

        for (let t = t1; t <= t2; t += step) {
            const pos = add(this.origin, scale(this.dir, t));
            const textureCoords = spline_map.texture_coords(pos);
            if (textureCoords === null) continue;
            return [textureCoords, t];
        }

        return null;
    }
}

export { Ray };
