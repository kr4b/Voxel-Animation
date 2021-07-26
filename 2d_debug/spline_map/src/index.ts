import { Plane } from "./plane.js";
import { Ray } from "./ray.js";
import Spline from "./spline.js";
import SplineChain from "./spline_chain.js";
import SplineMap from "./spline_map.js";
import { norm, scale, vec2 } from "./vec2.js";

let canvas: HTMLCanvasElement;
let ctx: CanvasRenderingContext2D;
let transformed_1d: CanvasRenderingContext2D;

let camera_position = vec2(-0.8, 0);
let camera_rotation = Math.atan2(0, 0.8);
const halfFieldOfView = Math.PI / 3;

const pixels = [
    [0, 0, 0],
    [200, 200, 0],
    [0, 255, 0],
    [0, 0, 255],

    [0, 0, 255],
    [255, 0, 0],
    [200, 200, 0],
    [0, 0, 0],

    [0, 0, 0],
    [0, 0, 255],
    [255, 0, 0],
    [200, 200, 0],

    [200, 200, 0],
    [0, 0, 0],
    [0, 0, 255],
    [255, 0, 0],
];

const size = Math.sqrt(pixels.length);
const aabbMin = vec2(-0.25, -0.25);
const aabbMax = vec2(0.25, 0.25);
const canvasWidth = 400;
const canvasHeight = 400;
const pixelSizeX = 2.0 / canvasWidth;
const pixelSizeY = 2.0 / canvasHeight;

onload = () => {
    canvas = <HTMLCanvasElement>document.getElementById("canvas");
    ctx = <CanvasRenderingContext2D>canvas.getContext("2d")

    canvas.width = canvasWidth;
    canvas.height = canvasHeight;

    canvas.onmousemove = e => {
        const x = (e.pageX - canvas.offsetLeft) / canvas.width * 2 - 1;
        const y = (e.pageY - canvas.offsetTop) / canvas.height * 2 - 1;

        camera_position = vec2(x, y);
        camera_rotation = Math.atan2(-y, -x);
    }

    ctx.scale(canvas.width / 2, canvas.height / 2);
    ctx.translate(1, 1);
    ctx.lineWidth = pixelSizeX * 2.0;

    transformed_1d = <CanvasRenderingContext2D>(<HTMLCanvasElement>document.getElementById("1d_tf")).getContext("2d");
    transformed_1d.canvas.width = 400;
    transformed_1d.canvas.height = 40;
    transformed_1d.scale(1, transformed_1d.canvas.height / 2);
    transformed_1d.translate(0, 1);

    const base: Plane = new Plane(vec2(aabbMin.x + 0.5 * (aabbMax.x - aabbMin.x), aabbMin.y), scale(vec2(aabbMax.x - aabbMin.x, 0), 0.5));
    // const base: Plane = new Plane(vec2(0.1, -0.2), scale(norm(vec2(0.5, 0.3)), aabbMax.x - aabbMin.x));

    // const spline = Spline.with_control_points(aabbMin, vec2(aabbMin.x - 0.1, aabbMax.y), vec2(-15.0, -3.0), vec2(12.0, 0.0));
    // const spline = Spline.with_control_points(vec2(0, 0), vec2(0, 1), vec2(-15.0, -3.0), vec2(20.0, -10.0));
    const spline_chain = SplineChain.from_points_with_outer_tangents(
        vec2(1, 0.3),
        vec2(0, 0),
        [
            vec2(0, 0),
            vec2(0.0, 0.3),
            vec2(0.0, 0.5),
        ], 1.0);

    const spline_map = new SplineMap(base, spline_chain);

    let t = 0;
    setInterval(() => {
        ctx.clearRect(-1, -1, 2, 2);
        transformed_1d.clearRect(0, -1, transformed_1d.canvas.width, 2);

        render_texture(ctx, spline_map);
        spline_map.draw(ctx);

        const increment = 0.05;
        const pixel_width = 400 / (halfFieldOfView * 2 / increment);
        for (let i = -halfFieldOfView; i < halfFieldOfView; i += increment) {
            const distance: number = Math.sqrt(camera_position.x ** 2 + camera_position.y ** 2) * 2;
            const direction: vec2 = vec2(Math.cos(camera_rotation + i), Math.sin(camera_rotation + i));
            const ray: Ray = new Ray(camera_position, scale(direction, distance));
            // const ray = new Ray(vec2(Math.sin(t) * spline_map.aabb.size().x * 0.5, 0.6), vec2(0.0, -1.2));

            ray.draw(ctx);

            const result = ray.walk_spline_map(spline_map, pixelSizeY, pixels, size);
            if (result !== null) {
                const [texCoords, t] = result;
                const color = pixels[texCoords.y * size + texCoords.x];
                ctx.fillStyle = `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
                ray.draw_point_at(ctx, t);

                transformed_1d.fillStyle = `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
                transformed_1d.fillRect((i + halfFieldOfView) * pixel_width / increment, -1, pixel_width, 2);
            }
        }

        t += 0.035;
    }, 50);
}

function render_texture(ctx: CanvasRenderingContext2D, spline_map: SplineMap) {
    const spline = spline_map.spline;
    const _pixelSizeX = spline_map.width / size;
    const direction = norm(spline_map.base.half_size);

    ctx.save();
    ctx.lineWidth = spline_map.height / 50;

    for (let y = 0; y < 100; y++) {
        const t = y / 100;

        // Determine x offset
        const splinePos = spline.position_on_chain(t);
        const pixelY = Math.floor(t * size);

        // Draw the slice for each pixel
        for (let pixelX = 0; pixelX < size; pixelX++) {
            const color = pixels[pixelY * size + pixelX];
            if (color[0] === 0 && color[1] === 0 && color[2] === 0) continue;
            ctx.strokeStyle = `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
            ctx.beginPath();
            ctx.moveTo(
                splinePos.x + _pixelSizeX * pixelX * direction.x,
                splinePos.y + _pixelSizeX * pixelX * direction.y
            );
            ctx.lineTo(
                splinePos.x + _pixelSizeX * (pixelX + 1) * direction.x,
                splinePos.y + _pixelSizeX * (pixelX + 1) * direction.y
            );
            ctx.stroke();
        }
    }
    ctx.restore();
}
