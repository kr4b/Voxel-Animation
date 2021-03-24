var __spreadArray = (this && this.__spreadArray) || function (to, from) {
    for (var i = 0, il = from.length, j = to.length; i < il; i++, j++)
        to[j] = from[i];
    return to;
};
var vec2 = function (x, y) { return ({ x: x, y: y }); };
var reduce = function (f, v, w) {
    var u = [];
    for (var _i = 3; _i < arguments.length; _i++) {
        u[_i - 3] = arguments[_i];
    }
    return __spreadArray([w], u).reduce(function (acc, val) { return vec2(f(val.x, acc.x), f(val.y, acc.y)); }, v);
};
var scale = function (v, n) { return vec2(v.x * n, v.y * n); };
var add = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return reduce.apply(void 0, __spreadArray([function (a, b) { return a + b; }, w, v], u));
};
var subtract = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return reduce.apply(void 0, __spreadArray([function (a, b) { return a - b; }, w, v], u));
};
var multiply = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return reduce.apply(void 0, __spreadArray([function (a, b) { return a * b; }, w, v], u));
};
var divide = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return reduce.apply(void 0, __spreadArray([function (a, b) { return a / b; }, w, v], u));
};
var min = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return reduce.apply(void 0, __spreadArray([function (a, b) { return Math.min(a, b); }, w, v], u));
};
var max = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return reduce.apply(void 0, __spreadArray([function (a, b) { return Math.max(a, b); }, w, v], u));
};
var length = function (v, w) { return Math.sqrt(Math.pow((v.x - w.x), 2) + Math.pow((v.y - w.y), 2)); };
var copy = function (v) { return vec2(v.x, v.y); };
var step = function (v, w) { return vec2(+(w.x >= v.x), +(w.y >= v.y)); };
var mix = function (v, w, n) { return add(scale(v, 1 - n), scale(w, n)); };
var norm = function (v) { return scale(v, 1.0 / length(v, vec2(0.0, 0.0))); };
export { vec2, scale, add, subtract, multiply, divide, min, max, length, copy, step, mix, norm };
