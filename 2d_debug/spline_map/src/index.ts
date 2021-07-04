import { AABB } from "./aabb.js";
import DepressedCubic from "./depressed_cubic.js";
import { Plane } from "./plane.js";
import { Ray } from "./ray.js";
import Spline from "./spline.js";
import SplineMap from "./spline_map.js";
import { scale, vec2 } from "./vec2.js";

let canvas: HTMLCanvasElement;
let ctx: CanvasRenderingContext2D;

const EPSILON = 1e-6;

const pixels = [
    [255, 0, 0],
    [0, 255, 0],
    [0, 0, 255],
    [200, 200, 0]
];

const size = Math.sqrt(pixels.length);
const aabbMin = vec2(-0.35, -0.35);
const aabbMax = vec2(0.35, 0.35);
const canvasWidth = 400;
const canvasHeight = 400;
const pixelSizeX = 2.0 / canvasWidth;
const pixelSizeY = 2.0 / canvasHeight;

onload = () => {
    canvas = <HTMLCanvasElement> document.getElementById("canvas");
    ctx = <CanvasRenderingContext2D> canvas.getContext("2d")

    canvas.width = canvasWidth;
    canvas.height = canvasHeight;

    ctx.scale(canvas.width / 2, canvas.height / 2);
    ctx.translate(1, 1);
    ctx.lineWidth = pixelSizeX * 2.0;

    const base: Plane = new Plane(vec2(aabbMin.x + 0.5 * (aabbMax.x - aabbMin.x), aabbMin.y), scale(vec2(aabbMax.x - aabbMin.x, 0), 0.5));

    // const spline = Spline.with_tangents(aabbMin, vec2(aabbMin.x - 0.1, aabbMax.y), vec2(-2.0, 0.5), vec2(1.0, 0.0));
    const spline = Spline.with_control_points(aabbMin, vec2(aabbMin.x - 0.1, aabbMax.y), vec2(-15.0, -3.0), vec2(12.0, 0.0));

    const spline_map = new SplineMap(base, spline);

    let t = 0;
    setInterval(() => {
        ctx.clearRect(-1, -1, 2, 2);

        render_texture(ctx, spline);
        // spline_map.draw(ctx);

        const ray = new Ray(vec2(Math.sin(t) * (aabbMax.x - aabbMin.x), 0.6), vec2(0.0, -1.2));
        ray.draw(ctx);
        const result = ray.walk_spline_map(spline_map, pixelSizeY);
        if (result !== null) {
            const [texCoords, t] = result;
            const pixelX = Math.floor((Math.round(texCoords.x * 1000.0) / 1000.0 - EPSILON) * size);
            const pixelY = Math.floor((Math.round(texCoords.y * 1000.0) / 1000.0 - EPSILON) * size);
            if (pixelX >= 0 && pixelX < size && pixelY >= 0 && pixelY < size) {
                const color = pixels[pixelY * size + pixelX];
                ctx.fillStyle = `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
                ctx.fillRect(-1.0, -1.0, 0.2, 0.2);
                ray.draw_point_at(ctx, t);
            }
        }

        t += 0.035;
    }, 50);
}

function render_texture(ctx: CanvasRenderingContext2D, spline: Spline) {
    // Draw a slice for each canvas y pixel
    for (let y = aabbMin.y;; y += pixelSizeY) {
        // Linear approximation of t
        const t = (y - aabbMin.y) / (aabbMax.y - aabbMin.y);
        // Cubic calculation of t
        const cubic = new DepressedCubic(spline.a.y, spline.b.y, spline.c.y, spline.d.y - y);
        const conversion = -spline.b.y / (spline.a.y * 3.0);
        const t1 = conversion + cubic.first_root();
        const t2 = conversion + cubic.second_root();
        const t3 = conversion + cubic.third_root();
        // Determine optimal t
        const t1Diff = Math.abs(t1 - t);
        const t2Diff = Math.abs(t2 - t);
        const t3Diff = Math.abs(t3 - t);
        let realT = t;
        if (t1Diff < t2Diff && t1Diff < t3Diff && t1 >= 0.0 && t1 <= 1.0) {
            realT = t1;
        } else if (t2Diff < t3Diff && t2 >= 0.0 && t2 <= 1.0) {
            realT = t2;
        } else if (t3 >= 0.0 && t3 <= 1.0) {
            realT = t3;
        }

        if (realT >= 1.0) break;

        // Determine x offset
        const splinePos = spline.position_on_spline(realT);
        const xOffset = aabbMin.x - splinePos.x;
        const pixelY = Math.floor(realT * size);
        // Draw the slice for each pixel
        for (let pixelX = 0; pixelX < size; pixelX++) {
            const color = pixels[pixelY * size + pixelX];
            ctx.fillStyle = `rgb(${color[0]}, ${color[1]}, ${color[2]})`;
            ctx.fillRect(
                aabbMin.x - xOffset + (aabbMax.x - aabbMin.x) / size * pixelX,
                y,
                (aabbMax.x - aabbMin.x) / size,
                pixelSizeY
            );
        }
    }
}
