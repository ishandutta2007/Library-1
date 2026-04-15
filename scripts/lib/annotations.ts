/**
 * テストファイルの competitive-verifier アノテーションをパースする
 *
 * 対応アノテーション:
 *   competitive-verifier: PROBLEM <url>
 *   competitive-verifier: TLE <seconds>
 *   competitive-verifier: MLE <megabytes>
 *   competitive-verifier: ERROR <tolerance>
 *   competitive-verifier: IGNORE
 *   competitive-verifier: STANDALONE
 */
import fs from "fs";

export interface TestAnnotations {
  problem: string;
  tleSec: number;
  mleMb: number;
  errorTolerance: number;
  isIgnore: boolean;
  isStandalone: boolean;
}

const DEFAULTS: TestAnnotations = {
  problem: "",
  tleSec: 10,
  mleMb: 256,
  errorTolerance: 0,
  isIgnore: false,
  isStandalone: false,
};

/** ファイル内容の文字列からアノテーションをパースする */
export function parseAnnotations(content: string): TestAnnotations {
  const result = { ...DEFAULTS };
  // 先頭 20 行だけ走査（run-tests.sh と同じ）
  const lines = content.split("\n", 20);
  for (const line of lines) {
    const m = line.match(/competitive-verifier:\s*(\S+)(?:\s+(.+))?/);
    if (!m) continue;
    const [, key, value] = m;
    switch (key) {
      case "PROBLEM":
        if (value) result.problem = value.trim();
        break;
      case "TLE":
        if (value) result.tleSec = Number.parseFloat(value);
        break;
      case "MLE":
        if (value) result.mleMb = Number.parseInt(value, 10);
        break;
      case "ERROR":
        if (value) result.errorTolerance = Number.parseFloat(value);
        break;
      case "IGNORE":
        result.isIgnore = true;
        break;
      case "STANDALONE":
        result.isStandalone = true;
        break;
    }
  }
  return result;
}

/** ファイルパスからアノテーションをパースする */
export function parseAnnotationsFromFile(filePath: string): TestAnnotations {
  if (!fs.existsSync(filePath)) return { ...DEFAULTS };
  return parseAnnotations(fs.readFileSync(filePath, "utf-8"));
}
