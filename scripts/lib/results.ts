/**
 * .verify-results/results.json の読み込み
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

function readJsonFile(filePath: string): unknown {
  if (!fs.existsSync(filePath)) return null;
  try {
    return JSON.parse(fs.readFileSync(filePath, "utf-8"));
  } catch {
    return null;
  }
}

function loadCompactResultsFromJson(raw: unknown): CompactResults {
  if (
    raw &&
    typeof raw === "object" &&
    "tests" in raw &&
    "hpp_map" in raw
  ) {
    return raw as CompactResults;
  }
  return { tests: {}, hpp_map: {} };
}

export function loadCompactResults(): CompactResults {
  return loadCompactResultsFromJson(readJsonFile(RESULTS_PATH));
}

export function loadCompactResultsFromPath(filePath: string): CompactResults {
  return loadCompactResultsFromJson(readJsonFile(filePath));
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
