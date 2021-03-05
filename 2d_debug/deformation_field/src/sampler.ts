import { vec2 } from "./vec2.js";

class Sampler<T> {
  aabbMin: vec2;
  aabbMax: vec2;
  sampler: Array<boolean>;
  data:    Array<T>;
  size:    number;

  constructor(aabbMin: vec2, aabbMax: vec2, sampler: Array<boolean>, data: Array<T>, size: number) {
    this.aabbMin = aabbMin;
    this.aabbMax = aabbMax;
    this.sampler = sampler;
    this.data    = data;
    this.size    = size;
  }

  get(samplePos: vec2): T | null {
    const index: number = Math.round(samplePos.y * this.size) + Math.round(samplePos.x);
    if (this.sampler[index]) {
      return null;
    }

    return this.data[index];
  }
}

export { Sampler };