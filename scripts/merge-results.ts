/**
 * テスト結果をマージして results.json を生成する
 *
 * 使い方:
 *   npx tsx scripts/merge-results.ts                          # .cache/results/ からマージ
 *   npx tsx scripts/merge-results.ts --prev .verify-results/results.json  # 前回結果とマージ
 *   npx tsx scripts/merge-results.ts --from-committed         # .verify-results/results.json をそのまま md/public/ にコピー
 */
import fs from 'fs'
import path from 'path'

const ROOT = path.resolve(__dirname, '..')

interface CaseResult {
  name: string
  status: string
  time_ms: number
  memory_kb: number
}

interface TestResult {
  file: string
  problem: string
  environment: string
  status: string
  last_execution_time?: string
  cases: CaseResult[]
}

interface EnvSummary {
  status: string
  summary: { time_max_ms: number; time_total_ms: number; memory_max_kb: number }
  last_execution_time?: string
  cases: CaseResult[]
}

interface MergedProblem {
  problem: string
  file: string
  time_limit_ms: number
  environments: Record<string, EnvSummary>
}

// hpp 間の依存グラフ構築
function buildDependsOn(): Record<string, string[]> {
  const srcDir = path.join(ROOT, 'src')
  const dependsOn: Record<string, string[]> = {}

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) scan(full)
      else if (entry.name.endsWith('.hpp')) {
        const content = fs.readFileSync(full, 'utf-8')
        const rel = path.relative(ROOT, full)
        const deps: string[] = []
        for (const m of content.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) {
          deps.push(m[1])
        }
        dependsOn[rel] = deps
      }
    }
  }

  scan(srcDir)
  return dependsOn
}

// 推移閉包
function getTransitiveDeps(hpp: string, dependsOn: Record<string, string[]>, cache: Record<string, Set<string>>): Set<string> {
  if (cache[hpp]) return cache[hpp]
  cache[hpp] = new Set()
  for (const dep of (dependsOn[hpp] || [])) {
    cache[hpp].add(dep)
    for (const transDep of getTransitiveDeps(dep, dependsOn, cache)) {
      cache[hpp].add(transDep)
    }
  }
  return cache[hpp]
}

// hpp → テストファイル のマッピング（推移的依存を考慮）
function buildHppMap(): Record<string, string[]> {
  const testDir = path.join(ROOT, 'test')
  const dependsOn = buildDependsOn()
  const transCache: Record<string, Set<string>> = {}
  const map: Record<string, Set<string>> = {}

  function addTest(hpp: string, testFile: string) {
    if (!map[hpp]) map[hpp] = new Set()
    map[hpp].add(testFile)
  }

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) scan(full)
      else if (entry.name.endsWith('.test.cpp')) {
        const content = fs.readFileSync(full, 'utf-8')
        const rel = path.relative(ROOT, full)
        const directIncludes: string[] = []
        for (const m of content.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) {
          directIncludes.push(m[1])
        }
        // 直接 include + 推移的依存すべてにテストを紐付け
        for (const hpp of directIncludes) {
          addTest(hpp, rel)
          for (const dep of getTransitiveDeps(hpp, dependsOn, transCache)) {
            addTest(dep, rel)
          }
        }
      }
    }
  }

  scan(testDir)
  // Set → sorted array
  const result: Record<string, string[]> = {}
  for (const [key, set] of Object.entries(map)) {
    result[key] = [...set].sort()
  }
  return result
}

// 引数パース
const args = process.argv.slice(2)
const fromCommitted = args.includes('--from-committed')
const prevIdx = args.indexOf('--prev')
const prevFile = prevIdx >= 0 ? args[prevIdx + 1] : null

const OUTPUT = path.join(ROOT, '.verify-results', 'results.json')

// --from-committed: 何もしない（ビルド時に直接 .verify-results/results.json を読む）
if (fromCommitted) {
  if (fs.existsSync(OUTPUT)) {
    console.log(`Results available at ${OUTPUT}`)
  } else {
    console.log('No committed results found')
  }
  process.exit(0)
}

// 新しい結果ファイルの読み込み
const resultDir = path.join(ROOT, '.cache/results')
const newResults: TestResult[] = []
if (fs.existsSync(resultDir)) {
  for (const f of fs.readdirSync(resultDir)) {
    if (f.startsWith('result-') && f.endsWith('.json')) {
      const data: TestResult[] = JSON.parse(fs.readFileSync(path.join(resultDir, f), 'utf-8'))
      newResults.push(...data)
      console.log(`Loaded ${data.length} results from ${f}`)
    }
  }
}

// 前回結果の読み込み（--prev で指定 or デフォルト）
const prevMap: Record<string, Record<string, EnvSummary>> = {}
const prevPath = prevFile || OUTPUT
if (fs.existsSync(prevPath)) {
  const raw = JSON.parse(fs.readFileSync(prevPath, 'utf-8'))
  console.log(`Loaded previous results from ${prevPath}`)

  if (raw.tests && raw.hpp_map) {
    // コンパクト形式
    for (const [file, data] of Object.entries(raw.tests) as [string, any][]) {
      prevMap[file] = data.environments || {}
    }
  } else {
    // 従来形式 (hpp → MergedProblem[])
    for (const problems of Object.values(raw) as any[]) {
      for (const problem of problems) {
        prevMap[problem.file] = { ...(prevMap[problem.file] || {}), ...problem.environments }
      }
    }
  }
}

// 新しい結果をマージ
for (const result of newResults) {
  const key = result.file
  if (!prevMap[key]) prevMap[key] = {}

  const cases = result.cases || []
  const timeMax = cases.length > 0 ? Math.max(...cases.map(c => c.time_ms)) : 0
  const timeTotal = cases.reduce((sum, c) => sum + c.time_ms, 0)
  const memMax = cases.length > 0 ? Math.max(...cases.map(c => c.memory_kb)) : 0

  prevMap[key][result.environment] = {
    status: result.status,
    summary: { time_max_ms: timeMax, time_total_ms: timeTotal, memory_max_kb: memMax },
    last_execution_time: result.last_execution_time || new Date().toISOString(),
    cases,
  }
}

// hpp ごとにグループ化して出力形式に変換
const hppMap = buildHppMap()
const output: Record<string, MergedProblem[]> = {}

// テストファイルの問題URLとTLを取得するヘルパー
function getTestMeta(testFile: string): { problem: string; tlMs: number } {
  const full = path.join(ROOT, testFile)
  if (!fs.existsSync(full)) return { problem: '', tlMs: 0 }
  const content = fs.readFileSync(full, 'utf-8')
  const problemMatch = content.match(/competitive-verifier:\s*PROBLEM\s+(\S+)/)
  const tleMatch = content.match(/competitive-verifier:\s*TLE\s+([0-9.]+)/)
  return {
    problem: problemMatch ? problemMatch[1] : '',
    tlMs: tleMatch ? Math.round(parseFloat(tleMatch[1]) * 1000) : 0,
  }
}

// IGNORE / STANDALONE の判定
function getTestStatus(testFile: string): 'normal' | 'ignore' | 'standalone' {
  const full = path.join(ROOT, testFile)
  if (!fs.existsSync(full)) return 'normal'
  const content = fs.readFileSync(full, 'utf-8')
  if (/competitive-verifier:\s*IGNORE/.test(content)) return 'ignore'
  if (/competitive-verifier:\s*STANDALONE/.test(content)) return 'standalone'
  return 'normal'
}

for (const [hpp, testFiles] of Object.entries(hppMap)) {
  const problems: MergedProblem[] = []
  for (const tf of testFiles) {
    const meta = getTestMeta(tf)
    const testStatus = getTestStatus(tf)

    if (testStatus === 'ignore') {
      // IGNORE テストは全環境 IGNORE ステータスで記録
      const ignoreEnvs: Record<string, EnvSummary> = {}
      for (const env of ['x64-g++', 'x64-clang++', 'arm-g++', 'arm-clang++']) {
        ignoreEnvs[env] = {
          status: 'IGNORE',
          summary: { time_max_ms: 0, time_total_ms: 0, memory_max_kb: 0 },
          cases: [],
        }
      }
      problems.push({
        problem: meta.problem,
        file: tf,
        time_limit_ms: meta.tlMs,
        environments: ignoreEnvs,
      })
    } else if (prevMap[tf]) {
      problems.push({
        problem: meta.problem,
        file: tf,
        time_limit_ms: meta.tlMs,
        environments: prevMap[tf],
      })
    }
  }
  if (problems.length > 0) {
    output[hpp] = problems
  }
}

// git commit 用: テスト結果を重複なしで格納 + hpp マッピング
interface CompactResults {
  // テストファイルごとの結果（重複なし）
  tests: Record<string, {
    problem: string
    time_limit_ms: number
    environments: Record<string, EnvSummary>
  }>
  // hpp → テストファイル一覧のマッピング
  hpp_map: Record<string, string[]>
}

function buildCompactResults(): CompactResults {
  const tests: CompactResults['tests'] = {}

  // テストごとに1回だけ格納
  for (const problems of Object.values(output)) {
    for (const p of problems) {
      if (!tests[p.file]) {
        tests[p.file] = {
          problem: p.problem,
          time_limit_ms: p.time_limit_ms,
          environments: p.environments,
        }
      }
    }
  }

  // hpp → テストファイル一覧
  const hpp_map: Record<string, string[]> = {}
  for (const [hpp, problems] of Object.entries(output)) {
    hpp_map[hpp] = problems.map(p => p.file)
  }

  return { tests, hpp_map }
}

// VitePress 用: 従来形式（hpp ごとにグループ化、重複あり）
// git commit 用: コンパクト形式（重複なし）

const compact = buildCompactResults()

fs.mkdirSync(path.dirname(OUTPUT), { recursive: true })
fs.writeFileSync(OUTPUT, JSON.stringify(compact, null, 2))

const testCount = Object.keys(compact.tests).length
const hppCount = Object.keys(compact.hpp_map).length
console.log(`\nResults: ${testCount} test files, ${hppCount} hpp files`)
console.log(`Written to ${OUTPUT}`)
