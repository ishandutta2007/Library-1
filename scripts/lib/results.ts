/**
 * .verify-results/results.json の読み込み
 * compact 形式 / hpp grouped 形式を用途ごとに返す
 */
import fs from "fs";
import path from "path";

const ROOT = path.resolve(__dirname, "../..");
const RESULTS_PATH = path.join(ROOT, ".verify-results", "results.json");

export interface CaseResult {
  name: string;
  status: string;
  time_ms: number;
  memory_kb: number;
  detail?: string;
}

export interface EnvSummary {
  status: string;
  compile_error?: string;
  summary: {
    time_max_ms: number;
    time_total_ms: number;
    memory_max_kb: number;
  };
  last_execution_time?: string;
  cases: CaseResult[];
}

export interface CompactTestResult {
  problem: string;
  time_limit_ms: number;
  environments: Record<string, EnvSummary>;
}

export interface CompactResults {
  tests: Record<string, CompactTestResult>;
  hpp_map: Record<string, string[]>;
}

export interface GroupedProblemResult {
  file: string;
  problem: string;
  time_limit_ms: number;
  environments: Record<string, EnvSummary>;
}

export type GroupedResultsByHpp = Record<string, GroupedProblemResult[]>;

export function readJsonFile(filePath: string): unknown {
  if (!fs.existsSync(filePath)) return null;
  try {
    return JSON.parse(fs.readFileSync(filePath, "utf-8"));
  } catch {
    return null;
  }
}

export function isCompactResults(raw: any): raw is CompactResults {
  return !!raw && typeof raw === "object" && raw.tests && raw.hpp_map;
}

export function parseCompactResults(raw: unknown): CompactResults {
  if (!raw || typeof raw !== "object") {
    return { tests: {}, hpp_map: {} };
  }

  if (isCompactResults(raw)) {
    return raw;
  }

  const grouped = raw as GroupedResultsByHpp;
  const tests: Record<string, CompactTestResult> = {};
  const hpp_map: Record<string, string[]> = {};

  for (const [hpp, problems] of Object.entries(grouped)) {
    hpp_map[hpp] = [];
    for (const problem of problems ?? []) {
      hpp_map[hpp].push(problem.file);
      const existing = tests[problem.file];
      tests[problem.file] = {
        problem: problem.problem,
        time_limit_ms: problem.time_limit_ms,
        environments: {
          ...(existing?.environments || {}),
          ...(problem.environments || {}),
        },
      };
    }
  }

  return { tests, hpp_map };
}

export function loadCompactResults(): CompactResults {
  return parseCompactResults(readJsonFile(RESULTS_PATH));
}

export function loadCompactResultsFromPath(filePath: string): CompactResults {
  return parseCompactResults(readJsonFile(filePath));
}

export function loadGroupedResultsByHpp(): GroupedResultsByHpp {
  const compact = loadCompactResults();
  const data: GroupedResultsByHpp = {};

  for (const [hpp, files] of Object.entries(compact.hpp_map)) {
    data[hpp] = files.map((file) => ({
      file,
      problem: compact.tests[file]?.problem || "",
      time_limit_ms: compact.tests[file]?.time_limit_ms || 0,
      environments: compact.tests[file]?.environments || {},
    }));
  }

  return data;
}

export const loadResults = loadGroupedResultsByHpp;
