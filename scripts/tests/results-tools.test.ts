import test from "node:test";
import assert from "node:assert/strict";
import fs from "fs";
import os from "os";
import path from "path";
import {
  buildResultEntry,
  carryOverResults,
  CASE_RECORD_SEPARATOR,
  finalizeResults,
} from "../lib/run-results";
import { parseCompactResults } from "../lib/results";

function makeTempDir(): string {
  return fs.mkdtempSync(path.join(os.tmpdir(), "library-scripts-"));
}

test("parseCompactResults normalizes grouped legacy results", () => {
  const grouped = {
    "mylib/foo.hpp": [
      {
        file: "test/foo.test.cpp",
        problem: "https://example.com/foo",
        time_limit_ms: 2000,
        split: 3,
        environments: {
          "x64-g++": {
            status: "AC",
            summary: { time_max_ms: 10, time_total_ms: 10, memory_max_kb: 20 },
            cases: [],
          },
        },
      },
    ],
    "mylib/bar.hpp": [
      {
        file: "test/foo.test.cpp",
        problem: "https://example.com/foo",
        time_limit_ms: 2000,
        environments: {
          "x64-clang++": {
            status: "WA",
            summary: { time_max_ms: 11, time_total_ms: 11, memory_max_kb: 21 },
            cases: [],
          },
        },
      },
    ],
  };

  const compact = parseCompactResults(grouped);
  assert.deepEqual(compact.hpp_map["mylib/foo.hpp"], ["test/foo.test.cpp"]);
  assert.equal(compact.tests["test/foo.test.cpp"].split, 3);
  assert.deepEqual(
    Object.keys(compact.tests["test/foo.test.cpp"].environments).sort(),
    ["x64-clang++", "x64-g++"],
  );
});

test("buildResultEntry parses case record file", () => {
  const dir = makeTempDir();
  const casesFile = path.join(dir, "cases.txt");
  fs.writeFileSync(
    casesFile,
    [
      ["case1", "AC", "12", "34", ""].join(CASE_RECORD_SEPARATOR),
      ["case2", "WA", "56", "78", "detail text"].join(CASE_RECORD_SEPARATOR),
    ].join("\n") + "\n",
  );

  const entry = buildResultEntry({
    file: "test/foo.test.cpp",
    problem: "https://example.com/foo",
    environment: "x64-g++",
    status: "WA",
    lastExecutionTime: "2026-04-14T00:00:00+00:00",
    split: 7,
    casesRecords: casesFile,
  });

  assert.equal(entry.split, 7);
  assert.equal(entry.cases.length, 2);
  assert.deepEqual(entry.cases[0], {
    name: "case1",
    status: "AC",
    time_ms: 12,
    memory_kb: 34,
  });
  assert.equal(entry.cases[1].detail, "detail text");
});

test("carryOverResults and finalizeResults keep only cached target env results", () => {
  const dir = makeTempDir();
  const prevFile = path.join(dir, "prev.json");
  const needRerunFile = path.join(dir, "need-rerun.txt");
  const splitTestsFile = path.join(dir, "split-tests.txt");
  const jsonlFile = path.join(dir, "result.jsonl");
  const outJsonFile = path.join(dir, "result.json");

  fs.writeFileSync(
    prevFile,
    JSON.stringify(
      {
        tests: {
          "test/a.test.cpp": {
            problem: "A",
            time_limit_ms: 1000,
            split: 1,
            environments: {
              "x64-g++": {
                status: "AC",
                last_execution_time: "2026-04-14T00:00:00+00:00",
                summary: { time_max_ms: 1, time_total_ms: 1, memory_max_kb: 1 },
                cases: [],
              },
              "x64-clang++": {
                status: "WA",
                last_execution_time: "2026-04-14T00:00:00+00:00",
                summary: { time_max_ms: 2, time_total_ms: 2, memory_max_kb: 2 },
                cases: [],
              },
            },
          },
          "test/b.test.cpp": {
            problem: "B",
            time_limit_ms: 1000,
            environments: {
              "x64-g++": {
                status: "AC",
                last_execution_time: "2026-04-14T00:00:00+00:00",
                summary: { time_max_ms: 3, time_total_ms: 3, memory_max_kb: 3 },
                cases: [],
              },
            },
          },
        },
        hpp_map: {},
      },
      null,
      2,
    ),
  );
  fs.writeFileSync(needRerunFile, "test/b.test.cpp\n");
  fs.writeFileSync(splitTestsFile, "test/a.test.cpp\ntest/b.test.cpp\n");

  const carried = carryOverResults({
    prevResult: prevFile,
    needRerunFile,
    splitTestsFile,
    env: "x64-g++",
    outJsonl: jsonlFile,
  });
  assert.equal(carried, 1);

  const count = finalizeResults({ inJsonl: jsonlFile, outJson: outJsonFile });
  assert.equal(count, 1);

  const data = JSON.parse(fs.readFileSync(outJsonFile, "utf-8"));
  assert.equal(data.length, 1);
  assert.equal(data[0].file, "test/a.test.cpp");
  assert.equal(data[0].environment, "x64-g++");
});
