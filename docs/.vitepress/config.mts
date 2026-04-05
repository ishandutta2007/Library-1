import { defineConfig } from 'vitepress'
import fs from 'fs'
import path from 'path'
import { buildTestMap, buildDependencyGraph, readSourceCode } from './build-data'
import { katex } from '@mdit/plugin-katex'

const ROOT = path.resolve(__dirname, '../..')

// md ファイルからフロントマターを読む
function readFrontmatter(mdPath: string): { title?: string; order?: number } {
  if (!fs.existsSync(mdPath)) return {}
  const content = fs.readFileSync(mdPath, 'utf-8')
  const titleMatch = content.match(/^---\s*\n[\s\S]*?title:\s*(.+)\n[\s\S]*?---/)
  const orderMatch = content.match(/^---\s*\n[\s\S]*?order:\s*(\d+)\n[\s\S]*?---/)
  return {
    title: titleMatch ? titleMatch[1].trim() : undefined,
    order: orderMatch ? parseInt(orderMatch[1]) : undefined,
  }
}

// src/ の hpp 構造から再帰的にサイドバーを自動生成
function generateSidebar() {
  const srcDir = path.join(ROOT, 'src')
  const mdDir = path.join(ROOT, 'md')

  function buildItems(srcPath: string, mdPath: string, linkPrefix: string): any[] {
    const entries = fs.readdirSync(srcPath, { withFileTypes: true })

    const items: { text: string; link?: string; collapsed?: boolean; items?: any[]; order: number }[] = []

    // hpp ファイル
    for (const entry of entries) {
      if (!entry.isFile() || !entry.name.endsWith('.hpp')) continue
      const name = entry.name.replace(/\.hpp$/, '')
      const fm = readFrontmatter(path.join(mdPath, name + '.md'))

      items.push({
        text: fm.title || name,
        link: `${linkPrefix}/${name}`,
        order: fm.order ?? 999,
      })
    }

    // サブディレクトリ（再帰）
    for (const entry of entries) {
      if (!entry.isDirectory()) continue
      const subSrcPath = path.join(srcPath, entry.name)
      const subMdPath = path.join(mdPath, entry.name)
      const subItems = buildItems(subSrcPath, subMdPath, `${linkPrefix}/${entry.name}`)

      if (subItems.length > 0) {
        // ディレクトリ自体の order を _index.md や最初のアイテムから推測
        const indexFm = readFrontmatter(path.join(subMdPath, '_index.md'))
        items.push({
          text: indexFm.title || entry.name,
          collapsed: true,
          items: subItems,
          order: indexFm.order ?? 999,
        })
      }
    }

    // order → テキスト名でソート
    items.sort((a, b) => a.order !== b.order ? a.order - b.order : a.text.localeCompare(b.text))

    return items
  }

  // トップレベルのカテゴリ
  const categories = fs.readdirSync(srcDir, { withFileTypes: true })
    .filter((e: fs.Dirent) => e.isDirectory())

  const sidebar: any[] = []
  for (const cat of categories) {
    const srcPath = path.join(srcDir, cat.name)
    const mdPath = path.join(mdDir, cat.name)
    const items = buildItems(srcPath, mdPath, `/${cat.name}`)

    if (items.length > 0) {
      const indexFm = readFrontmatter(path.join(mdPath, '_index.md'))
      sidebar.push({
        text: indexFm.title || cat.name,
        collapsed: true,
        items,
        order: indexFm.order ?? 999,
      })
    }
  }

  sidebar.sort((a, b) => a.order !== b.order ? a.order - b.order : a.text.localeCompare(b.text))
  return sidebar
}

// ビルド時に依存グラフとテストマッピングを構築
const depGraph = buildDependencyGraph()
const testMap = buildTestMap(depGraph)

// hpp パスから md ページへのリンクとタイトルを生成するヘルパー
// "src/DataStructure/BinaryIndexedTree.hpp" → link: "/DataStructure/BinaryIndexedTree", title: "BinaryIndexedTree"
function hppToLink(hpp: string) {
  return '/' + hpp.replace(/^src\//, '').replace(/\.hpp$/, '')
}

function hppToTitle(hpp: string) {
  // 対応する md があればそのタイトルを使う
  const mdPath = path.join(ROOT, 'md', hpp.replace(/^src\//, '').replace(/\.hpp$/, '.md'))
  if (fs.existsSync(mdPath)) {
    const content = fs.readFileSync(mdPath, 'utf-8')
    const titleMatch = content.match(/^---\s*\n[\s\S]*?title:\s*(.+)\n[\s\S]*?---/)
    if (titleMatch) return titleMatch[1].trim()
  }
  return hpp.split('/').pop()?.replace(/\.hpp$/, '') || hpp
}

// hpp の verify ステータスアイコン
function hppStatusIcon(hpp: string): string {
  const tests = testMap[hpp]
  if (!tests || tests.length === 0) return '❓'
  // TODO: Phase 2 で results.json を読んで AC/WA/TLE を判定
  // 今はテストがあれば ✅ とする
  return '✅'
}

// ステータスアイコン付きのリンクを生成
function hppLinkWithStatus(hpp: string): string {
  const icon = hppStatusIcon(hpp)
  const title = hppToTitle(hpp)
  const link = hppToLink(hpp)
  return `${icon} [${title}](${link})`
}

export default defineConfig({
  title: "Hashiryo's Library",
  description: '競技プログラミング用C++ライブラリ',

  // md/ ディレクトリをソースとして使う
  srcDir: '../md',

  // 既存 md 内の .hpp 等へのリンクは無視
  ignoreDeadLinks: true,

  // 数式サポート (KaTeX)
  markdown: {
    config: (md) => {
      md.use(katex)
    }
  },

  // Vite プラグインで Markdown ソースを変換
  vite: {
    plugins: [
      {
        name: 'inject-library-sections',
        enforce: 'pre',
        transform(code, id) {
          // md ファイルのみ対象
          if (!id.endsWith('.md')) return null

          // frontmatter から documentation_of を取得
          const docOfMatch = code.match(/^---\s*\n[\s\S]*?documentation_of:\s*(.+)\n[\s\S]*?---/)
          if (!docOfMatch) return null

          const docOf = docOfMatch[1].trim()
          const hppPath = docOf.replace(/^(\.\.\/)+/, '')

          // --- md の前に挿入するセクション ---
          let before = ''

          // タイトル (ステータスアイコン付き)
          const icon = hppStatusIcon(hppPath)
          const title = hppToTitle(hppPath)
          before += `\n# ${icon} ${title}\n`

          // Code セクション
          const source = readSourceCode(hppPath)
          if (source) {
            const githubUrl = `https://github.com/hashiryo/Library/blob/master/${hppPath}`
            before += '\n## Code\n\n'
            before += `[${hppPath}](${githubUrl})\n\n`
            before += '```cpp\n' + source + '\n```\n'
          }

          // --- md の後に挿入するセクション ---
          let after = ''

          // Verified with セクション (マトリクス表コンポーネント)
          const tests = testMap[hppPath] || []
          if (tests.length > 0) {
            after += '\n## Verified with\n\n<VerifyMatrix />\n'
          }

          // Depends on セクション (この hpp が依存している hpp)
          const deps = depGraph.dependsOn[hppPath] || []
          if (deps.length > 0) {
            after += '\n## Depends on\n\n'
            for (const dep of deps) {
              after += `- ${hppLinkWithStatus(dep)} (${dep})\n`
            }
          }

          // Required by セクション (この hpp に依存している hpp)
          const reqBy = depGraph.requiredBy[hppPath] || []
          if (reqBy.length > 0) {
            after += '\n## Required by\n\n'
            for (const req of reqBy) {
              after += `- ${hppLinkWithStatus(req)} (${req})\n`
            }
          }

          if (!before && !after) return null

          // frontmatter の直後に before を挿入、末尾に after を追加
          const fmEnd = code.indexOf('---', code.indexOf('---') + 3)
          if (fmEnd === -1) return before + code + after
          const insertPos = fmEnd + 3
          return code.slice(0, insertPos) + '\n' + before + code.slice(insertPos) + after
        }
      }
    ]
  },

  themeConfig: {
    sidebar: generateSidebar(),

    nav: [
      { text: 'Home', link: '/' },
      { text: 'GitHub', link: 'https://github.com/hashiryo/Library' }
    ],

    search: {
      provider: 'local'
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/hashiryo/Library' }
    ]
  }
})
