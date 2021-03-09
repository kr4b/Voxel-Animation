import { AABB } from "./aabb.js";
import { Ray } from "./ray.js";
import { Sampler } from "./sampler.js";
import Spline from "./spline.js";
import { add, multiply, scale, vec2 } from "./vec2.js";

const { atan2, cos, PI, sin, sqrt } = Math;

let canvas: HTMLCanvasElement;
let ctx: CanvasRenderingContext2D;
let sampler: Sampler<[number, number]>;
const halfFieldOfView = PI / 3;
let camera_position = vec2(-3, 0);
let camera_rotation = atan2(3, -0);

const aabbMin = vec2(-0.15, -0.15);
const aabbMax = vec2(0.15, 0.15);

onload = () => {
    canvas = <HTMLCanvasElement> document.getElementById("canvas");
    ctx = <CanvasRenderingContext2D> canvas.getContext("2d");
    canvas.width = 400;
    canvas.height = 400;

    sampler = new Sampler(
        new AABB(scale(aabbMin, 2), scale(aabbMax, 2)),
        new AABB(aabbMin, aabbMax),
        [true, true, true, true],
        [[0, 1], [1, 0], [-1, 0], [0, -1]],
        [[255, 0, 0], [0, 255, 0], [0, 0, 255], [200, 200, 0]],
        (ray: Ray, t: [number, number], c: [number, number, number]) => {
            const P1 = ray.origin;
            const P2 = add(P1, ray.dir);

            const P0 = vec2(t[0], t[1]);
            const P3 = vec2(0.0, 0.0);

            const spline = Spline.with_tangents(P1, P2, P0, P3);
            spline.set_color(c);
            return spline;
        }
    );

    ctx.scale(canvas.width / 2, canvas.height / 2);
    ctx.translate(1, 1);
    ctx.lineWidth = 0.0075;

    canvas.onmousemove = e => {
        const x = e.pageX / canvas.width * 2 - 1;
        const y = e.pageY / canvas.height * 2 - 1;

        camera_position = vec2(x, y);
        camera_rotation = atan2(-y, -x);

        render();
    }

    render();
}

function render() {
    ctx.clearRect(-1, -1, 2, 2);
    sampler.draw(ctx);
    const rays: boolean = (document.querySelector("#rays") as HTMLInputElement).checked;
    const splines: boolean = (document.querySelector("#splines") as HTMLInputElement).checked;

    for (let i = -halfFieldOfView; i < halfFieldOfView; i += 0.1) {
        const distance:  number = sqrt(camera_position.x ** 2 + camera_position.y ** 2) * 2;
        const direction: vec2   = vec2(cos(camera_rotation + i), sin(camera_rotation + i));
        const ray:       Ray    = new Ray(camera_position, scale(direction, distance));

        if (rays) {
            ctx.strokeStyle = "green";
            ray.draw(ctx);
            ctx.strokeStyle = "black";
        }

        const spline: Spline | null = ray.intersect_ray_sampler(sampler);
        const result: boolean | undefined = spline?.intersect_spline_aabb(sampler.realAABB);

        if (splines && spline != null) {
            spline.draw(ctx);
        }

        if (result) {
            ctx.fillStyle = "red";
            spline?.draw_point_at(ctx, spline.ts.x);
            spline?.draw_point_at(ctx, spline.ts.y);
            ctx.fillStyle = "black";
        }
    }

    ctx.strokeStyle = "gray";
    ctx.strokeRect(-canvas.width / 2, sampler.realAABB.min.x, canvas.width, sampler.realAABB.size().x);
    ctx.strokeRect(sampler.realAABB.min.y, -canvas.height / 2, sampler.realAABB.size().y, canvas.height);
    ctx.strokeStyle = "black";
}