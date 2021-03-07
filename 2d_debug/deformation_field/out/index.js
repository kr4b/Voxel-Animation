import { AABB } from "./aabb.js";
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
    sampler = new Sampler(new AABB(scale(aabbMin, 2), scale(aabbMax, 2)), new AABB(aabbMin, aabbMax), [true, true, true, true], [[0, 1], [1, 0], [-1, 0], [0, -1]], [[255, 0, 0], [0, 255, 0], [0, 0, 255], [200, 200, 0]], 2, function (ray, t, c) {
        var P1 = ray.origin;
        var P2 = add(P1, ray.dir);
        var P0 = vec2(t[0], t[1]);
        var P3 = vec2(0.0, 0.0);
        var spline = Spline.with_tangents(P1, P2, P0, P3);
        spline.set_color(c);
        return spline;
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
    sampler.draw(ctx);
    var rays = document.querySelector("#rays").checked;
    var splines = document.querySelector("#splines").checked;
    for (var i = -halfFieldOfView; i < halfFieldOfView; i += 0.1) {
        var distance = sqrt(Math.pow(camera_position.x, 2) + Math.pow(camera_position.y, 2)) * 2;
        var direction = vec2(cos(camera_rotation + i), sin(camera_rotation + i));
        var ray = new Ray(camera_position, scale(direction, distance));
        if (rays) {
            ctx.strokeStyle = "green";
            ray.draw(ctx);
            ctx.strokeStyle = "black";
        }
        var spline = ray.intersect_ray_sampler(sampler);
        var result = spline === null || spline === void 0 ? void 0 : spline.intersect_spline_aabb(sampler.realAABB);
        if (splines && spline != null) {
            spline.draw(ctx);
        }
        if (result) {
            ctx.fillStyle = "red";
            spline === null || spline === void 0 ? void 0 : spline.draw_point_at(ctx, spline.ts.x);
            spline === null || spline === void 0 ? void 0 : spline.draw_point_at(ctx, spline.ts.y);
            ctx.fillStyle = "black";
        }
    }
    ctx.strokeStyle = "gray";
    ctx.strokeRect(-canvas.width / 2, sampler.realAABB.min.x, canvas.width, sampler.realAABB.size().x);
    ctx.strokeRect(sampler.realAABB.min.y, -canvas.height / 2, sampler.realAABB.size().y, canvas.height);
    ctx.strokeStyle = "black";
}
