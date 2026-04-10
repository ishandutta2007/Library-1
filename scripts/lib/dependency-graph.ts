/**
 * src/ 内の hpp 間の依存グラフ構築とテストマッピング
 */
import fs from 'fs'
import path from 'path'

const ROOT = path.resolve(__dirname, '../..')
const SRC_DIR = path.join(ROOT, 'src')
const TEST_DIR = path.join(ROOT, 'test')

export interface DependencyGraph {
  dependsOn: Record<string, string[]>
  requiredBy: Record<string, string[]>
  transitiveDeps: Record<string, Set<string>>
}

export function buildDependencyGraph(): DependencyGraph {
  const dependsOn: Record<string, string[]> = {}
  const requiredBy: Record<string, string[]> = {}

  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) scan(full)
      else if (entry.name.endsWith('.hpp')) {
        const content = fs.readFileSync(full, 'utf-8')
        const rel = path.relative(ROOT, full)
        const deps: string[] = []
        for (const m of content.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) deps.push(m[1])
        dependsOn[rel] = deps
        for (const dep of deps) {
          if (!requiredBy[dep]) requiredBy[dep] = []
          requiredBy[dep].push(rel)
        }
      }
    }
  }
  scan(SRC_DIR)
  for (const key of Object.keys(dependsOn)) dependsOn[key].sort()
  for (const key of Object.keys(requiredBy)) requiredBy[key].sort()

  const transitiveDeps: Record<string, Set<string>> = {}
  function getTransitive(hpp: string, visited: Set<string>): Set<string> {
    if (transitiveDeps[hpp]) return transitiveDeps[hpp]
    if (visited.has(hpp)) return new Set()
    visited.add(hpp)
    const result = new Set<string>()
    for (const dep of dependsOn[hpp] || []) {
      result.add(dep)
      for (const t of getTransitive(dep, visited)) result.add(t)
    }
    transitiveDeps[hpp] = result
    return result
  }
  for (const hpp of Object.keys(dependsOn)) getTransitive(hpp, new Set())

  return { dependsOn, requiredBy, transitiveDeps }
}

export function buildTestMap(graph: DependencyGraph): Record<string, string[]> {
  const map: Record<string, Set<string>> = {}
  function addTest(hpp: string, test: string) {
    if (!map[hpp]) map[hpp] = new Set()
    map[hpp].add(test)
  }
  function scan(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) scan(full)
      else if (entry.name.endsWith('.test.cpp')) {
        const content = fs.readFileSync(full, 'utf-8')
        const rel = path.relative(ROOT, full)
        for (const m of content.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) {
          addTest(m[1], rel)
          const trans = graph.transitiveDeps[m[1]]
          if (trans) for (const dep of trans) addTest(dep, rel)
        }
      }
    }
  }
  scan(TEST_DIR)
  const result: Record<string, string[]> = {}
  for (const [k, v] of Object.entries(map)) result[k] = [...v].sort()
  return result
}
