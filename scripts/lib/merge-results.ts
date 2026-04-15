import fs from "fs";
import path from "path";
import {
  type CaseResult,
  type CompactResults,
  type EnvSummary,
  isCompactResults,
  readJsonFile,
} from "./results";
import { buildDependencyGraph, buildTestMap } from "./dependency-graph";
import { parseAnnotationsFromFile } from "./annotations";

export interface TestResult {
  file: string;
  problem: string;
  environment: string;
  status: string;
  compile_error?: string;
  last_execution_time?: string;
  cases: CaseResult[];
}

export interface MergeArgs {
  root: string;
  prevFile?: string | null;
}

function loadNewResults(root: string): TestResult[] {
  const resultDir = path.join(root, ".cache/results");
  const newResults: TestResult[] = [];
  if (!fs.existsSync(resultDir)) return newResults;

  for (const file of fs.readdirSync(resultDir)) {
    if (!file.startsWith("result-") || !file.endsWith(".json")) continue;
    const data = JSON.parse(
      fs.readFileSync(path.join(resultDir, file), "utf-8"),
    ) as TestResult[];
    newResults.push(...data);
    console.log(`Loaded ${data.length} results from ${file}`);
  }
  return newResults;
}

function loadPreviousEnvMap(
  prevPath: string,
): Record<string, Record<string, EnvSummary>> {
  const prevMap: Record<string, Record<string, EnvSummary>> = {};
  const raw = readJsonFile(prevPath);
  if (!raw) return prevMap;

  console.log(`Loaded previous results from ${prevPath}`);
  if (isCompactResults(raw)) {
    for (const [file, data] of Object.entries(raw.tests)) {
      prevMap[file] = data.environments || {};
    }
    return prevMap;
  }

  // レガシー grouped 形式
  for (const problems of Object.values(
    raw as Record<string, { file: string; environments: Record<string, EnvSummary> }[]>,
  )) {
    for (const problem of problems) {
      prevMap[problem.file] = {
        ...(prevMap[problem.file] || {}),
        ...problem.environments,
      };
    }
  }
  return prevMap;
}

/** 前回結果と新しい結果をマージして、テストファイル→環境別結果のマップを返す */
function mergeEnvironments(
  previous: Record<string, Record<string, EnvSummary>>,
  newResults: TestResult[],
): Record<string, Record<string, EnvSummary>> {
  const merged = { ...previous };

  for (const result of newResults) {
    const key = result.file;
    if (!merged[key]) merged[key] = {};

    const cases = result.cases || [];
    const timeMax =
      cases.length > 0 ? Math.max(...cases.map((c) => c.time_ms)) : 0;
    const timeTotal = cases.reduce((sum, c) => sum + c.time_ms, 0);
    const memMax =
      cases.length > 0 ? Math.max(...cases.map((c) => c.memory_kb)) : 0;

    const envSummary: EnvSummary = {
      status: result.status,
      summary: {
        time_max_ms: timeMax,
        time_total_ms: timeTotal,
        memory_max_kb: memMax,
      },
      last_execution_time:
        result.last_execution_time || new Date().toISOString(),
      cases,
    };
    if (result.compile_error) envSummary.compile_error = result.compile_error;
    merged[key][result.environment] = envSummary;
  }

  return merged;
}

function getTestAnnotations(root: string, testFile: string) {
  return parseAnnotationsFromFile(path.join(root, testFile));
}

const IGNORE_ENV_SUMMARY: EnvSummary = {
  status: "IGNORE",
  summary: { time_max_ms: 0, time_total_ms: 0, memory_max_kb: 0 },
  cases: [],
};

/** envMap に含まれる全環境名を収集する */
function collectEnvNames(
  envMap: Record<string, Record<string, EnvSummary>>,
): string[] {
  const envSet = new Set<string>();
  for (const envs of Object.values(envMap)) {
    for (const env of Object.keys(envs)) envSet.add(env);
  }
  return [...envSet].sort();
}

/** hpp→テストファイルのマッピングを構築し、IGNORE テストの疑似環境も envMap に注入する */
function buildHppMap(
  root: string,
  envMap: Record<string, Record<string, EnvSummary>>,
): Record<string, string[]> {
  const hppTestMap = buildTestMap(buildDependencyGraph());
  const hppMap: Record<string, string[]> = {};
  const envNames = collectEnvNames(envMap);

  for (const [hpp, testFiles] of Object.entries(hppTestMap)) {
    const files: string[] = [];
    for (const file of testFiles) {
      const ann = getTestAnnotations(root, file);
      if (ann.isIgnore && !envMap[file]) {
        // IGNORE テストは実行されないので envMap に結果がない → 疑似環境を注入
        envMap[file] = Object.fromEntries(
          envNames.map((env) => [env, IGNORE_ENV_SUMMARY]),
        );
      }
      files.push(file);
    }
    if (files.length > 0) hppMap[hpp] = files;
  }

  return hppMap;
}

export function mergeResults(args: MergeArgs): CompactResults {
  const outputPath = path.join(args.root, ".verify-results", "results.json");
  const newResults = loadNewResults(args.root);
  const prevPath = args.prevFile || outputPath;
  const prevMap = fs.existsSync(prevPath) ? loadPreviousEnvMap(prevPath) : {};
  const envMap = mergeEnvironments(prevMap, newResults);
  const hpp_map = buildHppMap(args.root, envMap);

  const tests: CompactResults["tests"] = {};
  for (const [file, environments] of Object.entries(envMap)) {
    const ann = getTestAnnotations(args.root, file);
    tests[file] = {
      problem: ann.problem,
      time_limit_ms: Math.round(ann.tleSec * 1000),
      environments,
    };
  }

  return { tests, hpp_map };
}
