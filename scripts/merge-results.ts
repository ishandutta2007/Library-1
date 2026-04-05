/**
 * 各環境の結果 JSON をマージして、VitePress 用の results.json を生成する
 *
 * 入力: .cache/results/result-{env}.json (各環境の結果)
 * 出力: md/public/results.json (VitePress 用)
 *
 * 使い方: npx tsx scripts/merge-results.ts [結果ファイル...]
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

// hpp → テストファイル のマッピングを構築
function buildHppMap(): Record<string, string[]> {
  const testDir = path.join(ROOT, 'test')
  const map: Record<string, string[]> = {}

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) {
        scan(full)
      } else if (entry.name.endsWith('.test.cpp')) {
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

// 結果ファイルの読み込み
const resultFiles = process.argv.slice(2)
if (resultFiles.length === 0) {
  // デフォルト: .cache/results/ から全て読む
  const resultDir = path.join(ROOT, '.cache/results')
  if (fs.existsSync(resultDir)) {
    for (const f of fs.readdirSync(resultDir)) {
      if (f.startsWith('result-') && f.endsWith('.json')) {
        resultFiles.push(path.join(resultDir, f))
      }
    }
  }
}

if (resultFiles.length === 0) {
  console.log('No result files found')
  process.exit(0)
}

// 全環境の結果を読み込み
const allResults: TestResult[] = []
for (const file of resultFiles) {
  const data: TestResult[] = JSON.parse(fs.readFileSync(file, 'utf-8'))
  allResults.push(...data)
  console.log(`Loaded ${data.length} results from ${path.basename(file)}`)
}

// file + problem をキーにしてマージ
const mergedMap: Record<string, MergedProblem> = {}

for (const result of allResults) {
  const key = result.file
  if (!mergedMap[key]) {
    // TLE をアノテーションから読む
    const testFilePath = path.join(ROOT, result.file)
    let tlMs = 0
    if (fs.existsSync(testFilePath)) {
      const content = fs.readFileSync(testFilePath, 'utf-8')
      const tleMatch = content.match(/competitive-verifier:\s*TLE\s+([0-9.]+)/)
      if (tleMatch) tlMs = Math.round(parseFloat(tleMatch[1]) * 1000)
    }

    mergedMap[key] = {
      problem: result.problem,
      file: result.file,
      time_limit_ms: tlMs,
      environments: {},
    }
  }

  const merged = mergedMap[key]

  // サマリ計算
  const cases = result.cases
  const timeMax = cases.length > 0 ? Math.max(...cases.map(c => c.time_ms)) : 0
  const timeTotal = cases.reduce((sum, c) => sum + c.time_ms, 0)
  const memMax = cases.length > 0 ? Math.max(...cases.map(c => c.memory_kb)) : 0

  merged.environments[result.environment] = {
    status: result.status,
    summary: { time_max_ms: timeMax, time_total_ms: timeTotal, memory_max_kb: memMax },
    cases,
  }
}

// hpp ごとにグループ化
const hppMap = buildHppMap()
const output: Record<string, MergedProblem[]> = {}

for (const [hpp, testFiles] of Object.entries(hppMap)) {
  const problems: MergedProblem[] = []
  for (const tf of testFiles) {
    if (mergedMap[tf]) {
      problems.push(mergedMap[tf])
    }
  }
  if (problems.length > 0) {
    output[hpp] = problems
  }
}

// 出力
const outPath = path.join(ROOT, 'md/public/results.json')
fs.mkdirSync(path.dirname(outPath), { recursive: true })
fs.writeFileSync(outPath, JSON.stringify(output, null, 2))
console.log(`\nMerged results written to ${outPath}`)
console.log(`  ${Object.keys(output).length} hpp files, ${Object.keys(mergedMap).length} test files`)
