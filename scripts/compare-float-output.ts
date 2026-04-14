import { compareFloatOutputs } from "./lib/float-compare";

function getOption(args: string[], name: string): string | undefined {
  const index = args.indexOf(name);
  return index >= 0 ? args[index + 1] : undefined;
}

function requireOption(args: string[], name: string): string {
  const value = getOption(args, name);
  if (value == null) {
    throw new Error(`Missing option: ${name}`);
  }
  return value;
}

function main() {
  const args = process.argv.slice(2);
  const actualPath = requireOption(args, "--actual");
  const expectedPath = requireOption(args, "--expected");
  const tolerance = Number.parseFloat(requireOption(args, "--tolerance"));

  if (!Number.isFinite(tolerance)) {
    throw new Error("Tolerance must be a finite number");
  }

  const ok = compareFloatOutputs({ actualPath, expectedPath, tolerance });
  process.exit(ok ? 0 : 1);
}

try {
  main();
} catch (error) {
  const message = error instanceof Error ? error.message : String(error);
  console.error(message);
  process.exit(1);
}
