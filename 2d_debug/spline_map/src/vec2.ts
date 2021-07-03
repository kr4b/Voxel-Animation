type vec2 = { x: number, y: number };
const vec2 = (x: number, y: number): vec2 => ({ x, y });

//   _               _ 
//  |  m00  m01  m02  |
//  |  m10  m11  m12  |
//  |_ m20  m21  m22 _|
//
type mat3 = { m00: number, m01: number, m02: number, m10: number, m11: number, m12: number, m20: number, m21: number, m22: number };
const mat3 = (m00: number, m01: number, m02: number, m10: number, m11: number, m12: number, m20: number, m21: number, m22: number) =>
    ({ m00, m01, m02, m10, m11, m12, m20, m21, m22 });

const reduce    = (f: { (a: number, b: number): number }, v: vec2, w: vec2, ...u: vec2[]): vec2 => [w, ...u].reduce((acc, val) => vec2(f(val.x, acc.x), f(val.y, acc.y)), v);

const scale     = (v: vec2, n: number)  => vec2(v.x * n, v.y * n);
const add       = (v: vec2, w: vec2, ...u: vec2[]): vec2 => reduce((a, b) => a + b, w, v, ...u);
const subtract  = (v: vec2, w: vec2, ...u: vec2[]): vec2 => reduce((a, b) => a - b, w, v, ...u);
const multiply  = (v: vec2, w: vec2, ...u: vec2[]): vec2 => reduce((a, b) => a * b, w, v, ...u);
const divide    = (v: vec2, w: vec2, ...u: vec2[]): vec2 => reduce((a, b) => a / b, w, v, ...u);
const min       = (v: vec2, w: vec2, ...u: vec2[]): vec2 => reduce((a, b) => Math.min(a, b), w, v, ...u);
const max       = (v: vec2, w: vec2, ...u: vec2[]): vec2 => reduce((a, b) => Math.max(a, b), w, v, ...u);
const length    = (v: vec2, w: vec2)    => Math.sqrt((v.x - w.x)**2 + (v.y - w.y)**2);
const copy      = (v: vec2)             => vec2(v.x, v.y);
const step      = (v: vec2, w: vec2)    => vec2(+(w.x >= v.x), +(w.y >= v.y));
const mix       = (v: vec2, w: vec2, n: number) => add(scale(v, 1 - n), scale(w, n));
const norm      = (v: vec2)             => scale(v, 1.0 / length(v, vec2(0.0, 0.0)));
const dot       = (v: vec2, w: vec2)    => v.x * w.x + v.y * w.y;
const transform = (v: vec2, m: mat3)    => scale(vec2(v.x * m.m00 + v.y * m.m01 + m.m02, v.x * m.m10 + v.y * m.m11 + m.m12), 1.0 / (m.m20 + m.m21 + m.m22));

export { vec2, mat3, scale, add, subtract, multiply, divide, min, max, length, copy, step, mix, norm, dot, transform };