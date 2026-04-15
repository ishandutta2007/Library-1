import fs from "fs";
import path from "path";
import { spawnSync } from "child_process";
import { loadCompactResultsFromPath } from "./results";

const ROOT = path.resolve(__dirname, "../..");
const TEST_DIR = path.join(ROOT, "test");

interface PreviousResultInfo {
  file: string;
  status: string;
  last_execution_time: string;
}

function readFileIfExists(filePath: string): string {
  return fs.existsSync(filePath) ? fs.readFileSync(filePath, "utf-8") : "";
}

export function getGitTimestamp(files: string[]): Date | null {
  if (files.length === 0) return null;
  const result = spawnSync(
    "git",
    ["log", "-1", "--date=iso-strict", "--pretty=%ad", "--", ...files],
    { cwd: ROOT, encoding: "utf-8" },
  );
  if (result.status !== 0 || !result.stdout.trim()) return null;
  const parsed = new Date(result.stdout.trim());
  return Number.isNaN(parsed.getTime()) ? null : parsed;
}

export function getDependencies(testFile: string): string[] {
  const content = readFileIfExists(path.join(ROOT, testFile));
  return [...content.matchAll(/#include\s+"(mylib\/[^\"]+\.hpp)"/g)].map(
    (match) => match[1],
  );
}

export function getHppTransitiveDeps(
  hpp: string,
  depCache: Map<string, Set<string>>,
): Set<string> {
  const cached = depCache.get(hpp);
  if (cached) return cached;

  const result = new Set<string>();
  depCache.set(hpp, result);
  const content = readFileIfExists(path.join(ROOT, hpp));
  for (const match of content.matchAll(/#include\s+"(mylib\/[^\"]+\.hpp)"/g)) {
    const dep = match[1];
    result.add(dep);
    for (const transitive of getHppTransitiveDeps(dep, depCache)) {
      result.add(transitive);
    }
  }
  return result;
}

export function loadPreviousResults(
  prevResultPath: string,
  env: string,
): Record<string, PreviousResultInfo> {
  const results = loadCompactResultsFromPath(prevResultPath);
  const previous: Record<string, PreviousResultInfo> = {};

  for (const [file, testData] of Object.entries(results.tests)) {
    const envResult = testData.environments[env];
    if (!envResult) continue;
    previous[file] = {
      file,
      status: envResult.status,
      last_execution_time: envResult.last_execution_time || "",
    };
  }

  return previous;
}

export function listTargetTests(
  all: boolean,
  explicitTests: string[],
): string[] {
  if (!all) return explicitTests;

  const result: string[] = [];
  const stack = [TEST_DIR];
  while (stack.length > 0) {
    const dir = stack.pop()!;
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name);
      if (entry.isDirectory()) {
        stack.push(full);
      } else if (entry.name.endsWith(".test.cpp")) {
        const content = fs.readFileSync(full, "utf-8");
        if (/competitive-verifier:\s*IGNORE/.test(content)) continue;
        result.push(path.relative(ROOT, full));
      }
    }
  }
  return result.sort();
}

export function computeNeedRerun(options: {
  prevResultPath?: string;
  testFiles: string[];
  env: string;
}): { rerun: string[]; skipped: number } {
  const previous = options.prevResultPath
    ? loadPreviousResults(options.prevResultPath, options.env)
    : {};
  const depCache = new Map<string, Set<string>>();
  const rerun: string[] = [];
  let skipped = 0;

  for (const testFile of options.testFiles) {
    let needRerun = true;
    const prev = previous[testFile];
    if (prev && prev.status === "AC" && prev.last_execution_time) {
      const prevTime = new Date(prev.last_execution_time);
      if (!Number.isNaN(prevTime.getTime())) {
        const directDeps = getDependencies(testFile);
        const allDeps = new Set<string>(directDeps);
        for (const hpp of directDeps) {
          for (const dep of getHppTransitiveDeps(hpp, depCache)) {
            allDeps.add(dep);
          }
        }
        const latest = getGitTimestamp([testFile, ...allDeps]);
        if (latest && latest <= prevTime) {
          needRerun = false;
        }
      }
    }

    if (needRerun) rerun.push(testFile);
    else skipped += 1;
  }

  return { rerun, skipped };
}
