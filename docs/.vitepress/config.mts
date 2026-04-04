import { defineConfig } from 'vitepress'
import fs from 'fs'
import path from 'path'

// md/ ディレクトリからサイドバーを自動生成
function generateSidebar() {
  const mdDir = path.resolve(__dirname, '../../md')
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
