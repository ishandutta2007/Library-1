/**
 * GitHub Actions の Job Summary に検証結果のサマリーを出力する
 * merge-results.ts の後に実行する
 */
import fs from 'fs'
import path from 'path'

const ROOT = path.resolve(__dirname, '..')
const resultsPath = path.join(ROOT, 'md/public/results.json')

if (!fs.existsSync(resultsPath)) {
  console.log('No results found.')
  process.exit(0)
}

const data = JSON.parse(fs.readFileSync(resultsPath, 'utf-8'))

// 全テスト結果を集計
let passed = 0
let failed = 0
let skipped = 0
let ce = 0
const envSet = new Set<string>()

interface FileRow {
  file: string
  envResults: Record<string, string>
}

const rows: FileRow[] = []

for (const problems of Object.values(data) as any[]) {
  for (const problem of problems) {
    const row: FileRow = { file: problem.file, envResults: {} }
    for (const [env, result] of Object.entries(problem.environments) as [string, any][]) {
      envSet.add(env)
      row.envResults[env] = result.status
      switch (result.status) {
        case 'AC': passed++; break
        case 'CE': ce++; break
        case 'WA': case 'TLE': case 'MLE': case 'RE': failed++; break
        default: skipped++; break
      }
    }
    rows.push(row)
  }
}

const total = passed + failed + ce + skipped
const envs = [...envSet].sort()

// Markdown 出力
console.log('## Verification Result\n')

if (failed === 0 && ce === 0) {
  console.log('✅ All test cases passed!\n')
} else {
  console.log('❌ Some test cases failed.\n')
}

console.log(`| | Count |`)
console.log(`|:---|---:|`)
console.log(`| ✅ Passed | ${passed} |`)
console.log(`| ❌ Failed | ${failed} |`)
console.log(`| 🔨 CE | ${ce} |`)
console.log(`| ⏭️ Skipped | ${skipped} |`)
console.log(`| **Total** | **${total}** |`)
console.log()

// 失敗したテストの詳細
const failedRows = rows.filter(r =>
  Object.values(r.envResults).some(s => s !== 'AC')
)

if (failedRows.length > 0) {
  console.log('### Failed / Non-AC Tests\n')
  console.log(`| File | ${envs.join(' | ')} |`)
  console.log(`|:---|${envs.map(() => ':---:').join('|')}|`)
  for (const row of failedRows.slice(0, 50)) {
    const cells = envs.map(env => {
      const s = row.envResults[env]
      if (!s) return '-'
      if (s === 'AC') return '✅'
      if (s === 'CE') return '🔨'
      return `❌ ${s}`
    })
    console.log(`| ${row.file} | ${cells.join(' | ')} |`)
  }
  if (failedRows.length > 50) {
    console.log(`\n... and ${failedRows.length - 50} more`)
  }
}
