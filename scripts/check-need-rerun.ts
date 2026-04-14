import { computeNeedRerun, listTargetTests } from "./lib/rerun";

function getOption(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function hasFlag(args: string[], name: string): boolean {
  return args.includes(name);
}

function getMultiOption(args: string[], name: string): string[] {
  const index = args.indexOf(name);
  if (index < 0) return [];
  const values: string[] = [];
  for (
    let cursor = index + 1;
    cursor < args.length && !args[cursor].startsWith("--");
    cursor += 1
  ) {
    values.push(args[cursor]);
  }
  return values;
}

function main() {
  const args = process.argv.slice(2);
  const prevResultPath = getOption(args, "--prev-result");
  const env = getOption(args, "--env") ?? "";
  const all = hasFlag(args, "--all");
  const explicitTests = getMultiOption(args, "--test-files");
  const testFiles = listTargetTests(all, explicitTests);
  const { rerun, skipped } = computeNeedRerun({
    prevResultPath,
    testFiles,
    env,
  });

  for (const file of rerun) {
    console.log(file);
  }
  console.error(`# Need rerun: ${rerun.length}, Skip: ${skipped}`);
}

main();
