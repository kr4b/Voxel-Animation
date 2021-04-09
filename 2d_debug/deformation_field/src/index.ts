import { AABB } from "./aabb.js";
import { PlaneSampler } from "./plane_sampler.js";
import { Ray } from "./ray.js";
import Spline from "./spline.js";
import { add, divide, max, min, scale, subtract, vec2 } from "./vec2.js";

const { atan2, cos, PI, sin, sqrt } = Math;

let original_1d: CanvasRenderingContext2D;
let transformed_1d: CanvasRenderingContext2D;

let canvas: HTMLCanvasElement;
let ctx: CanvasRenderingContext2D;
let sampler: PlaneSampler;
const halfFieldOfView = PI / 3;
let camera_position = vec2(-3, 0);
let camera_rotation = atan2(3, -0);

const aabbColors = [
    [255, 0, 0],
    [0, 255, 0],
    [0, 0, 255],
    [200, 200, 0]
];

const aabbMin = vec2(-0.15, -0.15);
const aabbMax = vec2(0.15, 0.15);
const realAABB = new AABB(aabbMin, aabbMax);

function map(value: number, min1: number, max1: number, min2: number, max2: number) {
    return min2 + (value - min1) / (max1 - min1) * (max2 - min2);
}

onload = () => {
    canvas = <HTMLCanvasElement>document.getElementById("canvas");
    ctx = <CanvasRenderingContext2D>canvas.getContext("2d");
    canvas.width = 400;
    canvas.height = 400;

    original_1d = <CanvasRenderingContext2D>(<HTMLCanvasElement>document.getElementById("1d_og")).getContext("2d");
    original_1d.canvas.width = 400;
    original_1d.canvas.height = 40;
    original_1d.scale(1, original_1d.canvas.height / 2);
    original_1d.translate(0, 1);

    transformed_1d = <CanvasRenderingContext2D>(<HTMLCanvasElement>document.getElementById("1d_tf")).getContext("2d");
    transformed_1d.canvas.width = 400;
    transformed_1d.canvas.height = 40;
    transformed_1d.scale(1, transformed_1d.canvas.height / 2);
    transformed_1d.translate(0, 1);

    const size = 1;
    sampler = new PlaneSampler(
        vec2(-0.3, -0.3), vec2(0.3, 0.3),
        new AABB(vec2(-0.3, -0.3), vec2(0.3, 0.3)),
        [   // Order of textures is in plane_sampler.ts
            [
                vec2(size, -size),
                vec2(size, size),
            ],
            [
                vec2(-size, -size),
                vec2(-size, size),
            ],
            [
                vec2(-size, size),
                vec2(size, size),
            ],
            [
                vec2(-size, -size),
                vec2(size, -size),
            ],
        ],
        (ray: Ray, plane: number, data: vec2, color: [number, number, number]) => {
            const P1 = ray.origin;
            const P2 = add(P1, ray.dir);

            const P0 = vec2(0.0, 0.0);
            const P3 = vec2(data.x, data.y);

            const spline = Spline.with_tangents(P1, P2, P0, P3);
            spline.set_color(color);
            return spline;
        });

    ctx.scale(canvas.width / 2, canvas.height / 2);
    ctx.translate(1, 1);
    ctx.lineWidth = 0.0075;

    canvas.onmousemove = e => {
        const x = (e.pageX - canvas.offsetLeft) / canvas.width * 2 - 1;
        const y = (e.pageY - canvas.offsetTop) / canvas.height * 2 - 1;

        camera_position = vec2(x, y);
        camera_rotation = atan2(-y, -x);

        render();
    }

    render();
}

onkeyup = e => {
    if (e.key == "r") {
        (<HTMLInputElement>document.getElementById("rays")).checked = !(<HTMLInputElement>document.getElementById("rays")).checked;
    }
    if (e.key == "s") {
        (<HTMLInputElement>document.getElementById("splines")).checked = !(<HTMLInputElement>document.getElementById("splines")).checked;
    }
    render();
}

function get_interpolated_color(alpha: vec2) {
    const y_colors = [
        aabbColors[0].map((v, i) => Math.floor(aabbColors[2][i] * alpha.y + aabbColors[0][i] * (1 - alpha.y))),
        aabbColors[1].map((v, i) => Math.floor(aabbColors[3][i] * alpha.y + aabbColors[1][i] * (1 - alpha.y)))
    ];
    const color = y_colors[0].map((v, i) => Math.floor(y_colors[1][i] * alpha.x + y_colors[0][i] * (1 - alpha.x)));

    return `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
}

function draw_aabb_colors() {
    const vscale: vec2 = subtract(aabbMax, aabbMin);
    ctx.fillStyle = `rgb(${aabbColors[0][0]}, ${aabbColors[0][1]}, ${aabbColors[0][2]})`;
    ctx.fillRect(aabbMin.x, aabbMin.y, vscale.x / 2, vscale.y / 2);

    ctx.fillStyle = `rgb(${aabbColors[1][0]}, ${aabbColors[1][1]}, ${aabbColors[1][2]})`;
    ctx.fillRect(aabbMin.x + vscale.x / 2, aabbMin.y, vscale.x / 2, vscale.y / 2);

    ctx.fillStyle = `rgb(${aabbColors[2][0]}, ${aabbColors[2][1]}, ${aabbColors[2][2]})`;
    ctx.fillRect(aabbMin.x, aabbMin.y + vscale.y / 2, vscale.x / 2, vscale.y / 2);

    ctx.fillStyle = `rgb(${aabbColors[3][0]}, ${aabbColors[3][1]}, ${aabbColors[3][2]})`;
    ctx.fillRect(aabbMin.x + vscale.x / 2, aabbMin.y + vscale.y / 2, vscale.x / 2, vscale.y / 2);
}

function render() {
    original_1d.clearRect(0, -1, original_1d.canvas.width, 2);
    transformed_1d.clearRect(0, -1, original_1d.canvas.width, 2);

    ctx.clearRect(-1, -1, 2, 2);
    draw_aabb_colors();
    sampler.draw(ctx);
    const rays: boolean = (document.querySelector("#rays") as HTMLInputElement).checked;
    const splines: boolean = (document.querySelector("#splines") as HTMLInputElement).checked;

    const increment = 0.05;
    const pixel_width = 400 / (halfFieldOfView * 2 / increment);
    for (let i = -halfFieldOfView; i < halfFieldOfView; i += increment) {
        const distance: number = sqrt(camera_position.x ** 2 + camera_position.y ** 2) * 2;
        const direction: vec2 = vec2(cos(camera_rotation + i), sin(camera_rotation + i));
        const ray: Ray = new Ray(camera_position, scale(direction, distance));

        if (rays) {
            ctx.strokeStyle = "green";
            ray.draw(ctx);
            ctx.strokeStyle = "black";
        }

        const spline: Spline | null = sampler.intersect(ray);
        const result: boolean | undefined = spline?.intersect_spline_aabb(realAABB);

        const ts: vec2 = ray.intersect_ray_aabb(new AABB(aabbMin, aabbMax));
        if (ts.x <= ts.y && ts.y >= 0.0) {
            // Draw the view with the rays
            const worldEntry: vec2 = add(ray.origin, scale(ray.dir, ts.x));
            const vscale: vec2 = subtract(aabbMax, aabbMin);
            const ventry: vec2 = min(vec2(1, 1), max(vec2(0, 0), divide(subtract(worldEntry, aabbMin), vscale)));

            original_1d.fillStyle = get_interpolated_color(ventry);
            original_1d.fillRect((i + halfFieldOfView) * pixel_width / increment, -1, pixel_width, 2);
        }

        if (splines && spline != null) {
            spline.draw(ctx);
        }

        if (spline != null && result) {
            ctx.fillStyle = "red";
            spline.draw_point_at(ctx, spline.ts.x);
            spline.draw_point_at(ctx, spline.ts.y);
            ctx.fillStyle = "black";

            const worldEntry: vec2 = spline.position_on_spline(spline.ts.x);
            const vscale: vec2 = subtract(aabbMax, aabbMin);
            const ventry: vec2 = min(vec2(1, 1), max(vec2(0, 0), divide(subtract(worldEntry, aabbMin), vscale)));

            transformed_1d.fillStyle = get_interpolated_color(ventry);
            transformed_1d.fillRect((i + halfFieldOfView) * pixel_width / increment, -1, pixel_width, 2);
        }
    }

    ctx.strokeStyle = "gray";
    ctx.strokeRect(-canvas.width / 2, realAABB.min.x, canvas.width, realAABB.size().x);
    ctx.strokeRect(realAABB.min.y, -canvas.height / 2, realAABB.size().y, canvas.height);
    ctx.strokeStyle = "black";
}