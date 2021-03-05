type vec2 = { x: number, y: number };
const vec2 = (x: number, y: number): vec2 => ({ x: x, y: y });

const scale     = (v: vec2, n: number)  => vec2(v.x * n, v.y * n);
const add       = (v: vec2, w: vec2, ...u: vec2[]): vec2 => [w, ...u].reduce((acc, val) => vec2(acc.x + val.x, acc.y + val.y), v);
const subtract  = (v: vec2, w: vec2, ...u: vec2[]): vec2 => [w, ...u].reduce((acc, val) => vec2(acc.x - val.x, acc.y - val.y), v);
const multiply  = (v: vec2, w: vec2, ...u: vec2[]): vec2 => [w, ...u].reduce((acc, val) => vec2(acc.x * val.x, acc.y * val.y), v);
const divide    = (v: vec2, w: vec2, ...u: vec2[]): vec2 => [w, ...u].reduce((acc, val) => vec2(acc.x / val.x, acc.y / val.y), v);
const length    = (v: vec2, w: vec2)    => Math.sqrt((v.x - w.x)**2 + (v.y - w.y)**2);
const copy      = (v: vec2)             => vec2(v.x, v.y);
const step      = (v: vec2, w: vec2)    => vec2(+(w.x >= v.x), +(w.y >= v.y));

export { vec2, scale, add, subtract, multiply, divide, length, copy, step };