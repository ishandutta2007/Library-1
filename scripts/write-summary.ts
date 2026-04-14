/**
 * GitHub Actions の Job Summary に検証結果のサマリーを出力する
 */
import { loadResults } from "./lib/results";

const data = loadResults();

if (Object.keys(data).length === 0) {
  console.log("No results found.");
  process.exit(0);
}

let passed = 0;
let failed = 0;
let skipped = 0;
let ce = 0;
const envSet = new Set<string>();

interface EnvInfo {
  status: string;
  timeMax: number;
  memMax: number;
}

interface FileRow {
  file: string;
  split?: number;
  envResults: Record<string, EnvInfo>;
}

const rowMap = new Map<string, FileRow>();

for (const problems of Object.values(data) as any[]) {
  for (const problem of problems) {
    const row: FileRow = rowMap.get(problem.file) ?? {
      file: problem.file,
      split: problem.split,
      envResults: {},
    };
    if (row.split == null && problem.split != null) {
      row.split = problem.split;
    }
    for (const [env, result] of Object.entries(problem.environments) as [
      string,
      any,
    ][]) {
      envSet.add(env);
      const status = result.status;
      const timeMax = result.summary?.time_max_ms ?? 0;
      const memMax = result.summary?.memory_max_kb ?? 0;
      row.envResults[env] = { status, timeMax, memMax };
    }
    rowMap.set(problem.file, row);
  }
}

const rows = [...rowMap.values()].sort((left, right) =>
  left.file.localeCompare(right.file),
);

for (const row of rows) {
  for (const info of Object.values(row.envResults)) {
    switch (info.status) {
      case "AC":
        passed++;
        break;
      case "CE":
        ce++;
        break;
      case "WA":
      case "TLE":
      case "MLE":
      case "RE":
        failed++;
        break;
      case "IGNORE":
        skipped++;
        break;
      default:
        skipped++;
        break;
    }
  }
}

const total = passed + failed + ce + skipped;
const envs = [...envSet].sort();

// Markdown 出力
console.log("## Verification Result\n");

if (failed === 0 && ce === 0) {
  console.log("✅ All test cases passed!\n");
} else {
  console.log("❌ Some test cases failed.\n");
}

console.log(`| | Count |`);
console.log(`|:---|---:|`);
console.log(`| ✅ Passed | ${passed} |`);
console.log(`| ❌ Failed | ${failed} |`);
console.log(`| 🔨 CE | ${ce} |`);
console.log(`| ⏭️ Skipped | ${skipped} |`);
console.log(`| **Total** | **${total}** |`);
console.log();

// 失敗したテストの詳細
const failedRows = rows.filter((r) =>
  Object.values(r.envResults).some(
    (info) => info.status !== "AC" && info.status !== "IGNORE",
  ),
);

function formatMem(kb: number): string {
  if (kb >= 1024) return `${(kb / 1024).toFixed(1)}MB`;
  return `${kb}KB`;
}

if (failedRows.length > 0) {
  console.log("### Failed / Non-AC Tests\n");
  console.log(`| File | ${envs.map((e) => `${e}`).join(" | ")} | Job |`);
  console.log(`|:---|${envs.map(() => ":---:").join("|")}|:---|`);
  for (const row of failedRows.slice(0, 50)) {
    const cells = envs.map((env) => {
      const info = row.envResults[env];
      if (!info) return "-";
      if (info.status === "AC")
        return `✅ ${info.timeMax}ms ${formatMem(info.memMax)}`;
      if (info.status === "CE") return "🔨 CE";
      if (info.status === "IGNORE") return "⏭️";
      return `❌ ${info.status} ${info.timeMax}ms ${formatMem(info.memMax)}`;
    });
    const jobCell = row.split != null ? `split-${row.split}` : "-";
    console.log(`| ${row.file} | ${cells.join(" | ")} | ${jobCell} |`);
  }
  if (failedRows.length > 50) {
    console.log(`\n... and ${failedRows.length - 50} more`);
  }
}
