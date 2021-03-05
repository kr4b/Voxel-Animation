import { Ray } from "./ray.js";
import { Sampler } from "./sampler.js";
import Spline from "./spline.js";
import { add, scale, vec2 } from "./vec2.js";
var atan2 = Math.atan2, cos = Math.cos, PI = Math.PI, sin = Math.sin, sqrt = Math.sqrt;
var canvas;
var ctx;
var sampler;
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
    sampler = new Sampler(aabbMin, aabbMax, [true, true, true, true], [[0, 10], [10, 0], [-10, 0], [0, -10]], [[255, 0, 0], [0, 255, 0], [0, 0, 255], [255, 255, 0]], 2, function (ray, t) {
        var P1 = ray.origin;
        var distance = sqrt(Math.pow(P1.x, 2) + Math.pow(P1.y, 2)) * 2;
        var P2 = add(P1, scale(ray.dir, distance));
        var P0 = vec2(t[0], t[1]);
        var P3 = vec2(0.0, 0.0);
        return Spline.with_tangents(P1, P2, P0, P3);
    });
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
        var direction = vec2(cos(camera_rotation + i), sin(camera_rotation + i));
        var ray = new Ray(camera_position, direction);
        sampler.draw(ctx);
        ctx.strokeStyle = "gray";
        ctx.strokeRect(-canvas.width / 2, aabbMin.x, canvas.width, aabbMax.x - aabbMin.x);
        ctx.strokeRect(aabbMin.y, -canvas.height / 2, aabbMax.y - aabbMin.y, canvas.height);
        ctx.strokeStyle = "black";
        var spline = ray.intersect_ray_sampler(sampler);
        var result = spline === null || spline === void 0 ? void 0 : spline.intersect_spline_aabb(sampler.aabbMin, sampler.aabbMax);
        if (result && spline != null) {
            spline.draw(ctx);
            ctx.fillStyle = "red";
            spline.draw_point_at(ctx, spline.ts.x);
            spline.draw_point_at(ctx, spline.ts.y);
            ctx.fillStyle = "black";
        }
    }
}
