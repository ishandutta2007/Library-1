/**
 * GitHub Actions の Job Summary に検証結果のサマリーを出力する
 */
import fs from 'fs'
import path from 'path'
import { loadResults } from './lib/results'

const ROOT = path.resolve(__dirname, '..')
const LOGS_DIR = path.join(ROOT, '.cache/logs')

const data = loadResults()

if (Object.keys(data).length === 0) {
  console.log('No results found.')
  process.exit(0)
}

let passed = 0
let failed = 0
let skipped = 0
let ce = 0
const envSet = new Set<string>()

interface EnvInfo {
  status: string
  timeMax: number
  memMax: number
  compileError?: string
  caseDetails?: { name: string; status: string; detail?: string }[]
}

interface FileRow {
  file: string
  envResults: Record<string, EnvInfo>
}

const rows: FileRow[] = []

for (const problems of Object.values(data) as any[]) {
  for (const problem of problems) {
    const row: FileRow = { file: problem.file, envResults: {} }
    for (const [env, result] of Object.entries(problem.environments) as [string, any][]) {
      envSet.add(env)
      const status = result.status
      const timeMax = result.summary?.time_max_ms ?? 0
      const memMax = result.summary?.memory_max_kb ?? 0
      const compileError = result.compile_error || undefined
      const cases = result.cases || []
      const caseDetails = cases
        .filter((c: any) => c.status !== 'AC')
        .slice(0, 5)
        .map((c: any) => ({ name: c.name, status: c.status, detail: c.detail }))

      row.envResults[env] = { status, timeMax, memMax, compileError, caseDetails }
      switch (status) {
        case 'AC': passed++; break
        case 'CE': ce++; break
        case 'WA': case 'TLE': case 'MLE': case 'RE': failed++; break
        case 'IGNORE': skipped++; break
        default: skipped++; break
      }
    }
    rows.push(row)
  }
}

const total = passed + failed + ce + skipped
const envs = [...envSet].sort()

// --- ログファイル読み込みヘルパー ---
function readLogFile(env: string, file: string): string | undefined {
  const logDir = path.join(LOGS_DIR, env, file)
  if (!fs.existsSync(logDir)) return undefined
  try {
    const entries = fs.readdirSync(logDir).filter(f => f.endsWith('.log')).sort()
    if (entries.length === 0) return undefined
    const lines: string[] = []
    for (const entry of entries.slice(0, 5)) {
      const content = fs.readFileSync(path.join(logDir, entry), 'utf-8').trim()
      if (content) {
        lines.push(`[${entry.replace('.log', '')}]`)
        lines.push(content)
        lines.push('')
      }
    }
    if (entries.length > 5) {
      lines.push(`... and ${entries.length - 5} more log files`)
    }
    return lines.join('\n').trim() || undefined
  } catch {
    return undefined
  }
}

// --- Markdown 出力 ---
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
  Object.values(r.envResults).some(info => info.status !== 'AC' && info.status !== 'IGNORE')
)

function formatMem(kb: number): string {
  if (kb >= 1024) return `${(kb / 1024).toFixed(1)}MB`
  return `${kb}KB`
}

if (failedRows.length > 0) {
  console.log('### Failed / Non-AC Tests\n')
  console.log(`| File | ${envs.map(e => `${e}`).join(' | ')} |`)
  console.log(`|:---|${envs.map(() => ':---:').join('|')}|`)
  for (const row of failedRows.slice(0, 50)) {
    const cells = envs.map(env => {
      const info = row.envResults[env]
      if (!info) return '-'
      if (info.status === 'AC') return `✅ ${info.timeMax}ms ${formatMem(info.memMax)}`
      if (info.status === 'CE') return '🔨 CE'
      if (info.status === 'IGNORE') return '⏭️'
      return `❌ ${info.status} ${info.timeMax}ms ${formatMem(info.memMax)}`
    })
    console.log(`| ${row.file} | ${cells.join(' | ')} |`)
  }
  if (failedRows.length > 50) {
    console.log(`\n... and ${failedRows.length - 50} more`)
  }

  // --- エラー詳細セクション ---
  console.log('\n### Error Details\n')
  for (const row of failedRows.slice(0, 30)) {
    const details: string[] = []

    for (const env of envs) {
      const info = row.envResults[env]
      if (!info || info.status === 'AC' || info.status === 'IGNORE') continue

      // CE: compile_error フィールドから取得
      if (info.status === 'CE' && info.compileError) {
        const truncated = info.compileError.split('\n').slice(0, 20).join('\n')
        details.push(`**${env}** — CE\n\`\`\`\n${truncated}\n\`\`\``)
        continue
      }

      // WA/RE/TLE/MLE: cases の detail + ログファイル
      const parts: string[] = []

      // cases に含まれる detail
      if (info.caseDetails && info.caseDetails.length > 0) {
        for (const c of info.caseDetails) {
          const detailStr = c.detail ? `: ${c.detail}` : ''
          parts.push(`- \`${c.name}\` ${c.status}${detailStr}`)
        }
      }

      // ログファイルから読み取り
      const logContent = readLogFile(env, row.file)
      if (logContent) {
        parts.push('```')
        parts.push(logContent.split('\n').slice(0, 30).join('\n'))
        parts.push('```')
      }

      if (parts.length > 0) {
        details.push(`**${env}** — ${info.status}\n${parts.join('\n')}`)
      }
    }

    if (details.length > 0) {
      console.log(`<details><summary>${row.file}</summary>\n`)
      console.log(details.join('\n\n'))
      console.log('\n</details>\n')
    }
  }
}
