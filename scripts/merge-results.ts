/**
 * テスト結果をマージして results.json を生成する
 *
 * 使い方:
 *   npx tsx scripts/merge-results.ts                          # .cache/results/ からマージ
 *   npx tsx scripts/merge-results.ts --prev .verify-results/results.json  # 前回結果とマージ
 *   npx tsx scripts/merge-results.ts --from-committed         # .verify-results/results.json をそのまま利用
 */
import fs from "fs";
import path from "path";
import { mergeResults } from "./lib/merge-results";

const ROOT = path.resolve(__dirname, "..");
const OUTPUT = path.join(ROOT, ".verify-results", "results.json");

const args = process.argv.slice(2);
const fromCommitted = args.includes("--from-committed");
const prevIndex = args.indexOf("--prev");
const prevFile = prevIndex >= 0 ? args[prevIndex + 1] : null;

if (fromCommitted) {
  if (fs.existsSync(OUTPUT)) {
    console.log(`Results available at ${OUTPUT}`);
  } else {
    console.log("No committed results found");
  }
  process.exit(0);
}

const compact = mergeResults({ root: ROOT, prevFile });

fs.mkdirSync(path.dirname(OUTPUT), { recursive: true });
fs.writeFileSync(OUTPUT, `${JSON.stringify(compact, null, 2)}\n`);

const testCount = Object.keys(compact.tests).length;
const hppCount = Object.keys(compact.hpp_map).length;
console.log(`\nResults: ${testCount} test files, ${hppCount} hpp files`);
console.log(`Written to ${OUTPUT}`);
