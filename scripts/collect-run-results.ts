import {
  buildResultEntry,
  carryOverResults,
  finalizeResults,
} from "./lib/run-results";

function getOption(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function hasFlag(args: string[], name: string): boolean {
  return args.includes(name);
}

function requireOption(args: string[], name: string): string {
  const value = getOption(args, name);
  if (value == null) {
    throw new Error(`Missing option: ${name}`);
  }
  return value;
}

function main() {
  const [command, ...args] = process.argv.slice(2);

  switch (command) {
    case "carry-over": {
      const carried = carryOverResults({
        prevResult: requireOption(args, "--prev-result"),
        needRerunFile: requireOption(args, "--need-rerun-file"),
        splitTestsFile: requireOption(args, "--split-tests-file"),
        env: requireOption(args, "--env"),
        outJsonl: requireOption(args, "--out-jsonl"),
      });
      console.log(`Carried over ${carried} results from previous run`);
      return;
    }
    case "finalize": {
      const count = finalizeResults({
        inJsonl: requireOption(args, "--in-jsonl"),
        outJson: requireOption(args, "--out-json"),
      });
      console.log(
        `Results written to ${requireOption(args, "--out-json")} (${count} tests)`,
      );
      return;
    }
    case "build-entry": {
      const splitValue = getOption(args, "--split");
      const entry = buildResultEntry({
        file: requireOption(args, "--file"),
        problem: requireOption(args, "--problem"),
        environment: requireOption(args, "--environment"),
        status: requireOption(args, "--status"),
        lastExecutionTime: requireOption(args, "--last-execution-time"),
        split: splitValue != null ? Number.parseInt(splitValue, 10) : undefined,
        compileError: getOption(args, "--compile-error"),
        compileErrorFile: getOption(args, "--compile-error-file"),
        casesRecords: getOption(args, "--cases-records"),
      });
      process.stdout.write(`${JSON.stringify(entry)}\n`);
      return;
    }
    default:
      throw new Error(
        `Unknown command: ${command ?? "(none)"}. Expected carry-over, finalize, or build-entry.`,
      );
  }
}

try {
  main();
} catch (error) {
  const message = error instanceof Error ? error.message : String(error);
  console.error(message);
  process.exit(1);
}
