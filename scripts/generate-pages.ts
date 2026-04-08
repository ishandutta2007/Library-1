/**
 * ビルド前にページを自動生成するスクリプト
 * - md がない hpp のスタブページを生成
 *
 * テストファイルページは VitePress Dynamic Routes で生成するため、
 * このスクリプトでは扱わない (md/test/[...path].paths.ts を参照)
 *
 * 実行: npx tsx scripts/generate-pages.ts
 */
import fs from 'fs'
import path from 'path'

const ROOT = path.resolve(__dirname, '..')
const SRC_DIR = path.join(ROOT, 'src')
const MD_DIR = path.join(ROOT, 'md')

// ============================================================
// md がない hpp のスタブページ生成
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
