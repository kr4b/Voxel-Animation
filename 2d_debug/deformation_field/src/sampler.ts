import { Ray } from "./ray.js";
import Spline from "./spline.js";
import { vec2 } from "./vec2.js";
import { AABB } from "./aabb.js"

class Sampler<T> {
  samplerAABB: AABB;
  realAABB: AABB;
  sampler: Array<boolean>;
  data:    Array<T>;
  colors:  Array<[number, number, number]>;
  size:    number;
  make_spline: { (ray: Ray, t: T, c: [number, number, number]): Spline };

  constructor(
    samplerAABB: AABB,
    realAABB: AABB,
    sampler: Array<boolean>,
    data: Array<T>,
    colors: Array<[number, number, number]>,
    size: number,
    make_spline: { (ray: Ray, t: T, c: [number, number, number]): Spline }
  ) {
    this.samplerAABB = samplerAABB;
    this.realAABB = realAABB;
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
    const size: vec2 = this.samplerAABB.size();
    const width:  number = size.x / this.size;
    const height: number = size.y / this.size;

    for (let i = 0; i < this.size; i++) {
      for (let j = 0; j < this.size; j++) {
        const color: [number, number, number] = this.colors[i * this.size + j];

        ctx.fillStyle = "rgb(" + color[0] + "," + color[1] + "," + color[2] + ")";
        ctx.fillRect(this.samplerAABB.min.x + width * j, this.samplerAABB.min.y + height * i, width, height);
        ctx.fillStyle = "black";
      }
    }
  }
}

export { Sampler };