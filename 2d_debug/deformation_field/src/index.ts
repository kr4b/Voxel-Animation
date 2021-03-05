import Spline from "./spline.js";
import { vec2 } from "./vec2.js";

const { atan2, cos, PI, sin, sqrt } = Math;

let canvas: HTMLCanvasElement;
let ctx: CanvasRenderingContext2D;
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

    for (let i = -halfFieldOfView; i < halfFieldOfView; i += 0.1) {
        const P1 = camera_position;
        const direction = vec2(cos(camera_rotation + i), sin(camera_rotation + i));
        const distance = sqrt(P1.x ** 2 + P1.y ** 2) * 2;
        const P2 = vec2(P1.x + direction.x * distance, P1.y + direction.y * distance);

        const P0 = vec2(-5.0, 7.5);
        const P3 = vec2(0.0, 0.0);

        const s = Spline.with_tangents(P1, P2, P0, P3);

        s.draw(ctx);
        ctx.strokeStyle = "gray";
        ctx.strokeRect(-canvas.width / 2, aabbMin.x, canvas.width, aabbMax.x - aabbMin.x);
        ctx.strokeRect(aabbMin.y, -canvas.height / 2, aabbMax.y - aabbMin.y, canvas.height);
        ctx.strokeStyle = "black";

        const result = s.intersect_spline_aabb(vec2(-0.15, -0.15), vec2(0.15, 0.15));
        if (result) {
            ctx.fillStyle = "red";
            s.draw_point_at(ctx, s.ts.x);
            s.draw_point_at(ctx, s.ts.y);
            ctx.fillStyle = "black";
        }
    }
}