import { Plane } from "./plane.js";
import Spline from "./spline.js";
import { vec2 } from "./vec2.js";

let canvas: HTMLCanvasElement;
let ctx: CanvasRenderingContext2D;

onload = () => {
    canvas = <HTMLCanvasElement> document.getElementById("canvas");
    ctx = <CanvasRenderingContext2D> canvas.getContext("2d")

    canvas.width = 400;
    canvas.height = 400;

    ctx.scale(canvas.width / 2, canvas.height / 2);
    ctx.translate(1, 1);
    ctx.lineWidth = 0.0075;


    const spline = Spline.with_control_points(vec2(-0.2, 0.8), vec2(0.0, -0.8), vec2(-15.0, -3.0), vec2(12.0, 0.0));
    spline.draw(ctx);

    const plane = new Plane(vec2(0, 0), vec2(0.5, 0.3));
    plane.draw(ctx);

    spline.intersect_spline_plane(plane);

    ctx.strokeStyle = "aquamarine";
    spline.draw_point_at(ctx, spline.ts.x)
}