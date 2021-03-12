import { AABB } from "./aabb.js";
import { Ray } from "./ray.js";
import { Sampler } from "./sampler.js";
import Spline from "./spline.js";
import { add, divide, max, min, scale, subtract, vec2 } from "./vec2.js";
var atan2 = Math.atan2, cos = Math.cos, PI = Math.PI, sin = Math.sin, sqrt = Math.sqrt;
var original_1d;
var transformed_1d;
var canvas;
var ctx;
var sampler;
var halfFieldOfView = PI / 3;
var camera_position = vec2(-3, 0);
var camera_rotation = atan2(3, -0);
var aabbColors = [
    [255, 0, 0],
    [0, 255, 0],
    [0, 0, 255],
    [200, 200, 0]
];
var aabbMin = vec2(-0.15, -0.15);
var aabbMax = vec2(0.15, 0.15);
onload = function () {
    canvas = document.getElementById("canvas");
    ctx = canvas.getContext("2d");
    canvas.width = 400;
    canvas.height = 400;
    original_1d = document.getElementById("1d_og").getContext("2d");
    original_1d.canvas.width = 400;
    original_1d.canvas.height = 40;
    original_1d.scale(1, original_1d.canvas.height / 2);
    original_1d.translate(0, 1);
    transformed_1d = document.getElementById("1d_tf").getContext("2d");
    transformed_1d.canvas.width = 400;
    transformed_1d.canvas.height = 40;
    transformed_1d.scale(1, transformed_1d.canvas.height / 2);
    transformed_1d.translate(0, 1);
    sampler = new Sampler(new AABB(scale(aabbMin, 2), scale(aabbMax, 2)), new AABB(aabbMin, aabbMax), [true, true, true, true], [[0, 1], [1, 0], [-1, 0], [0, -1]], [[255, 0, 0], [0, 255, 0], [0, 0, 255], [200, 200, 0]], function (ray, t, c) {
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
        var x = (e.pageX - canvas.offsetLeft) / canvas.width * 2 - 1;
        var y = (e.pageY - canvas.offsetTop) / canvas.height * 2 - 1;
        camera_position = vec2(x, y);
        camera_rotation = atan2(-y, -x);
        render();
    };
    render();
};
function get_interpolated_color(alpha) {
    var y_colors = [
        aabbColors[0].map(function (v, i) { return Math.floor(aabbColors[2][i] * alpha.y + aabbColors[0][i] * (1 - alpha.y)); }),
        aabbColors[1].map(function (v, i) { return Math.floor(aabbColors[3][i] * alpha.y + aabbColors[1][i] * (1 - alpha.y)); })
    ];
    var color = y_colors[0].map(function (v, i) { return Math.floor(y_colors[1][i] * alpha.x + y_colors[0][i] * (1 - alpha.x)); });
    return "rgb(" + color[0] + ", " + color[1] + ", " + color[2] + ")";
}
function render() {
    original_1d.clearRect(0, -1, original_1d.canvas.width, 2);
    transformed_1d.clearRect(0, -1, original_1d.canvas.width, 2);
    ctx.clearRect(-1, -1, 2, 2);
    sampler.draw(ctx);
    var rays = document.querySelector("#rays").checked;
    var splines = document.querySelector("#splines").checked;
    var increment = 0.1;
    var pixel_width = 400 / (halfFieldOfView * 2 / increment);
    for (var i = -halfFieldOfView; i < halfFieldOfView; i += increment) {
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
        var ts = ray.intersect_ray_aabb(new AABB(aabbMin, aabbMax));
        if (ts.x <= ts.y && ts.y >= 0.0) {
            // Draw the view with the rays
            var worldEntry = add(ray.origin, scale(ray.dir, ts.x));
            var vscale = subtract(aabbMax, aabbMin);
            var ventry = min(vec2(1, 1), max(vec2(0, 0), divide(subtract(worldEntry, aabbMin), vscale)));
            original_1d.fillStyle = get_interpolated_color(ventry);
            original_1d.fillRect((i + halfFieldOfView) * pixel_width / increment, -1, pixel_width, 2);
        }
        if (splines && spline != null) {
            spline.draw(ctx);
        }
        if (result) {
            ctx.fillStyle = "red";
            spline === null || spline === void 0 ? void 0 : spline.draw_point_at(ctx, spline.ts.x);
            spline === null || spline === void 0 ? void 0 : spline.draw_point_at(ctx, spline.ts.y);
            ctx.fillStyle = "black";
            var worldEntry = spline.position_on_spline(ts.x);
            var vscale = subtract(aabbMax, aabbMin);
            var ventry = min(vec2(1, 1), max(vec2(0, 0), divide(subtract(worldEntry, aabbMin), vscale)));
            transformed_1d.fillStyle = get_interpolated_color(ventry);
            transformed_1d.fillRect((i + halfFieldOfView) * pixel_width / increment, -1, pixel_width, 2);
        }
    }
    ctx.strokeStyle = "gray";
    ctx.strokeRect(-canvas.width / 2, sampler.realAABB.min.x, canvas.width, sampler.realAABB.size().x);
    ctx.strokeRect(sampler.realAABB.min.y, -canvas.height / 2, sampler.realAABB.size().y, canvas.height);
    ctx.strokeStyle = "black";
}
