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
  cases: CaseResult[]
}

interface MergedProblem {
  problem: string
  file: string
  time_limit_ms: number
  environments: Record<string, EnvSummary>
}

// hpp → テストファイル のマッピング
function buildHppMap(): Record<string, string[]> {
  const testDir = path.join(ROOT, 'test')
  const map: Record<string, string[]> = {}

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) scan(full)
      else if (entry.name.endsWith('.test.cpp')) {
        const content = fs.readFileSync(full, 'utf-8')
        const rel = path.relative(ROOT, full)
        for (const m of content.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) {
          if (!map[m[1]]) map[m[1]] = []
          map[m[1]].push(rel)
        }
      }
    }
  }

  scan(testDir)
  return map
}

// 引数パース
const args = process.argv.slice(2)
const fromCommitted = args.includes('--from-committed')
const prevIdx = args.indexOf('--prev')
const prevFile = prevIdx >= 0 ? args[prevIdx + 1] : null

const OUTPUT = path.join(ROOT, '.verify-results', 'results.json')
const PUBLIC_OUTPUT = path.join(ROOT, 'md', 'public', 'results.json')

// --from-committed: 既存の results.json を md/public/ にコピーするだけ
if (fromCommitted) {
  if (fs.existsSync(OUTPUT)) {
    fs.mkdirSync(path.dirname(PUBLIC_OUTPUT), { recursive: true })
    fs.copyFileSync(OUTPUT, PUBLIC_OUTPUT)
    console.log(`Copied ${OUTPUT} to ${PUBLIC_OUTPUT}`)
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
let prevData: Record<string, MergedProblem[]> = {}
const prevPath = prevFile || OUTPUT
if (fs.existsSync(prevPath)) {
  prevData = JSON.parse(fs.readFileSync(prevPath, 'utf-8'))
  console.log(`Loaded previous results from ${prevPath}`)
}

// 前回結果をフラットなマップに変換: file → { env → EnvSummary }
const prevMap: Record<string, Record<string, EnvSummary>> = {}
for (const problems of Object.values(prevData)) {
  for (const problem of problems) {
    prevMap[problem.file] = { ...(prevMap[problem.file] || {}), ...problem.environments }
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

for (const [hpp, testFiles] of Object.entries(hppMap)) {
  const problems: MergedProblem[] = []
  for (const tf of testFiles) {
    if (prevMap[tf]) {
      const meta = getTestMeta(tf)
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

// 出力
fs.mkdirSync(path.dirname(OUTPUT), { recursive: true })
fs.writeFileSync(OUTPUT, JSON.stringify(output, null, 2))

fs.mkdirSync(path.dirname(PUBLIC_OUTPUT), { recursive: true })
fs.copyFileSync(OUTPUT, PUBLIC_OUTPUT)

const testCount = Object.values(prevMap).length
const hppCount = Object.keys(output).length
console.log(`\nResults: ${testCount} test files, ${hppCount} hpp files`)
console.log(`Written to ${OUTPUT} and ${PUBLIC_OUTPUT}`)
