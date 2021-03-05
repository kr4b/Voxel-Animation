import { Ray } from "./ray.js";
import Spline from "./spline.js";
import { vec2 } from "./vec2.js";

class Sampler<T> {
  aabbMin: vec2;
  aabbMax: vec2;
  sampler: Array<boolean>;
  data:    Array<T>;
  colors:  Array<[number, number, number]>;
  size:    number;
  make_spline: { (ray: Ray, t: T, c: [number, number, number]): Spline };

  constructor(
    aabbMin: vec2,
    aabbMax: vec2,
    sampler: Array<boolean>,
    data: Array<T>,
    colors: Array<[number, number, number]>,
    size: number,
    make_spline: { (ray: Ray, t: T, c: [number, number, number]): Spline }
  ) {
    this.aabbMin = aabbMin;
    this.aabbMax = aabbMax;
    this.sampler = sampler;
    this.data    = data;
    this.colors  = colors;
    this.size    = size;
    this.make_spline = make_spline;
  }

  get(ray: Ray, samplePos: vec2): Spline | null {
    const x: number = Math.round((this.size - 1) * samplePos.x);
    const y: number = Math.round((this.size - 1) * samplePos.y);
    const index: number = y * this.size + x;
    if (index < 0 || index >= this.data.length || !this.sampler[index]) {
      return null;
    }

    return this.make_spline(ray, this.data[index], this.colors[index]);
  }

  draw(ctx: CanvasRenderingContext2D) {
    const width:  number = (this.aabbMax.x - this.aabbMin.x) / this.size;
    const height: number = (this.aabbMax.y - this.aabbMin.y) / this.size;
    for (let i = 0; i < this.size; i++) {
      for (let j = 0; j < this.size; j++) {
        const color: [number, number, number] = this.colors[i * this.size + j];

        ctx.fillStyle = "rgb(" + color[0] + "," + color[1] + "," + color[2] + ")";
        ctx.fillRect(this.aabbMin.x + width * j, this.aabbMin.y + height * i, width, height);
        ctx.fillStyle = "black";
      }
    }
  }
}

export { Sampler };