import { vec2 } from "./vec2.js";

const { abs, acos, cos, PI, pow, sqrt } = Math;

const sign = (n: number): number => +(0 < n) - +(n < 0);

class DepressedCubic {
    p: number;
    q: number;
    fac: number = 0;
    root: number = 0;
    arccos: number = 0;
    discriminant: number;

    constructor(a: number, b: number, c: number, d: number) {
        this.p = (3 * a * c - b * b) / (3 * a * a);
        this.q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);
        
        this.discriminant = 27 * this.q * this.q + 4 * this.p * this.p * this.p;

        this.calculate_default_root();
    }

    calculate_default_root() {
        if (this.discriminant > 0) {
            this.root = this.single_root();
        } else {
            this.fac = 2 * sqrt(-this.p / 3);
            this.arccos = acos(3 * this.q / (2 * this.p) * sqrt(-3 / this.p)) / 3;

            this.root = this.first_root();
        }
    }

    single_root(): number {
        const D = sqrt(this.q * this.q / 4 + this.p * this.p * this.p / 27);
        const C0 = -0.5 * this.q + D;
        const C1 = -0.5 * this.q - D;

        return sign(C0) * pow(abs(C0), 1 / 3) + sign(C1) * pow(abs(C1), 1 / 3);
    }

    first_root(): number {
        if (this.discriminant <= 0) this.root = this.fac * cos(this.arccos);
        return this.root;
    }

    second_root(): number {
        if (this.discriminant <= 0) this.root = this.fac * cos(this.arccos - 2 / 3 * PI);
        return this.root;
    }

    third_root(): number {
        if (this.discriminant <= 0) this.root = this.fac * cos(this.arccos - 4 / 3 * PI);
        return this.root;
    }
}

export default DepressedCubic;