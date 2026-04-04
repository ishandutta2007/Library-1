/**
 * テストファイルごとの .md ページを md/test/ 配下に生成するスクリプト
 * ビルド前に実行する: npx tsx scripts/generate-test-pages.ts
 */
import fs from 'fs'
import path from 'path'

const ROOT = path.resolve(__dirname, '..')
const TEST_DIR = path.join(ROOT, 'test')
const OUT_DIR = path.join(ROOT, 'md', 'test')

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

function generateMarkdown(info: TestInfo): string {
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
      const name = hpp.split('/').pop()?.replace(/\.hpp$/, '') || hpp
      md += `- [${name}](${link}) (${hpp})\n`
    }
    md += '\n'
  }

  md += '## Test Results\n\n'
  md += `<TestFileResults test-file="${info.path}" />\n\n`

  md += '## Code\n\n'
  md += '```cpp\n' + info.source + '\n```\n'

  return md
}

// md/test/ をクリーンアップして再生成
if (fs.existsSync(OUT_DIR)) {
  fs.rmSync(OUT_DIR, { recursive: true })
}

const infos = scanTestFiles()
let count = 0

for (const info of infos) {
  // test/yosupo/foo.test.cpp → md/test/yosupo/foo.test.md
  const relPath = info.path.replace(/^test\//, '').replace(/\.cpp$/, '.md')
  const outPath = path.join(OUT_DIR, relPath)
  const outDir = path.dirname(outPath)

  fs.mkdirSync(outDir, { recursive: true })
  fs.writeFileSync(outPath, generateMarkdown(info))
  count++
}

console.log(`Generated ${count} test file pages in md/test/`)
