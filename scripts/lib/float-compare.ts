import fs from "fs";

function normalizeNewlines(text: string): string {
  return text.replace(/\r\n/g, "\n").replace(/\r/g, "\n");
}

function trimTrailingNewlines(text: string): string {
  return text.replace(/\n+$/u, "");
}

function splitLinesForComparison(text: string): string[] {
  return trimTrailingNewlines(text).split("\n");
}

function isStrictFloatToken(token: string): boolean {
  return /^[+-]?(?:Infinity|(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)$/u.test(
    token,
  );
}

function tryParseFloat(token: string): number | null {
  if (!isStrictFloatToken(token)) {
    return null;
  }
  return Number(token);
}

function isClose(actual: number, expected: number, tolerance: number): boolean {
  if (Object.is(actual, expected)) {
    return true;
  }

  const difference = Math.abs(actual - expected);
  const scale = Math.max(Math.abs(actual), Math.abs(expected));
  return difference <= Math.max(tolerance, tolerance * scale);
}

export function compareFloatText(options: {
  actual: string;
  expected: string;
  tolerance: number;
}): boolean {
  const actual = normalizeNewlines(options.actual);
  const expected = normalizeNewlines(options.expected);

  if (actual === expected) {
    return true;
  }

  const actualLines = splitLinesForComparison(actual);
  const expectedLines = splitLinesForComparison(expected);
  if (actualLines.length !== expectedLines.length) {
    return false;
  }

  for (let lineIndex = 0; lineIndex < actualLines.length; lineIndex += 1) {
    const actualWords = actualLines[lineIndex].split(/\s+/u).filter(Boolean);
    const expectedWords = expectedLines[lineIndex]
      .split(/\s+/u)
      .filter(Boolean);

    if (actualWords.length !== expectedWords.length) {
      return false;
    }

    for (let wordIndex = 0; wordIndex < actualWords.length; wordIndex += 1) {
      const actualWord = actualWords[wordIndex];
      const expectedWord = expectedWords[wordIndex];

      if (actualWord === expectedWord) {
        continue;
      }

      const actualValue = tryParseFloat(actualWord);
      const expectedValue = tryParseFloat(expectedWord);
      if (actualValue == null || expectedValue == null) {
        return false;
      }
      if (!isClose(actualValue, expectedValue, options.tolerance)) {
        return false;
      }
    }
  }

  return true;
}

export function compareFloatOutputs(options: {
  actualPath: string;
  expectedPath: string;
  tolerance: number;
}): boolean {
  return compareFloatText({
    actual: fs.readFileSync(options.actualPath, "utf-8"),
    expected: fs.readFileSync(options.expectedPath, "utf-8"),
    tolerance: options.tolerance,
  });
}
