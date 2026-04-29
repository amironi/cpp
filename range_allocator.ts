"use strict";

class Allc {
  private items: number[];
  // private items: Set<number>;
  private min: number;
  private max: number;

  constructor(min: number, max: number) {
    this.min = min;
    this.max = max;
    this.items = [] as number[];
    // new Set<number>();

    for (let i = min; i <= max; i++) {
      this.items.push(i);
    }
  }

  getFree(): number | null {
    const first = this.items[0];
    if (first === undefined) return null;

    this.items.shift();
    return first;
  }

  deAllocate(value: number): void {
    if (!this.isInRange(value)) {
      throw new Error("Not in range");
    }
    // found
    if (this.items.indexOf(value) !== -1) {
      throw new Error("Not in allocted");
    }

    this.items.push(value);
  }

  private isInRange(value: number): boolean {
    return value >= this.min && value <= this.max;
  }
}

console.log("HEY");
let a: Allc = new Allc(1, 3);

console.log(a.getFree());
console.log(a.getFree());
console.log(a.getFree());
console.log(a.deAllocate(2));
console.log(a.deAllocate(3));
console.log(a.getFree());
console.log(a.getFree());

function main() {}
