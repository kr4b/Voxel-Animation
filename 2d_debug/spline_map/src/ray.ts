import { AABB } from "./aabb.js";
import DepressedCubic from "./depressed_cubic.js";
import { Plane } from "./plane.js";
import Spline from "./spline.js";
import SplineMap from "./spline_map.js";
import { add, divide, max, min, scale, subtract, vec2 } from "./vec2.js";

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
    walk_spline_map(spline_map: SplineMap, step: number, pixels: number[][], size: number, ctx: CanvasRenderingContext2D): [vec2, number] | null {
        const { x: t1, y: t2 } = spline_map.intersect_ray_alt(this);

        for (let t = t1; t <= t2; t += step) {
            const pos = add(this.origin, scale(this.dir, t));
            const texCoords = spline_map.texture_coords(pos);
            if (texCoords === null) continue;
            const pixelX = Math.floor((Math.round(texCoords.x * 1000.0) / 1001.0) * size);
            const pixelY = Math.floor((Math.round(texCoords.y * 1000.0) / 1001.0) * size);

            if (pixelX >= 0 && pixelX < size && pixelY >= 0 && pixelY < size) {
                const color = pixels[pixelY * size + pixelX];
                if (color[0] === 0 && color[1] === 0 && color[2] === 0) continue;
            } else {
                continue;
            }
            return [vec2(pixelX, pixelY), t];
        }

        return null;
    }

    intersect_ray_spline(transformed_spline: Spline, original_spline: Spline): [number, number, number] {
        const EPSILON = 1e-3
        const conversion: vec2 = divide(scale(transformed_spline.b, -1), scale(add(transformed_spline.a, vec2(1e-6, 1e-6)), 3));
        const cubic = new DepressedCubic(
            transformed_spline.a.y,
            transformed_spline.b.y,
            transformed_spline.c.y,
            transformed_spline.d.y
        );

        let result: [number, number, number] = [20.0, 20.0, 20.0];
        let t = conversion.y + cubic.first_root();
        if (t >= EPSILON && t <= 1.0 + EPSILON)
            result[0] = (original_spline.position_on_spline(t).x - this.origin.x) / this.dir.x;

        t = conversion.y + cubic.second_root();
        if (t >= EPSILON && t <= 1.0 + EPSILON)
            result[1] = (original_spline.position_on_spline(t).x - this.origin.x) / this.dir.x;

        t = conversion.y + cubic.third_root();
        if (t >= EPSILON && t <= 1.0 + EPSILON)
            result[2] = (original_spline.position_on_spline(t).x - this.origin.x) / this.dir.x;

        return result;
    }

    // TODO: Replace with better algorithm at some point
    intersect_ray_plane(plane: Plane): number {
        const aabb = new AABB(vec2(plane.min.x, plane.min.y - 1e-3), vec2(plane.max.x, plane.max.y + 1e-3));
        const intermediate = this.intersect_ray_aabb(aabb);
        const result = intermediate.x + (intermediate.y - intermediate.x) * 0.5;
        const pos = add(this.origin, scale(this.dir, result));
        if (pos.x < aabb.min.x || pos.x > aabb.max.x ||
            pos.y < aabb.min.y || pos.y > aabb.max.y) {
            return 20;
        }

        return result;
    }
}

export { Ray };

