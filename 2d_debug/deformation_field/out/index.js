import Spline from "./spline.js";
import { vec2 } from "./vec2.js";
var atan2 = Math.atan2, cos = Math.cos, PI = Math.PI, sin = Math.sin, sqrt = Math.sqrt;
var canvas;
var ctx;
var halfFieldOfView = PI / 3;
var camera_position = vec2(-3, 0);
var camera_rotation = atan2(3, -0);
var aabbMin = vec2(-0.15, -0.15);
var aabbMax = vec2(0.15, 0.15);
onload = function () {
    canvas = document.getElementById("canvas");
    ctx = canvas.getContext("2d");
    canvas.width = 400;
    canvas.height = 400;
    ctx.scale(canvas.width / 2, canvas.height / 2);
    ctx.translate(1, 1);
    ctx.lineWidth = 0.0075;
    canvas.onmousemove = function (e) {
        var x = e.pageX / canvas.width * 2 - 1;
        var y = e.pageY / canvas.height * 2 - 1;
        camera_position = vec2(x, y);
        camera_rotation = atan2(-y, -x);
        render();
    };
    render();
};
function render() {
    ctx.clearRect(-1, -1, 2, 2);
    for (var i = -halfFieldOfView; i < halfFieldOfView; i += 0.1) {
        var P1 = camera_position;
        var direction = vec2(cos(camera_rotation + i), sin(camera_rotation + i));
        var distance = sqrt(Math.pow(P1.x, 2) + Math.pow(P1.y, 2)) * 2;
        var P2 = vec2(P1.x + direction.x * distance, P1.y + direction.y * distance);
        var P0 = vec2(-5.0, 7.5);
        var P3 = vec2(0.0, 0.0);
        var s = Spline.with_tangents(P1, P2, P0, P3);
        s.draw(ctx);
        ctx.strokeStyle = "gray";
        ctx.strokeRect(-canvas.width / 2, aabbMin.x, canvas.width, aabbMax.x - aabbMin.x);
        ctx.strokeRect(aabbMin.y, -canvas.height / 2, aabbMax.y - aabbMin.y, canvas.height);
        ctx.strokeStyle = "black";
        var result = s.intersect_spline_aabb(vec2(-0.15, -0.15), vec2(0.15, 0.15));
        if (result) {
            ctx.fillStyle = "red";
            s.draw_point_at(ctx, s.ts.x);
            s.draw_point_at(ctx, s.ts.y);
            ctx.fillStyle = "black";
        }
    }
}
