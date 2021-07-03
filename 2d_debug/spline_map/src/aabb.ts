import { subtract, min, max, vec2 } from "./vec2.js";

class AABB {
  min: vec2;
  max: vec2;

  constructor(aabbMin: vec2, aabbMax: vec2) {
    this.min = min(aabbMin, aabbMax);
    this.max = max(aabbMin, aabbMax);
  }

  size(): vec2 {
    return subtract(this.max, this.min);
  }

  draw(ctx: CanvasRenderingContext2D) {
    ctx.strokeRect(
        this.min.x,
        this.min.y,
        this.max.x - this.min.x,
        this.max.y - this.min.y
    );
  }
}

export { AABB };