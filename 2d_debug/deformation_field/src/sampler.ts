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
  make_spline: { (ray: Ray, t: T): Spline };

  constructor(aabbMin: vec2, aabbMax: vec2, sampler: Array<boolean>, data: Array<T>, colors: Array<[number, number, number]>, size: number, make_spline: { (ray: Ray, t: T): Spline }) {
    this.aabbMin = aabbMin;
    this.aabbMax = aabbMax;
    this.sampler = sampler;
    this.data    = data;
    this.colors  = colors;
    this.size    = size;
    this.make_spline = make_spline;
  }

  get(ray: Ray, samplePos: vec2): Spline | null {
    const index: number = this.size * (Math.round(samplePos.y * this.size) + Math.round(samplePos.x));
    if (this.sampler[index]) {
      return null;
    }

    return this.make_spline(ray, this.data[index]);
  }

  draw(ctx: CanvasRenderingContext2D) {
    const image: ImageData         = ctx.createImageData(this.size, this.size);
    const data:  Uint8ClampedArray = image.data;

    for (let i = 0; i < this.size; i++) {
      for (let j = 0; j < this.size; j++) {
        const color: [number, number, number] = this.colors[i * this.size + j];

        for (let k = 0; k < 3; k++) {
          data[(i * this.size + j) * 3 + k] = color[k];
        }
      }
    }

    ctx.putImageData(image, this.aabbMin.x, this.aabbMin.y);
  }
}

export { Sampler };