import { defineConfig } from 'vitepress'
import fs from 'fs'
import path from 'path'
import { buildTestMap, buildDependencyGraph, readSourceCode } from './build-data'
import { katex } from '@mdit/plugin-katex'

const ROOT = path.resolve(__dirname, '../..')

// src/ の hpp 構造からサイドバーを自動生成（md のタイトルがあれば使う）
function generateSidebar() {
  const srcDir = path.join(ROOT, 'src')
  const mdDir = path.join(ROOT, 'md')

  const categories = fs.readdirSync(srcDir).filter(f =>
    fs.statSync(path.join(srcDir, f)).isDirectory()
  ).sort()

  return categories.map(category => {
    const categoryDir = path.join(srcDir, category)
    const hppFiles = fs.readdirSync(categoryDir)
      .filter(f => f.endsWith('.hpp'))
      .sort()

    const items = hppFiles.map(file => {
      const name = file.replace(/\.hpp$/, '')

      // 対応する md があればタイトルを取得
      const mdPath = path.join(mdDir, category, name + '.md')
      let title = name
      if (fs.existsSync(mdPath)) {
        const content = fs.readFileSync(mdPath, 'utf-8')
        const titleMatch = content.match(/^---\s*\n[\s\S]*?title:\s*(.+)\n[\s\S]*?---/)
        if (titleMatch) title = titleMatch[1].trim()
      }

      return {
        text: title,
        link: `/${category}/${name}`
      }
    })

    return {
      text: category,
      collapsed: true,
      items
    }
  })
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

          // タイトル
          const title = hppToTitle(hppPath)
          before += `\n# ${title}\n`

          // Code セクション
          const source = readSourceCode(hppPath)
          if (source) {
            const githubUrl = `https://github.com/hashiryo/Library/blob/master/${hppPath}`
            before += '\n## Code\n\n'
            before += `[${hppPath}](${githubUrl})\n\n`
            before += '```cpp\n' + source + '\n```\n'
          }

          // Verified with セクション (マトリクス表コンポーネント)
          const tests = testMap[hppPath] || []
          if (tests.length > 0) {
            before += '\n## Verified with\n\n<VerifyMatrix />\n'
          }

          // --- md の後に挿入するセクション ---
          let after = ''

          // Depends on セクション (この hpp が依存している hpp)
          const deps = depGraph.dependsOn[hppPath] || []
          if (deps.length > 0) {
            after += '\n## Depends on\n\n'
            for (const dep of deps) {
              after += `- [${hppToTitle(dep)}](${hppToLink(dep)}) (${dep})\n`
            }
          }

          // Required by セクション (この hpp に依存している hpp)
          const reqBy = depGraph.requiredBy[hppPath] || []
          if (reqBy.length > 0) {
            after += '\n## Required by\n\n'
            for (const req of reqBy) {
              after += `- [${hppToTitle(req)}](${hppToLink(req)}) (${req})\n`
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
