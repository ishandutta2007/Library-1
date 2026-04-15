import fs from "fs";
import { loadCompactResultsFromPath, type CaseResult } from "./results";

export const CASE_RECORD_SEPARATOR = "\x1f";

export interface RunResultEntry {
  file: string;
  problem: string;
  environment: string;
  status: string;
  compile_error?: string;
  last_execution_time: string;
  cases: CaseResult[];
}

export function flattenPreviousResults(prevPath: string): RunResultEntry[] {
  const compact = loadCompactResultsFromPath(prevPath);
  const flat: RunResultEntry[] = [];

  for (const [file, testData] of Object.entries(compact.tests)) {
    for (const [environment, envData] of Object.entries(
      testData.environments,
    )) {
      flat.push({
        file,
        problem: testData.problem,
        environment,
        status: envData.status,
        compile_error: envData.compile_error,
        last_execution_time: envData.last_execution_time || "",
        cases: envData.cases || [],
      });
    }
  }

  return flat;
}

export function loadLinesAsSet(filePath: string): Set<string> {
  const result = new Set<string>();
  if (!filePath || !fs.existsSync(filePath)) return result;

  for (const rawLine of fs.readFileSync(filePath, "utf-8").split(/\r?\n/)) {
    const line = rawLine.trim();
    if (line && !line.startsWith("#")) {
      result.add(line);
    }
  }
  return result;
}

export function carryOverResults(options: {
  prevResult: string;
  needRerunFile: string;
  splitTestsFile: string;
  env: string;
  outJsonl: string;
}): number {
  if (!fs.existsSync(options.prevResult)) return 0;

  const needRerun = loadLinesAsSet(options.needRerunFile);
  const splitTests = loadLinesAsSet(options.splitTestsFile);
  const previous = flattenPreviousResults(options.prevResult);

  let carried = 0;
  const lines: string[] = [];
  for (const entry of previous) {
    if (!splitTests.has(entry.file)) continue;
    if (needRerun.has(entry.file)) continue;
    if (entry.environment !== options.env) continue;
    lines.push(`${JSON.stringify(entry)}\n`);
    carried += 1;
  }
  if (lines.length > 0) {
    fs.appendFileSync(options.outJsonl, lines.join(""));
  }

  return carried;
}

export function finalizeResults(options: {
  inJsonl: string;
  outJson: string;
}): number {
  const items: RunResultEntry[] = [];
  if (fs.existsSync(options.inJsonl)) {
    for (const rawLine of fs
      .readFileSync(options.inJsonl, "utf-8")
      .split(/\r?\n/)) {
      const line = rawLine.trim();
      if (!line) continue;
      items.push(JSON.parse(line) as RunResultEntry);
    }
  }

  fs.writeFileSync(options.outJson, `${JSON.stringify(items, null, 2)}\n`);
  return items.length;
}

export function loadCaseRecords(path: string | undefined): CaseResult[] {
  if (!path || !fs.existsSync(path)) return [];

  const cases: CaseResult[] = [];
  for (const rawLine of fs.readFileSync(path, "utf-8").split(/\r?\n/)) {
    if (!rawLine) continue;
    const [name, status, timeMs, memoryKb, detail] = rawLine.split(
      CASE_RECORD_SEPARATOR,
    );
    if (name == null || status == null || timeMs == null || memoryKb == null)
      continue;
    const entry: CaseResult = {
      name,
      status,
      time_ms: Number.parseInt(timeMs, 10),
      memory_kb: Number.parseInt(memoryKb, 10),
    };
    if (detail) entry.detail = detail;
    cases.push(entry);
  }
  return cases;
}

export function buildResultEntry(options: {
  file: string;
  problem: string;
  environment: string;
  status: string;
  lastExecutionTime: string;
  compileError?: string;
  compileErrorFile?: string;
  casesRecords?: string;
}): RunResultEntry {
  const compileError = options.compileErrorFile
    ? fs.readFileSync(options.compileErrorFile, "utf-8")
    : options.compileError;

  return {
    file: options.file,
    problem: options.problem,
    environment: options.environment,
    status: options.status,
    last_execution_time: options.lastExecutionTime,
    ...(compileError ? { compile_error: compileError } : {}),
    cases: loadCaseRecords(options.casesRecords),
  };
}
