import fs from "fs";

export function compareFloatOutputs(options: {
  actualPath: string;
  expectedPath: string;
  tolerance: number;
}): boolean {
  const actual = fs
    .readFileSync(options.actualPath, "utf-8")
    .split(/\s+/)
    .filter(Boolean);
  const expected = fs
    .readFileSync(options.expectedPath, "utf-8")
    .split(/\s+/)
    .filter(Boolean);

  if (actual.length !== expected.length) {
    return false;
  }

  for (let index = 0; index < actual.length; index += 1) {
    const actualValue = Number.parseFloat(actual[index]);
    const expectedValue = Number.parseFloat(expected[index]);
    if (Number.isNaN(actualValue) || Number.isNaN(expectedValue)) {
      return false;
    }
    if (Math.abs(actualValue - expectedValue) > options.tolerance) {
      return false;
    }
  }

  return true;
}
