import { defineConfig } from 'vitepress'
import fs from 'fs'
import path from 'path'
import { buildTestMap, readSourceCode } from './build-data'

const ROOT = path.resolve(__dirname, '../..')

// md/ ディレクトリからサイドバーを自動生成
function generateSidebar() {
  const mdDir = path.join(ROOT, 'md')
  const categories = fs.readdirSync(mdDir).filter(f =>
    fs.statSync(path.join(mdDir, f)).isDirectory()
  ).sort()

  return categories.map(category => {
    const categoryDir = path.join(mdDir, category)
    const files = fs.readdirSync(categoryDir)
      .filter(f => f.endsWith('.md'))
      .sort()

    const items = files.map(file => {
      const filePath = path.join(categoryDir, file)
      const content = fs.readFileSync(filePath, 'utf-8')
      const titleMatch = content.match(/^---\s*\n[\s\S]*?title:\s*(.+)\n[\s\S]*?---/)
      const title = titleMatch ? titleMatch[1].trim() : file.replace('.md', '')

      return {
        text: title,
        link: `/${category}/${file.replace('.md', '')}`
      }
    })

    return {
      text: category,
      collapsed: true,
      items
    }
  })
}

// ビルド時にテストマッピングを一度だけ構築
const testMap = buildTestMap()

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

          // Verified with セクション
          const tests = testMap[hppPath] || []
          let verifiedSection = ''
          if (tests.length > 0) {
            verifiedSection = '\n## Verified with\n\n'
            for (const test of tests) {
              const link = '/' + test.replace(/\.cpp$/, '')
              verifiedSection += `- ✅ [${test}](${link})\n`
            }
          }

          // Code セクション
          const source = readSourceCode(hppPath)
          let codeSection = ''
          if (source) {
            const githubUrl = `https://github.com/hashiryo/Library/blob/master/${hppPath}`
            codeSection = `\n## Code\n\n`
            codeSection += `[${hppPath}](${githubUrl})\n\n`
            codeSection += '```cpp\n' + source + '\n```\n'
          }

          if (!verifiedSection && !codeSection) return null

          // Markdown の末尾に追加
          return code + verifiedSection + codeSection
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
