/**
 * VitePress Dynamic Routes: テストファイルページのデータローダー
 * 842個の .md を生成する代わりに、テンプレート1つ + このデータで動的にページを生成
 *
 * 重いデータ（テスト結果・ソースコード）は静的JSONファイルとして出力し、
 * クライアント側で読み込む
 */
import fs from 'fs'
import path from 'path'
import { buildDependencyGraph, buildTestMap } from '../../docs/.vitepress/build-data'

const ROOT = path.resolve(__dirname, '../..')
const MD_DIR = path.join(ROOT, 'md')
const PUBLIC_DIR = path.join(ROOT, 'md', 'public')
const TEST_DATA_DIR = path.join(PUBLIC_DIR, 'test-data')

// 依存グラフとテストマップを構築
const depGraph = buildDependencyGraph()
const testMap = buildTestMap(depGraph)

// results.json を読み込み（コンパクト形式 or 従来形式）
let resultsData: Record<string, any[]> = {}
const resultsPath = path.join(ROOT, '.verify-results', 'results.json')
if (fs.existsSync(resultsPath)) {
  try {
    const raw = JSON.parse(fs.readFileSync(resultsPath, 'utf-8'))
    if (raw.tests && raw.hpp_map) {
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
  if (!tests || tests.length === 0) return '⚠'

  const problems = resultsData[hpp]
  if (!problems || problems.length === 0) return '⚠'

  let hasAC = false
  let hasFail = false

  for (const problem of problems) {
    const envs = problem.environments || {}
    for (const [, envResult] of Object.entries(envs) as [string, any][]) {
      const status = envResult.status
      if (status === 'AC' || status === 'IGNORE') {
        hasAC = true
      } else if (status) {
        hasFail = true
      }
    }
  }

  if (hasAC && !hasFail) return '✅'
  if (hasFail && !hasAC) return '❌'
  if (hasAC && hasFail) return '❓'
  return '⚠'
}

function hppToTitle(hpp: string): string {
  const mdPath = path.join(MD_DIR, hpp.replace(/^src\//, '').replace(/\.hpp$/, '.md'))
  if (fs.existsSync(mdPath)) {
    const content = fs.readFileSync(mdPath, 'utf-8')
    const titleMatch = content.match(/^---\s*\n[\s\S]*?title:\s*(.+)\n[\s\S]*?---/)
    if (titleMatch) return titleMatch[1].trim()
  }
  return hpp.split('/').pop()?.replace(/\.hpp$/, '') || hpp
}

interface DependInfo {
  icon: string
  title: string
  link: string
  hpp: string
}

export default {
  paths() {
    const testDir = path.join(ROOT, 'test')
    const results: { params: Record<string, any> }[] = []

    // 静的JSONファイルの出力先を準備
    if (fs.existsSync(TEST_DATA_DIR)) {
      fs.rmSync(TEST_DATA_DIR, { recursive: true })
    }

    function scan(dir: string) {
      for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
        const full = path.join(dir, entry.name)
        if (entry.isDirectory()) {
          scan(full)
        } else if (entry.name.endsWith('.test.cpp')) {
          const source = fs.readFileSync(full, 'utf-8')
          const rel = path.relative(ROOT, full) // "test/misc/foo.test.cpp"

          const problemMatch = source.match(/competitive-verifier:\s*PROBLEM\s+(\S+)/)
          const problem = problemMatch ? problemMatch[1] : null

          const directIncludes: string[] = []
          for (const m of source.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) {
            directIncludes.push(m[1])
          }

          // depends 情報を構築
          const depends: DependInfo[] = directIncludes.map(hpp => ({
            icon: hppStatusIcon(hpp),
            title: hppToTitle(hpp),
            link: '/' + hpp.replace(/^src\//, '').replace(/\.hpp$/, ''),
            hpp,
          }))

          // テスト結果
          const testResult = getTestResult(rel)

          // URL パス: test/misc/foo.test.cpp → misc/foo.test
          const urlPath = rel.replace(/^test\//, '').replace(/\.cpp$/, '')

          // 重いデータを静的JSONファイルとして書き出す
          const jsonPath = path.join(TEST_DATA_DIR, urlPath + '.json')
          fs.mkdirSync(path.dirname(jsonPath), { recursive: true })
          fs.writeFileSync(jsonPath, JSON.stringify({
            testResult,
            source,
          }))

          // params には軽量データのみ
          results.push({
            params: {
              id: urlPath,
              title: rel,
              githubUrl: `https://github.com/hashiryo/Library/blob/master/${rel}`,
              problem,
              depends,
            }
          })
        }
      }
    }

    scan(testDir)
    results.sort((a, b) => a.params.title.localeCompare(b.params.title))
    console.log(`Dynamic routes: ${results.length} test pages, data written to docs/public/test-data/`)
    return results
  }
}
