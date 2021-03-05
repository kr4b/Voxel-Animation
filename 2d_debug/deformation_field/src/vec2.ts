type vec2 = { x: number, y: number };
const vec2 = (x: number, y: number): vec2 => ({ x: x, y: y });

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

export { vec2, scale, add, subtract, multiply, divide, min, max, length, copy, step, mix };