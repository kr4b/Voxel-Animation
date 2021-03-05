import Spline from "./spline.js";
import { vec2 } from "./vec2.js";

class Sampler<T> {
  aabbMin: vec2;
  aabbMax: vec2;
  sampler: Array<boolean>;
  data:    Array<T>;
  size:    number;
  make_spline: { (t: T): Spline };

  constructor(aabbMin: vec2, aabbMax: vec2, sampler: Array<boolean>, data: Array<T>, size: number, make_spline: { (t: T): Spline }) {
    this.aabbMin = aabbMin;
    this.aabbMax = aabbMax;
    this.sampler = sampler;
    this.data    = data;
    this.size    = size;
    this.make_spline = make_spline
  }

  get(samplePos: vec2): Spline | null {
    const index: number = Math.round(samplePos.y * this.size) + Math.round(samplePos.x);
    if (this.sampler[index]) {
      return null;
    }

    return this.make_spline(this.data[index]);
  }
}

export { Sampler };