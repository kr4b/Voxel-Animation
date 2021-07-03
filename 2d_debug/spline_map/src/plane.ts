import { Ray } from "./ray.js";
import { add, divide, dot, norm, scale, subtract, vec2 } from "./vec2.js";

class Plane {
    center: vec2;
    half_size: vec2;
    size: vec2;
    normal: vec2;

    min: vec2;
    max: vec2;

    /**
     * 
     * @param center The center point of the plane
     * @param half_size Half the size of the plane
     */
    constructor(center: vec2, half_size: vec2) {
        this.center = center;
        this.half_size = half_size;
        this.size = scale(half_size, 2.0);
        this.normal = norm(vec2(-this.half_size.y, this.half_size.x));

        this.min = subtract(this.center, this.half_size);
        this.max = add(this.center, this.half_size);
    }

    draw(ctx: CanvasRenderingContext2D): void {
        ctx.save();
        ctx.lineWidth = 0.025;
        ctx.lineCap = "square";
        ctx.beginPath();
        ctx.moveTo(this.center.x - this.half_size.x, this.center.y - this.half_size.y);
        ctx.lineTo(this.center.x + this.half_size.x, this.center.y + this.half_size.y);
        ctx.stroke();
        ctx.restore();
    }

    intersect(ray: Ray): [number, number] {
        const discriminant = dot(this.normal, ray.dir);
        if (Math.abs(discriminant) < 1e-6) {
            return [-1, -1];
        }

        const t = dot(subtract(this.center, ray.origin), this.normal) / discriminant;
        if (t < 0.0) {
            return [-1, -1];
        }

        const worldPos = add(ray.origin, scale(ray.dir, t));
        const samplePos = scale(add(divide(subtract(worldPos, this.center), scale(this.half_size, 2)), vec2(1.0, 1.0)), 0.5);
        let uv;
        if (this.half_size.x == 0.0) {
            uv = samplePos.y;
        } else {
            uv = samplePos.x;
        }

        if (uv < 0.0 || uv > 1.0) {
            return [-1, -1];
        }

        return [uv, t];
    }
}

export { Plane };
