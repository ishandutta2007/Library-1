/**
 * ビルド前にページを自動生成するスクリプト
 * 1. テストファイルページ (md/test/)
 * 2. md がない hpp のスタブページ
 *
 * 実行: npx tsx scripts/generate-pages.ts
 */
import fs from 'fs'
import path from 'path'
import { buildDependencyGraph, buildTestMap } from '../docs/.vitepress/build-data'

const ROOT = path.resolve(__dirname, '..')
const SRC_DIR = path.join(ROOT, 'src')
const TEST_DIR = path.join(ROOT, 'test')
const MD_DIR = path.join(ROOT, 'md')
const TEST_OUT_DIR = path.join(MD_DIR, 'test')

// 依存グラフとテストマップを構築
const depGraph = buildDependencyGraph()
const testMap = buildTestMap(depGraph)

// results.json を読み込み (VitePress 用のフル版があればそれを、なければ git 用)
let resultsData: Record<string, any[]> = {}
const publicResultsPath = path.join(MD_DIR, 'public', 'results.json')
const gitResultsPath = path.join(ROOT, '.verify-results', 'results.json')
if (fs.existsSync(publicResultsPath)) {
  try { resultsData = JSON.parse(fs.readFileSync(publicResultsPath, 'utf-8')) } catch {}
} else if (fs.existsSync(gitResultsPath)) {
  // コンパクト形式から従来形式に変換
  try {
    const raw = JSON.parse(fs.readFileSync(gitResultsPath, 'utf-8'))
    if (raw.tests && raw.hpp_map) {
      // コンパクト → 従来形式に変換（テストファイルで引けるようにフラット化）
      for (const [hpp, files] of Object.entries(raw.hpp_map) as [string, string[]][]) {
        resultsData[hpp] = files.map(f => ({
          file: f,
          problem: raw.tests[f]?.problem || '',
          time_limit_ms: raw.tests[f]?.time_limit_ms || 0,
          environments: raw.tests[f]?.environments || {},
        }))
      }
    } else {
      resultsData = raw
    }
  } catch {}
}

// テストファイルから結果を検索
function getTestResult(testFile: string): any | null {
  for (const problems of Object.values(resultsData)) {
    for (const p of problems) {
      if (p.file === testFile) return p
    }
  }
  return null
}

function hppStatusIcon(hpp: string): string {
  const tests = testMap[hpp]
  if (!tests || tests.length === 0) return '❓'
  return '✅'
}

// ============================================================
// 1. テストファイルページの生成
// ============================================================

interface TestInfo {
  path: string
  problem: string | null
  depends: string[]
  source: string
}

function scanTestFiles(): TestInfo[] {
  const infos: TestInfo[] = []

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) {
        scan(full)
      } else if (entry.name.endsWith('.test.cpp')) {
        const content = fs.readFileSync(full, 'utf-8')
        const rel = path.relative(ROOT, full)

        const problemMatch = content.match(/competitive-verifier:\s*PROBLEM\s+(\S+)/)
        const problem = problemMatch ? problemMatch[1] : null

        const depends: string[] = []
        for (const m of content.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) {
          depends.push(m[1])
        }

        infos.push({ path: rel, problem, depends, source: content })
      }
    }
  }

  scan(TEST_DIR)
  infos.sort((a, b) => a.path.localeCompare(b.path))
  return infos
}

function generateTestMarkdown(info: TestInfo): string {
  const githubUrl = `https://github.com/hashiryo/Library/blob/master/${info.path}`

  let md = `---\ntitle: "${info.path}"\n---\n\n`
  md += `# ${info.path}\n\n`
  md += `- [View this file on GitHub](${githubUrl})\n`
  if (info.problem) {
    md += `- Problem: [${info.problem}](${info.problem})\n`
  }
  md += '\n'

  if (info.depends.length > 0) {
    md += '## Depends on\n\n'
    for (const hpp of info.depends) {
      const link = '/' + hpp.replace(/^src\//, '').replace(/\.hpp$/, '')
      const icon = hppStatusIcon(hpp)
      // md のタイトルがあればそれを使う
      const mdRelPath = hpp.replace(/^src\//, '').replace(/\.hpp$/, '.md')
      const mdPath = path.join(MD_DIR, mdRelPath)
      let title = hpp.split('/').pop()?.replace(/\.hpp$/, '') || hpp
      if (fs.existsSync(mdPath)) {
        const content = fs.readFileSync(mdPath, 'utf-8')
        const titleMatch = content.match(/^---\s*\n[\s\S]*?title:\s*(.+)\n[\s\S]*?---/)
        if (titleMatch) title = titleMatch[1].trim()
      }
      md += `- ${icon} [${title}](${link}) (${hpp})\n`
    }
    md += '\n'
  }

  md += '## Test Results\n\n'
  const testResult = getTestResult(info.path)
  if (testResult) {
    const jsonStr = JSON.stringify(testResult).replace(/'/g, '&#39;')
    md += `<TestFileResults :data='${jsonStr}' />\n\n`
  } else {
    md += `<TestFileResults />\n\n`
  }

  md += '## Code\n\n'
  md += '```cpp\n' + info.source + '\n```\n'

  return md
}

// ============================================================
// 2. md がない hpp のスタブページ生成
// ============================================================

function scanHppFiles(): string[] {
  const hpps: string[] = []

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) {
        scan(full)
      } else if (entry.name.endsWith('.hpp')) {
        hpps.push(path.relative(ROOT, full)) // "src/DataStructure/Foo.hpp"
      }
    }
  }

  scan(SRC_DIR)
  hpps.sort()
  return hpps
}

function generateStubMarkdown(hppPath: string): string {
  // src/DataStructure/Foo.hpp → Foo
  const name = hppPath.split('/').pop()?.replace(/\.hpp$/, '') || hppPath
  const docOf = '../../' + hppPath

  let md = `---\ntitle: ${name}\ndocumentation_of: ${docOf}\n---\n`

  return md
}

// ============================================================
// メイン
// ============================================================

// 1. テストファイルページ
if (fs.existsSync(TEST_OUT_DIR)) {
  fs.rmSync(TEST_OUT_DIR, { recursive: true })
}

const testInfos = scanTestFiles()
let testCount = 0

for (const info of testInfos) {
  const relPath = info.path.replace(/^test\//, '').replace(/\.cpp$/, '.md')
  const outPath = path.join(TEST_OUT_DIR, relPath)
  const outDir = path.dirname(outPath)

  fs.mkdirSync(outDir, { recursive: true })
  fs.writeFileSync(outPath, generateTestMarkdown(info))
  testCount++
}

console.log(`Generated ${testCount} test file pages in md/test/`)

// 2. md がない hpp のスタブページ
const hpps = scanHppFiles()
let stubCount = 0

for (const hpp of hpps) {
  // src/DataStructure/Foo.hpp → md/DataStructure/Foo.md
  const mdRelPath = hpp.replace(/^src\//, '').replace(/\.hpp$/, '.md')
  const mdPath = path.join(MD_DIR, mdRelPath)

  if (!fs.existsSync(mdPath)) {
    const mdDir = path.dirname(mdPath)
    fs.mkdirSync(mdDir, { recursive: true })
    fs.writeFileSync(mdPath, generateStubMarkdown(hpp))
    stubCount++
    console.log(`  Created stub: ${mdRelPath}`)
  }
}

console.log(`Generated ${stubCount} stub pages for hpp files without documentation`)
