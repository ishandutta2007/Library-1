import { defineConfig } from 'vitepress'
import fs from 'fs'
import path from 'path'
import { buildTestMap, buildDependencyGraph, readSourceCode } from './build-data'

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

  // 数式サポート
  markdown: {
    math: true
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

          let extra = ''

          // Depends on セクション (この hpp が依存している hpp)
          const deps = depGraph.dependsOn[hppPath] || []
          if (deps.length > 0) {
            extra += '\n## Depends on\n\n'
            for (const dep of deps) {
              extra += `- [${hppToTitle(dep)}](${hppToLink(dep)}) (${dep})\n`
            }
          }

          // Required by セクション (この hpp に依存している hpp)
          const reqBy = depGraph.requiredBy[hppPath] || []
          if (reqBy.length > 0) {
            extra += '\n## Required by\n\n'
            for (const req of reqBy) {
              extra += `- [${hppToTitle(req)}](${hppToLink(req)}) (${req})\n`
            }
          }

          // Verified with セクション (マトリクス表コンポーネント)
          const tests = testMap[hppPath] || []
          if (tests.length > 0) {
            extra += '\n## Verified with\n\n<VerifyMatrix />\n'
          }

          // Code セクション
          const source = readSourceCode(hppPath)
          if (source) {
            const githubUrl = `https://github.com/hashiryo/Library/blob/master/${hppPath}`
            extra += `\n## Code\n\n`
            extra += `[${hppPath}](${githubUrl})\n\n`
            extra += '```cpp\n' + source + '\n```\n'
          }

          if (!extra) return null

          return code + extra
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
