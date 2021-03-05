import { vec2 } from "./vec2";

const { abs, acos, cos, PI, pow, sqrt } = Math;

const sign = (n: number): number => +(0 < n) - +(n < 0);

class DepressedCubic {
    p: number;
    q: number;
    discriminant: number;

    constructor(a: number, b: number, c: number, d: number) {
        this.p = (3 * a * c - b * b) / (3 * a * a);
        this.q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);

        this.discriminant = 27 * this.q * this.q + 4 * this.p * this.p * this.p;
    }

    find_roots(): number {
        if (this.discriminant > 0) {
            return this.single_root();
        } else {
            const fac = 2 * sqrt(-this.p / 3);
            const arccos = acos(3 * this.q / (2 * this.p) * sqrt(-3 / this.p)) / 3;

            return this.second_root(fac, arccos);
        }
    }

    single_root(): number {
        const D = sqrt(this.q * this.q / 4 + this.p * this.p * this.p / 27);
        const C0 = -0.5 * this.q + D;
        const C1 = -0.5 * this.q - D;

        return sign(C0) * pow(abs(C0), 1 / 3) + sign(C1) * pow(abs(C1), 1 / 3);
    }

    first_root(fac: number, arccos: number): number {
        return fac * cos(arccos);
    }

    second_root(fac: number, arccos: number): number {
        return fac * cos(arccos - 2 / 3 * PI);
    }

    third_root(fac: number, arccos: number): number {
        return fac * cos(arccos - 4 / 3 * PI);
    }

    static find_roots_static(a: vec2, b: vec2, c: vec2, d: vec2): vec2 {
        return vec2(
            new DepressedCubic(a.x, b.x, c.x, d.x).find_roots(),
            new DepressedCubic(a.y, b.y, c.y, d.y).find_roots());
    }
}

export default DepressedCubic;