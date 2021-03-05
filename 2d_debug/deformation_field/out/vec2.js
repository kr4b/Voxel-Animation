var __spreadArray = (this && this.__spreadArray) || function (to, from) {
    for (var i = 0, il = from.length, j = to.length; i < il; i++, j++)
        to[j] = from[i];
    return to;
};
var vec2 = function (x, y) { return ({ x: x, y: y }); };
var scale = function (v, n) { return vec2(v.x * n, v.y * n); };
var add = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return __spreadArray([w], u).reduce(function (acc, val) { return vec2(acc.x + val.x, acc.y + val.y); }, v);
};
var subtract = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return __spreadArray([w], u).reduce(function (acc, val) { return vec2(acc.x - val.x, acc.y - val.y); }, v);
};
var multiply = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return __spreadArray([w], u).reduce(function (acc, val) { return vec2(acc.x * val.x, acc.y * val.y); }, v);
};
var divide = function (v, w) {
    var u = [];
    for (var _i = 2; _i < arguments.length; _i++) {
        u[_i - 2] = arguments[_i];
    }
    return __spreadArray([w], u).reduce(function (acc, val) { return vec2(acc.x / val.x, acc.y / val.y); }, v);
};
var length = function (v, w) { return Math.sqrt(Math.pow((v.x - w.x), 2) + Math.pow((v.y - w.y), 2)); };
var copy = function (v) { return vec2(v.x, v.y); };
var step = function (v, w) { return vec2(+(w.x >= v.x), +(w.y >= v.y)); };
export { vec2, scale, add, subtract, multiply, divide, length, copy, step };
