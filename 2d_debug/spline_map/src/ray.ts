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

    find_ray_spline_intersection(spline: Spline, t: number, ret: vec2) {
        if (t >= 0.0 && t <= 1.0) {
            const pos = spline.position_on_spline(t);
            const _t = (pos.x - this.origin.x) / this.dir.x;
            if (_t >= 0.0) {
                ret.x = Math.min(ret.x, _t);
                ret.y = Math.max(ret.y, _t);
            }
        }
    }

    intersect_ray_spline_map(splineMap: SplineMap, ts: vec2): boolean {
        ts.x = 10.0, ts.y = -10.0;

        const plane1 = new Plane(this.origin, this.dir);
        let its1, its2, its3;

        [its1, its2, its3] = splineMap.spline.intersect_spline_plane(plane1);
        this.find_ray_spline_intersection(splineMap.spline, its1, ts);
        this.find_ray_spline_intersection(splineMap.spline, its2, ts);
        this.find_ray_spline_intersection(splineMap.spline, its3, ts);

        [its1, its2, its3] = splineMap.furthest_spline.intersect_spline_plane(plane1);
        this.find_ray_spline_intersection(splineMap.furthest_spline, its1, ts);
        this.find_ray_spline_intersection(splineMap.furthest_spline, its2, ts);
        this.find_ray_spline_intersection(splineMap.furthest_spline, its3, ts);

        {
            let { empty, value } = this.intersect_ray_plane(splineMap.base);
            if (!empty && typeof value === "number" && value > 0.0) {
                ts.x = Math.min(ts.x, value);
                ts.y = Math.max(ts.y, value);
            }
        }

        {
            let { empty, value } = this.intersect_ray_plane(splineMap.bottom_base);
            if (!empty && typeof value === "number" && value > 0.0) {
                ts.x = Math.min(ts.x, value);
                ts.y = Math.max(ts.y, value);
            }
        }

        return ts.x < ts.y;
    }

    /// Walks the ray through the given spline map
    walk_spline_map(spline_map: SplineMap, step: number, pixels: number[][], size: number, ctx: CanvasRenderingContext2D): [vec2, number] | null {
        let ts: vec2 = vec2(0.0, 0.0);
        const result = this.intersect_ray_spline_map(spline_map, ts);

        if (!result) return null;

        for (let t = Math.max(0.0, ts.x); t <= ts.y; t += step) {
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
        if (t >= 0.0 && t <= 1.0)
            result[0] = (original_spline.position_on_spline(t).x - this.origin.x) / this.dir.x;

        t = conversion.y + cubic.second_root();
        if (t >= 0.0 && t <= 1.0)
            result[1] = (original_spline.position_on_spline(t).x - this.origin.x) / this.dir.x;

        t = conversion.y + cubic.third_root();
        if (t >= 0.0 && t <= 1.0)
            result[2] = (original_spline.position_on_spline(t).x - this.origin.x) / this.dir.x;

        return result;
    }

    intersect_ray_plane(plane: Plane): { empty: boolean, value?: number } {
        const point1 = plane.min;
        const point2 = plane.max;
        const dpoint = subtract(point1, point2);

        const dxy = this.dir.y * dpoint.x - this.dir.x * dpoint.y;
        if (dxy == 0.0) return { empty: true };

        let result = (this.dir.y * (point1.x - this.origin.x) - this.dir.x * (point1.y - this.origin.y)) / dxy;
        if (result < 0.0 || result > 1.0) return { empty: true };

        const intersection = add(scale(point1, 1.0 - result), scale(point2, result));
        return {
            empty: false,
            value: (intersection.x - this.origin.x) / this.dir.x,
        };
    }
}

export { Ray };

