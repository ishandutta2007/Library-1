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

export interface TestResult {
  file: string;
  problem: string;
  environment: string;
  status: string;
  compile_error?: string;
  last_execution_time?: string;
  cases: CaseResult[];
}

export interface MergedProblem {
  problem: string;
  file: string;
  time_limit_ms: number;
  environments: Record<string, EnvSummary>;
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

  for (const problems of Object.values(
    raw as Record<string, MergedProblem[]>,
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

function mergeEnvironments(
  previous: Record<string, Record<string, EnvSummary>>,
  newResults: TestResult[],
): Record<string, Record<string, EnvSummary>> {
  const prevMap = { ...previous };

  for (const result of newResults) {
    const key = result.file;
    if (!prevMap[key]) prevMap[key] = {};

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
    prevMap[key][result.environment] = envSummary;
  }

  return prevMap;
}

function getTestMeta(
  root: string,
  testFile: string,
): { problem: string; tlMs: number } {
  const full = path.join(root, testFile);
  if (!fs.existsSync(full)) return { problem: "", tlMs: 0 };
  const content = fs.readFileSync(full, "utf-8");
  const problemMatch = content.match(/competitive-verifier:\s*PROBLEM\s+(\S+)/);
  const tleMatch = content.match(/competitive-verifier:\s*TLE\s+([0-9.]+)/);
  return {
    problem: problemMatch ? problemMatch[1] : "",
    tlMs: tleMatch ? Math.round(Number.parseFloat(tleMatch[1]) * 1000) : 0,
  };
}

function getTestStatus(
  root: string,
  testFile: string,
): "normal" | "ignore" | "standalone" {
  const full = path.join(root, testFile);
  if (!fs.existsSync(full)) return "normal";
  const content = fs.readFileSync(full, "utf-8");
  if (/competitive-verifier:\s*IGNORE/.test(content)) return "ignore";
  if (/competitive-verifier:\s*STANDALONE/.test(content)) return "standalone";
  return "normal";
}

function buildGroupedOutput(
  root: string,
  prevMap: Record<string, Record<string, EnvSummary>>,
): Record<string, MergedProblem[]> {
  const hppMap = buildTestMap(buildDependencyGraph());
  const output: Record<string, MergedProblem[]> = {};

  for (const [hpp, testFiles] of Object.entries(hppMap)) {
    const problems: MergedProblem[] = [];
    for (const file of testFiles) {
      const meta = getTestMeta(root, file);
      const status = getTestStatus(root, file);
      if (status === "ignore") {
        const ignoreEnvs: Record<string, EnvSummary> = {};
        for (const env of [
          "x64-g++",
          "x64-clang++",
          "arm-g++",
          "arm-clang++",
        ]) {
          ignoreEnvs[env] = {
            status: "IGNORE",
            summary: { time_max_ms: 0, time_total_ms: 0, memory_max_kb: 0 },
            cases: [],
          };
        }
        problems.push({
          problem: meta.problem,
          file,
          time_limit_ms: meta.tlMs,
          environments: ignoreEnvs,
        });
      } else if (prevMap[file]) {
        problems.push({
          problem: meta.problem,
          file,
          time_limit_ms: meta.tlMs,
          environments: prevMap[file],
        });
      }
    }
    if (problems.length > 0) output[hpp] = problems;
  }

  return output;
}

export function mergeResults(args: MergeArgs): CompactResults {
  const outputPath = path.join(args.root, ".verify-results", "results.json");
  const newResults = loadNewResults(args.root);
  const prevPath = args.prevFile || outputPath;
  const prevMap = fs.existsSync(prevPath) ? loadPreviousEnvMap(prevPath) : {};
  const envMap = mergeEnvironments(prevMap, newResults);
  const grouped = buildGroupedOutput(args.root, envMap);

  // hpp_map: grouped から構築
  const hpp_map: Record<string, string[]> = {};
  for (const [hpp, problems] of Object.entries(grouped)) {
    hpp_map[hpp] = problems.map((p) => p.file);
  }

  // tests: envMap の全テストから構築（hpp に紐づかないテストも漏れない）
  const tests: CompactResults["tests"] = {};
  for (const [file, environments] of Object.entries(envMap)) {
    const meta = getTestMeta(args.root, file);
    tests[file] = {
      problem: meta.problem,
      time_limit_ms: meta.tlMs,
      environments,
    };
  }

  return { tests, hpp_map };
}
