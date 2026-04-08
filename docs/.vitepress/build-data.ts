import fs from 'fs'
import path from 'path'

const ROOT = path.resolve(__dirname, '../..')

let highlighter: any = null

export async function getHighlighter() {
  if (!highlighter) {
    const { createHighlighter } = await import('shiki')
    highlighter = await createHighlighter({
      themes: ['github-light', 'github-dark'],
      langs: ['cpp']
    })
  }
  return highlighter
}

/**
 * C++ ソースコードをハイライト済み HTML に変換
 */
export async function highlightCode(code: string): Promise<string> {
  const hl = await getHighlighter()
  return hl.codeToHtml(code, {
    lang: 'cpp',
    themes: { light: 'github-light', dark: 'github-dark' }
  })
}

/**
 * hpp ファイルのソースコードを読む
 */
export function readSourceCode(hppPath: string): string | null {
  const full = path.join(ROOT, hppPath)
  if (!fs.existsSync(full)) return null
  return fs.readFileSync(full, 'utf-8')
}

// ============================================================
// hpp 間の依存グラフ
// ============================================================

export interface DependencyGraph {
  /** hpp → この hpp が直接 include している hpp 一覧 */
  dependsOn: Record<string, string[]>
  /** hpp → この hpp を直接 include している hpp 一覧 */
  requiredBy: Record<string, string[]>
  /** hpp → この hpp が推移的に依存している全 hpp (自身含まず) */
  transitiveDeps: Record<string, Set<string>>
}

/**
 * src/ 内の全 .hpp をスキャンして依存グラフを構築
 */
export function buildDependencyGraph(): DependencyGraph {
  const srcDir = path.join(ROOT, 'src')
  const dependsOn: Record<string, string[]> = {}
  const requiredBy: Record<string, string[]> = {}

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) {
        scan(full)
      } else if (entry.name.endsWith('.hpp')) {
        const content = fs.readFileSync(full, 'utf-8')
        const rel = path.relative(ROOT, full) // "src/DataStructure/Foo.hpp"

        const deps: string[] = []
        for (const m of content.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) {
          deps.push(m[1])
        }

        dependsOn[rel] = deps

        for (const dep of deps) {
          if (!requiredBy[dep]) requiredBy[dep] = []
          requiredBy[dep].push(rel)
        }
      }
    }
  }

  scan(srcDir)

  // ソート
  for (const key of Object.keys(dependsOn)) dependsOn[key].sort()
  for (const key of Object.keys(requiredBy)) requiredBy[key].sort()

  // 推移閉包を計算 (各 hpp から到達可能な全 hpp)
  const transitiveDeps: Record<string, Set<string>> = {}

  function getTransitiveDeps(hpp: string, visited: Set<string>): Set<string> {
    if (transitiveDeps[hpp]) return transitiveDeps[hpp]
    if (visited.has(hpp)) return new Set() // 循環参照回避

    visited.add(hpp)
    const result = new Set<string>()
    for (const dep of (dependsOn[hpp] || [])) {
      result.add(dep)
      for (const transDep of getTransitiveDeps(dep, visited)) {
        result.add(transDep)
      }
    }
    transitiveDeps[hpp] = result
    return result
  }

  for (const hpp of Object.keys(dependsOn)) {
    getTransitiveDeps(hpp, new Set())
  }

  return { dependsOn, requiredBy, transitiveDeps }
}

// ============================================================
// テストファイルマッピング（推移的依存を考慮）
// ============================================================

/**
 * test/ をスキャンして hpp → テストファイル のマッピングを作る
 * 推移的依存を考慮：テストが hpp A を include し、A が hpp B に依存しているなら、B にもこのテストを紐付ける
 */
export function buildTestMap(graph: DependencyGraph): Record<string, string[]> {
  const testDir = path.join(ROOT, 'test')
  const map: Record<string, Set<string>> = {}

  function addTest(hpp: string, testFile: string) {
    if (!map[hpp]) map[hpp] = new Set()
    map[hpp].add(testFile)
  }

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) {
        scan(full)
      } else if (entry.name.endsWith('.test.cpp')) {
        const content = fs.readFileSync(full, 'utf-8')
        const rel = path.relative(ROOT, full)

        // テストが直接 include している hpp
        const directIncludes: string[] = []
        for (const m of content.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) {
          directIncludes.push(m[1])
        }

        // 直接 include + その推移的依存すべてにテストを紐付け
        for (const hpp of directIncludes) {
          addTest(hpp, rel)
          const transDeps = graph.transitiveDeps[hpp]
          if (transDeps) {
            for (const dep of transDeps) {
              addTest(dep, rel)
            }
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

// ============================================================
// テストファイル情報（generate-test-pages 用）
// ============================================================

export interface TestFileInfo {
  path: string
  problem: string | null
  depends: string[]
  source: string
}

export function buildTestFileInfos(): TestFileInfo[] {
  const testDir = path.join(ROOT, 'test')
  const infos: TestFileInfo[] = []

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

  scan(testDir)
  infos.sort((a, b) => a.path.localeCompare(b.path))
  return infos
}
