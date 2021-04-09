import { AABB } from "./aabb.js";
import { Plane } from "./plane.js";
import { Ray } from "./ray.js";
import Spline from "./spline.js";
import { add, length, mix, multiply, norm, scale, subtract, vec2 } from "./vec2.js";

type Texture = Array<vec2>;
type Squaremap = [Texture, Texture, Texture, Texture];
type SplineBuilder = (ray: Ray, plane: number, data: vec2, color: [number, number, number]) => Spline | null;

const SQUARE_MAP_POSITIVE_X = 0;
const SQUARE_MAP_NEGATIVE_X = 1;
const SQUARE_MAP_POSITIVE_Y = 2;
const SQUARE_MAP_NEGATIVE_Y = 3;

const PLANE_COLORS: Array<[number, number, number]> = [
    [52, 152, 219],
    [155, 89, 182],
    [46, 204, 113],
    [241, 196, 15]
];

class PlaneSampler {

    private textures: Squaremap;
    private builder: SplineBuilder;
    private planes: Array<Plane> = new Array(4);

    constructor(min: vec2, max: vec2, original: AABB, textures: Squaremap, builder: SplineBuilder) {
        this.textures = textures;
        this.builder = builder;

        if (this.textures.some(t => t.length != textures[0].length))
            throw "Textures should all be of the same size";

        this.init_planes(min, max, original);
    }

    private init_planes(min: vec2, max: vec2, original: AABB) {
        const size = scale(vec2(max.x - min.x, max.y - min.y), 0.5);

        for (let face = 0; face < this.planes.length; face++) {
            switch (face) {
                case SQUARE_MAP_POSITIVE_X:
                    this.planes[face] = new Plane(
                        vec2(original.max.x, min.y + size.y),
                        vec2(0, size.y));
                    break;
                case SQUARE_MAP_NEGATIVE_X:
                    this.planes[face] = new Plane(
                        vec2(original.min.x, min.y + size.y),
                        vec2(0, size.y));
                    break;
                case SQUARE_MAP_POSITIVE_Y:
                    this.planes[face] = new Plane(
                        vec2(min.x + size.x, original.max.y),
                        vec2(size.x, 0));
                    break;
                case SQUARE_MAP_NEGATIVE_Y:
                    this.planes[face] = new Plane(
                        vec2(min.x + size.x, original.min.y),
                        vec2(size.x, 0));
                    break;
                default:
                    console.warn("???");
            }
        }
    }

    intersect(ray: Ray): Spline | null {
        let final: [number, vec2] = [-1, vec2(0, 0)];

        this.planes
            .map((plane, i): [Plane, number] => [plane, i])
            .sort((a, b) => length(a[0].center, ray.origin) - length(b[0].center, ray.origin))
            .forEach(([plane, i]) => {
                const result: number = plane.intersect(ray);

                if (result != -1 && final[0] < 0) {
                    const tex_len = this.textures[i].length;
                    const tex_space_result = result * (tex_len - 1);

                    const major = Math.floor(tex_space_result);
                    const minor = tex_space_result - major;

                    final = [i, mix(this.textures[i][major], this.textures[i][major + 1], minor)];
                }
            });

        if (final[0] >= 0) {
            return this.builder(ray, final[0], final[1], PLANE_COLORS[final[0]]);
        }

        return null;
    }

    // get(ray: Ray, samplePos: vec2): Spline | null {
    //     const x: number = Math.round((this.size - 1) * samplePos.x);
    //     const y: number = Math.round((this.size - 1) * samplePos.y);
    //     const index: number = y * this.size + x;
    //     if (index < 0 || index >= this.data.length || !this.sampler[index]) {
    //         return null;
    //     }

    //     return this.make_spline(ray, this.data[index], this.colors[index]);
    // }

    draw(ctx: CanvasRenderingContext2D) {
        const width = 2.0;
        ctx.lineWidth *= width;

        this.planes.forEach((plane, i) => {
            const normal = norm(plane.normal);

            const min = subtract(plane.center, multiply(plane.size, vec2(normal.y, normal.x)));
            const max = add(plane.center, multiply(plane.size, vec2(normal.y, normal.x)));

            ctx.strokeStyle = `rgb(${PLANE_COLORS[i][0]}, ${PLANE_COLORS[i][1]}, ${PLANE_COLORS[i][2]})`;
            ctx.beginPath();
            ctx.moveTo(min.x, min.y);
            ctx.lineTo(max.x, max.y);
            ctx.stroke();
        });

        ctx.lineWidth /= width;
    }
}

export { PlaneSampler };
