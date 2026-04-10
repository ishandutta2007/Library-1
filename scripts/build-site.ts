/**
 * 自作 SSG ビルドスクリプト
 * VitePress を使わず、1ページずつ処理して OOM を回避する
 *
 * 実行: npx tsx scripts/build-site.ts
 */
import fs from 'fs'
import path from 'path'
import MarkdownIt from 'markdown-it'
import { katex } from '@mdit/plugin-katex'
import matter from 'gray-matter'
import { createHighlighter, type Highlighter } from 'shiki'
import katexLib from 'katex'
import { loadResults } from './lib/results'
import { buildDependencyGraph, buildTestMap, type DependencyGraph } from './lib/dependency-graph'
import { hppStatusIcon } from './lib/status'

// ============================================================
// 定数
// ============================================================

const ROOT = path.resolve(__dirname, '..')
const SRC_DIR = path.join(ROOT, 'src')
const TEST_DIR = path.join(ROOT, 'test')
const MD_DIR = path.join(ROOT, 'md')
const SITE_DIR = path.join(ROOT, 'site')
const OUT_DIR = path.join(SITE_DIR, 'Library')
const BASE_PATH = '/Library'

// ============================================================
// markdown-it 初期化
// ============================================================

let highlighter: Highlighter

async function initMarkdown() {
  highlighter = await createHighlighter({
    themes: ['github-light', 'github-dark'],
    langs: ['cpp'],
  })

  const md = MarkdownIt({ html: true })
  md.use(katex)

  // shiki によるコードブロックハイライト
  const defaultFence = md.renderer.rules.fence!
  md.renderer.rules.fence = (tokens, idx, options, env, self) => {
    const token = tokens[idx]
    const lang = token.info.trim()
    if (lang === 'cpp' || lang === 'c++') {
      return highlighter.codeToHtml(token.content, {
        lang: 'cpp',
        themes: { light: 'github-light', dark: 'github-dark' },
      })
    }
    return defaultFence(tokens, idx, options, env, self)
  }

  return md
}

// ============================================================
// 依存グラフ・テストマップ (build-data.ts と同等)
// ============================================================

// ============================================================
// HTML テンプレート
// ============================================================

/** テキスト内の $...$ をKaTeX HTMLに変換 */
function renderInlineKatex(text: string): string {
  return text.replace(/\$([^$]+)\$/g, (_, expr) => {
    try {
      return katexLib.renderToString(expr, { throwOnError: false })
    } catch {
      return escapeHtml(expr)
    }
  })
}

function escapeHtml(s: string): string {
  return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;')
}

function statusLabel(status: string): string {
  if (status === 'IGNORE') return '-'
  return status
}

function statusClass(status: string): string {
  if (status === 'AC') return 'status-ac'
  if (status === 'WA' || status === 'RE') return 'status-fail'
  if (status === 'TLE' || status === 'MLE') return 'status-warn'
  if (status === 'CE' || status === 'IGNORE') return 'status-gray'
  return ''
}


function formatMemory(kb: number): string {
  if (kb >= 1024) return (kb / 1024).toFixed(1) + ' MB'
  return kb + ' KB'
}

function renderVerifyMatrix(hppResults: any[]): string {
  if (!hppResults || hppResults.length === 0) return ''

  // 全環境名を収集
  const envNames = new Set<string>()
  for (const r of hppResults) {
    for (const name of Object.keys(r.environments || {})) envNames.add(name)
  }
  const envList = [...envNames].sort()
  if (envList.length === 0) return ''

  const collapsed = hppResults.length > 20
  let html = `<details${collapsed ? '' : ' open'}><summary>Verification Results (${hppResults.length} tests)</summary>\n`
  html += '<div class="table-wrapper"><table class="verify-matrix"><thead><tr><th>Test</th>'
  for (const env of envList) html += `<th colspan="3">${escapeHtml(env)}</th>`
  html += '</tr><tr><th></th>'
  for (const _ of envList) html += '<th>Status</th><th>Time</th><th>Memory</th>'
  html += '</tr></thead><tbody>\n'

  for (const r of hppResults) {
    const testName = r.file.replace(/^test\//, '').replace(/\.cpp$/, '')
    const testLink = `${BASE_PATH}/test/${testName}.html`
    html += `<tr><td class="test-name-cell"><div class="test-name-scroll"><a href="${testLink}">${escapeHtml(r.file)}</a></div></td>`
    for (const env of envList) {
      const e = r.environments?.[env]
      if (e) {
        const time = e.summary?.time_max_ms != null ? `${e.summary.time_max_ms} ms` : ''
        const mem = e.summary?.memory_max_kb ? formatMemory(e.summary.memory_max_kb) : ''
        html += `<td class="${statusClass(e.status)}">${statusLabel(e.status)}</td><td>${time}</td><td>${mem}</td>`
      } else {
        html += '<td>-</td><td>-</td><td>-</td>'
      }
    }
    html += '</tr>\n'
  }

  html += '</tbody></table></div></details>\n'
  return html
}

function renderResultTable(result: any): string {
  if (!result) return '<p class="no-results">結果データがありません</p>'

  const envs = result.environments || {}
  const envList = Object.keys(envs).sort()
  if (envList.length === 0) return '<p class="no-results">結果データがありません</p>'

  let html = ''

  // Problem link
  if (result.problem) {
    html += `<p>Problem: <a href="${escapeHtml(result.problem)}" target="_blank">${escapeHtml(result.problem)}</a>`
    if (result.time_limit_ms) html += ` (TL: ${result.time_limit_ms} ms)`
    html += '</p>\n'
  }

  // Summary table
  html += '<div class="table-wrapper"><table class="result-table"><thead><tr><th>Environment</th><th>Status</th><th>Time (max)</th><th>Time (total)</th><th>Memory (max)</th></tr></thead><tbody>\n'
  for (const env of envList) {
    const e = envs[env]
    const timeMax = e.summary?.time_max_ms ?? '-'
    const timeTotal = e.summary?.time_total_ms ?? '-'
    const memMax = e.summary?.memory_max_kb ? formatMemory(e.summary.memory_max_kb) : '-'
    html += `<tr><td>${escapeHtml(env)}</td><td class="${statusClass(e.status)}">${statusLabel(e.status)}</td><td>${timeMax} ms</td><td>${timeTotal} ms</td><td>${memMax}</td></tr>\n`
  }
  html += '</tbody></table></div>\n'

  // Test case details
  const firstEnv = envs[envList[0]]
  if (firstEnv?.cases?.length) {
    html += '<details><summary>テストケース詳細</summary>\n'
    html += '<div class="table-wrapper"><table class="result-table"><thead><tr><th>Case</th>'
    for (const env of envList) html += `<th colspan="3">${escapeHtml(env)}</th>`
    html += '</tr><tr><th></th>'
    for (const _ of envList) html += '<th>Status</th><th>Time</th><th>Memory</th>'
    html += '</tr></thead><tbody>\n'

    for (const c of firstEnv.cases) {
      html += `<tr><td>${escapeHtml(c.name)}</td>`
      for (const env of envList) {
        const eCase = envs[env]?.cases?.find((x: any) => x.name === c.name)
        if (eCase) {
          const mem = eCase.memory_kb ? formatMemory(eCase.memory_kb) : '-'
          html += `<td class="${statusClass(eCase.status)}">${statusLabel(eCase.status)}</td><td>${eCase.time_ms} ms</td><td>${mem}</td>`
        } else {
          html += '<td>-</td><td>-</td><td>-</td>'
        }
      }
      html += '</tr>\n'
    }
    html += '</tbody></table></div></details>\n'
  }

  return html
}

// ============================================================
// レイアウト
// ============================================================

function renderPage(title: string, content: string, sidebar: string): string {
  return `<!DOCTYPE html>
<html lang="ja">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>${escapeHtml(title)} | Hashiryo's Library</title>
  <link rel="stylesheet" href="${BASE_PATH}/assets/style.css">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/katex@0.16.11/dist/katex.min.css">
  <link rel="stylesheet" href="${BASE_PATH}/pagefind/pagefind-ui.css">
</head>
<body>
  <nav class="top-nav">
    <a href="${BASE_PATH}/" class="site-title">Hashiryo's Library</a>
    <div id="search"></div>
    <a href="https://github.com/hashiryo/Library">GitHub</a>
  </nav>
  <div class="layout">
    <aside class="sidebar">
      ${sidebar}
    </aside>
    <main class="content" data-pagefind-body>
      ${content}
    </main>
  </div>
  <script src="${BASE_PATH}/pagefind/pagefind-ui.js"></script>
  <script>
    new PagefindUI({ element: "#search", showSubResults: true, showImages: false });
    document.querySelectorAll('pre.shiki').forEach(pre => {
      const wrapper = document.createElement('div');
      wrapper.className = 'shiki-wrapper';
      pre.parentNode.insertBefore(wrapper, pre);
      wrapper.appendChild(pre);
      const btn = document.createElement('button');
      btn.className = 'copy-btn';
      btn.textContent = 'Copy';
      btn.addEventListener('click', () => {
        navigator.clipboard.writeText(pre.textContent).then(() => {
          btn.textContent = 'Copied!';
          setTimeout(() => btn.textContent = 'Copy', 2000);
        });
      });
      wrapper.appendChild(btn);
    });
    // Bundle toggle
    const toggle = document.querySelector('.code-toggle');
    if (toggle) {
      const original = document.getElementById('code-original');
      const bundled = document.getElementById('code-bundled');
      const bundleBtn = document.createElement('button');
      bundleBtn.className = 'bundle-btn';
      bundleBtn.textContent = 'Bundle';
      bundleBtn.addEventListener('click', () => {
        const showingOriginal = original.style.display !== 'none';
        original.style.display = showingOriginal ? 'none' : '';
        bundled.style.display = showingOriginal ? '' : 'none';
        bundleBtn.textContent = showingOriginal ? 'Original' : 'Bundle';
      });
      toggle.insertBefore(bundleBtn, toggle.firstChild);
    }
  </script>
</body>
</html>`
}

// ============================================================
// サイドバー生成
// ============================================================

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

function generateSidebar(testMap: Record<string, string[]>, resultsData: Record<string, any[]>): string {
  const categories = fs.readdirSync(SRC_DIR, { withFileTypes: true })
    .filter(e => e.isDirectory())

  interface SidebarItem {
    text: string
    icon?: string
    link?: string
    items?: SidebarItem[]
    order: number
  }

  function buildItems(srcPath: string, mdPath: string, linkPrefix: string, hppPrefix: string): SidebarItem[] {
    const entries = fs.readdirSync(srcPath, { withFileTypes: true })
    const items: SidebarItem[] = []

    for (const entry of entries) {
      if (!entry.isFile() || !entry.name.endsWith('.hpp')) continue
      const name = entry.name.replace(/\.hpp$/, '')
      const fm = readFrontmatter(path.join(mdPath, name + '.md'))
      const hppPath = `src/${hppPrefix}${entry.name}`
      const icon = hppStatusIcon(hppPath, testMap, resultsData)
      items.push({ text: fm.title || name, icon, link: `${BASE_PATH}${linkPrefix}/${name}.html`, order: fm.order ?? 999 })
    }

    for (const entry of entries) {
      if (!entry.isDirectory()) continue
      const subItems = buildItems(path.join(srcPath, entry.name), path.join(mdPath, entry.name), `${linkPrefix}/${entry.name}`, `${hppPrefix}${entry.name}/`)
      if (subItems.length > 0) {
        const indexFm = readFrontmatter(path.join(mdPath, entry.name, '_index.md'))
        items.push({ text: indexFm.title || entry.name, items: subItems, order: indexFm.order ?? 999 })
      }
    }

    items.sort((a, b) => a.order !== b.order ? a.order - b.order : a.text.localeCompare(b.text))
    return items
  }

  function renderItems(items: SidebarItem[]): string {
    let html = '<ul>'
    for (const item of items) {
      if (item.link) {
        const icon = item.icon ? `${item.icon} ` : ''
        html += `<li><a href="${item.link}">${icon}${renderInlineKatex(escapeHtml(item.text))}</a></li>`
      } else if (item.items) {
        html += `<li><details><summary>${renderInlineKatex(escapeHtml(item.text))}</summary>${renderItems(item.items)}</details></li>`
      }
    }
    html += '</ul>'
    return html
  }

  const allItems: SidebarItem[] = []
  for (const cat of categories) {
    const items = buildItems(path.join(SRC_DIR, cat.name), path.join(MD_DIR, cat.name), `/${cat.name}`, `${cat.name}/`)
    if (items.length > 0) {
      const indexFm = readFrontmatter(path.join(MD_DIR, cat.name, '_index.md'))
      allItems.push({ text: indexFm.title || cat.name, items, order: indexFm.order ?? 999 })
    }
  }
  allItems.sort((a, b) => a.order !== b.order ? a.order - b.order : a.text.localeCompare(b.text))

  return renderItems(allItems)
}

// ============================================================
// C++ #include 展開 (バンドル)
// ============================================================

/**
 * C++ ソースの #include "src/..." を再帰的に展開して1ファイルにまとめる
 * #pragma once や include guard による二重展開を防ぐ
 */
function bundleCpp(filePath: string): string {
  const included = new Set<string>()

  function expand(file: string): string {
    const absPath = path.resolve(ROOT, file)
    const realPath = fs.realpathSync(absPath)

    // 二重展開防止
    if (included.has(realPath)) return ''
    included.add(realPath)

    if (!fs.existsSync(absPath)) return `// [bundle error] file not found: ${file}\n`

    const lines = fs.readFileSync(absPath, 'utf-8').split('\n')
    const result: string[] = []

    for (const line of lines) {
      // #pragma once はスキップ（included セットで管理済み）
      if (line.trim() === '#pragma once') continue

      // #include "src/..." を展開
      const m = line.match(/^(\s*)#include\s+"(src\/[^"]+\.hpp)"/)
      if (m) {
        const includePath = m[2]
        result.push(`// --- ${includePath} ---`)
        result.push(expand(includePath))
        result.push(`// --- end ${includePath} ---`)
        continue
      }

      result.push(line)
    }

    return result.join('\n')
  }

  return expand(filePath)
}

// ============================================================
// hpp ドキュメントページ生成
// ============================================================

function generateHppPage(
  md: MarkdownIt,
  hppPath: string,
  mdPath: string,
  sidebar: string,
  depGraph: DependencyGraph,
  testMap: Record<string, string[]>,
  resultsData: Record<string, any[]>,
): void {
  const raw = fs.readFileSync(mdPath, 'utf-8')
  const { data: fm, content: mdContent } = matter(raw)

  const icon = hppStatusIcon(hppPath, testMap, resultsData)
  const title = fm.title || hppPath.split('/').pop()?.replace(/\.hpp$/, '') || hppPath

  // md コンテンツをレンダリング
  let body = `<h1>${icon} ${escapeHtml(title)}</h1>\n`

  // ソースコード (元 + バンドル切り替え)
  const sourcePath = path.join(ROOT, hppPath)
  if (fs.existsSync(sourcePath)) {
    const source = fs.readFileSync(sourcePath, 'utf-8')
    const githubUrl = `https://github.com/hashiryo/Library/blob/master/${hppPath}`
    body += `<h2>Code</h2>\n`
    body += `<details><summary>${escapeHtml(hppPath)}</summary>\n`
    body += `<p><a href="${githubUrl}">View on GitHub</a></p>\n`

    // 依存がある場合のみバンドルボタンを表示
    const hasDeps = (depGraph.dependsOn[hppPath] || []).length > 0
    if (hasDeps) {
      const bundled = bundleCpp(hppPath)
      body += `<div class="code-toggle">`
      body += `<div class="code-view active" id="code-original">`
      body += highlighter.codeToHtml(source, { lang: 'cpp', themes: { light: 'github-light', dark: 'github-dark' } })
      body += `</div>`
      body += `<div class="code-view" id="code-bundled" style="display:none">`
      body += highlighter.codeToHtml(bundled, { lang: 'cpp', themes: { light: 'github-light', dark: 'github-dark' } })
      body += `</div>`
      body += `</div>`
    } else {
      body += highlighter.codeToHtml(source, { lang: 'cpp', themes: { light: 'github-light', dark: 'github-dark' } })
    }
    body += `</details>\n`
  }

  // ユーザーの md コンテンツ
  if (mdContent.trim()) {
    body += md.render(mdContent)
  }

  // Verified with
  const hppResults = resultsData[hppPath] || []
  if (hppResults.length > 0) {
    body += '<h2>Verified with</h2>\n'
    body += renderVerifyMatrix(hppResults)
  }

  // Depends on
  const deps = depGraph.dependsOn[hppPath] || []
  if (deps.length > 0) {
    body += '<h2>Depends on</h2>\n<ul>\n'
    for (const dep of deps) {
      const depIcon = hppStatusIcon(dep, testMap, resultsData)
      const depTitle = readFrontmatter(path.join(MD_DIR, dep.replace(/^src\//, '').replace(/\.hpp$/, '.md'))).title
        || dep.split('/').pop()?.replace(/\.hpp$/, '') || dep
      const depLink = `${BASE_PATH}/${dep.replace(/^src\//, '').replace(/\.hpp$/, '.html')}`
      body += `<li>${depIcon} <a href="${depLink}">${escapeHtml(depTitle)}</a> (${escapeHtml(dep)})</li>\n`
    }
    body += '</ul>\n'
  }

  // Required by
  const reqBy = depGraph.requiredBy[hppPath] || []
  if (reqBy.length > 0) {
    body += '<h2>Required by</h2>\n<ul>\n'
    for (const req of reqBy) {
      const reqIcon = hppStatusIcon(req, testMap, resultsData)
      const reqTitle = readFrontmatter(path.join(MD_DIR, req.replace(/^src\//, '').replace(/\.hpp$/, '.md'))).title
        || req.split('/').pop()?.replace(/\.hpp$/, '') || req
      const reqLink = `${BASE_PATH}/${req.replace(/^src\//, '').replace(/\.hpp$/, '.html')}`
      body += `<li>${reqIcon} <a href="${reqLink}">${escapeHtml(reqTitle)}</a> (${escapeHtml(req)})</li>\n`
    }
    body += '</ul>\n'
  }

  // 出力
  const outRelPath = hppPath.replace(/^src\//, '').replace(/\.hpp$/, '.html')
  const outPath = path.join(OUT_DIR, outRelPath)
  fs.mkdirSync(path.dirname(outPath), { recursive: true })
  fs.writeFileSync(outPath, renderPage(title, body, sidebar))
}

// ============================================================
// テストファイルページ生成
// ============================================================

function generateTestPage(
  testFile: string,
  sidebar: string,
  testMap: Record<string, string[]>,
  resultsData: Record<string, any[]>,
): void {
  const source = fs.readFileSync(path.join(ROOT, testFile), 'utf-8')

  const problemMatch = source.match(/competitive-verifier:\s*PROBLEM\s+(\S+)/)
  const problem = problemMatch ? problemMatch[1] : null

  const directIncludes: string[] = []
  for (const m of source.matchAll(/#include\s+"(src\/[^"]+\.hpp)"/g)) directIncludes.push(m[1])

  const githubUrl = `https://github.com/hashiryo/Library/blob/master/${testFile}`

  let body = `<h1>${escapeHtml(testFile)}</h1>\n`
  body += '<ul>\n'
  body += `<li><a href="${githubUrl}" target="_blank">View this file on GitHub</a></li>\n`
  if (problem) body += `<li>Problem: <a href="${escapeHtml(problem)}" target="_blank">${escapeHtml(problem)}</a></li>\n`
  body += '</ul>\n'

  // Depends on
  if (directIncludes.length > 0) {
    body += '<h2>Depends on</h2>\n<ul>\n'
    for (const hpp of directIncludes) {
      const icon = hppStatusIcon(hpp, testMap, resultsData)
      const title = readFrontmatter(path.join(MD_DIR, hpp.replace(/^src\//, '').replace(/\.hpp$/, '.md'))).title
        || hpp.split('/').pop()?.replace(/\.hpp$/, '') || hpp
      const link = `${BASE_PATH}/${hpp.replace(/^src\//, '').replace(/\.hpp$/, '.html')}`
      body += `<li>${icon} <a href="${link}">${escapeHtml(title)}</a> (${escapeHtml(hpp)})</li>\n`
    }
    body += '</ul>\n'
  }

  // Test results
  body += '<h2>Test Results</h2>\n'
  let testResult: any = null
  for (const problems of Object.values(resultsData)) {
    for (const p of problems) {
      if (p.file === testFile) { testResult = p; break }
    }
    if (testResult) break
  }
  body += renderResultTable(testResult)

  // Source code
  body += '<h2>Code</h2>\n'
  body += highlighter.codeToHtml(source, { lang: 'cpp', themes: { light: 'github-light', dark: 'github-dark' } })

  // 出力
  const outRelPath = 'test/' + testFile.replace(/^test\//, '').replace(/\.cpp$/, '.html')
  const outPath = path.join(OUT_DIR, outRelPath)
  fs.mkdirSync(path.dirname(outPath), { recursive: true })
  fs.writeFileSync(outPath, renderPage(testFile, body, sidebar))
}

// ============================================================
// ホームページ生成
// ============================================================

function generateHomePage(sidebar: string): void {
  const body = `
<div class="hero">
  <h1>Hashiryo's Library</h1>
  <p>競技プログラミング用C++ライブラリ</p>
  <div class="hero-actions">
    <a href="${BASE_PATH}/DataStructure/BinaryIndexedTree.html" class="btn btn-primary">ライブラリ一覧</a>
    <a href="https://github.com/hashiryo/Library" class="btn">GitHub</a>
  </div>
</div>`
  const outPath = path.join(OUT_DIR, 'index.html')
  fs.mkdirSync(path.dirname(outPath), { recursive: true })
  fs.writeFileSync(outPath, renderPage("Hashiryo's Library", body, sidebar))
}

// ============================================================
// メイン
// ============================================================

async function main() {
  console.time('Total build')

  // 初期化
  const md = await initMarkdown()
  const depGraph = buildDependencyGraph()
  const testMap = buildTestMap(depGraph)
  const resultsData = loadResults()
  const sidebar = generateSidebar(testMap, resultsData)

  // 出力ディレクトリを準備
  if (fs.existsSync(SITE_DIR)) fs.rmSync(SITE_DIR, { recursive: true })
  fs.mkdirSync(OUT_DIR, { recursive: true })

  // 静的アセットをコピー
  const publicDir = path.join(MD_DIR, 'public')
  if (fs.existsSync(publicDir)) {
    function copyDir(src: string, dest: string) {
      fs.mkdirSync(dest, { recursive: true })
      for (const entry of fs.readdirSync(src, { withFileTypes: true })) {
        const s = path.join(src, entry.name)
        const d = path.join(dest, entry.name)
        if (entry.isDirectory()) copyDir(s, d)
        else fs.copyFileSync(s, d)
      }
    }
    copyDir(publicDir, OUT_DIR)
  }

  // CSS を生成
  fs.mkdirSync(path.join(OUT_DIR, 'assets'), { recursive: true })
  fs.writeFileSync(path.join(OUT_DIR, 'assets', 'style.css'), CSS)

  // ホームページ
  generateHomePage(sidebar)
  console.log('Generated: index.html')

  // hpp ドキュメントページ (1ページずつ処理)
  let hppCount = 0
  function scanHpp(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) { scanHpp(full); continue }
      if (!entry.name.endsWith('.hpp')) continue

      const hppPath = path.relative(ROOT, full) // "src/DataStructure/Foo.hpp"
      const mdRelPath = hppPath.replace(/^src\//, '').replace(/\.hpp$/, '.md')
      const mdPath = path.join(MD_DIR, mdRelPath)

      // md がなければスタブとして生成
      const mdExists = fs.existsSync(mdPath)
      if (!mdExists) {
        const name = entry.name.replace(/\.hpp$/, '')
        const stubMd = path.join(MD_DIR, mdRelPath)
        fs.mkdirSync(path.dirname(stubMd), { recursive: true })
        fs.writeFileSync(stubMd, `---\ntitle: ${name}\ndocumentation_of: ../../${hppPath}\n---\n`)
      }

      generateHppPage(md, hppPath, mdPath || path.join(MD_DIR, mdRelPath), sidebar, depGraph, testMap, resultsData)
      hppCount++
    }
  }
  scanHpp(SRC_DIR)
  console.log(`Generated: ${hppCount} hpp documentation pages`)

  // テストファイルページ (1ページずつ処理)
  let testCount = 0
  function scanTest(dir: string) {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name)
      if (entry.isDirectory()) { scanTest(full); continue }
      if (!entry.name.endsWith('.test.cpp')) continue

      const testFile = path.relative(ROOT, full)
      generateTestPage(testFile, sidebar, testMap, resultsData)
      testCount++
    }
  }
  scanTest(TEST_DIR)
  console.log(`Generated: ${testCount} test pages`)

  console.timeEnd('Total build')
}

// ============================================================
// CSS
// ============================================================

const CSS = `
:root {
  --c-bg: #fff;
  --c-bg-soft: #f6f6f7;
  --c-text: #213547;
  --c-text-2: #666;
  --c-brand: #3451b2;
  --c-divider: #e2e2e3;
  --c-ac: #22863a;
  --c-fail: #cb2431;
  --c-warn: #b08800;
  --c-gray: #6a737d;
}

@media (prefers-color-scheme: dark) {
  :root {
    --c-bg: #1a1a1a;
    --c-bg-soft: #252529;
    --c-text: #ddd;
    --c-text-2: #aaa;
    --c-brand: #6a9fff;
    --c-divider: #3a3a3c;
    --c-ac: #56d364;
    --c-fail: #f85149;
    --c-warn: #d29922;
    --c-gray: #8b949e;
  }
}

* { margin: 0; padding: 0; box-sizing: border-box; }

body {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
  color: var(--c-text);
  background: var(--c-bg);
  line-height: 1.7;
}

a { color: var(--c-brand); text-decoration: none; }
a:hover { text-decoration: underline; }

.top-nav {
  display: flex;
  align-items: center;
  gap: 1.5rem;
  padding: 0.75rem 1.5rem;
  border-bottom: 1px solid var(--c-divider);
  background: var(--c-bg);
  position: sticky;
  top: 0;
  z-index: 10;
}

.site-title { font-weight: 700; font-size: 1.1rem; white-space: nowrap; }

#search { flex: 1; max-width: 400px; position: relative; }
#search .pagefind-ui__form { position: relative; }
#search .pagefind-ui__drawer {
  position: absolute; top: 100%; left: 0; z-index: 100;
  background: var(--c-bg); border: 1px solid var(--c-divider); border-radius: 6px;
  box-shadow: 0 4px 16px rgba(0,0,0,0.15); max-height: 70vh; overflow-y: auto;
  margin-top: 0.25rem; width: max(100%, 500px);
}
#search .pagefind-ui__message, #search .pagefind-ui__results { padding: 0.5rem; font-size: 0.85rem; }
#search .pagefind-ui__result { padding: 0.5rem 0; border-bottom: 1px solid var(--c-divider); }
#search .pagefind-ui__result-title { font-size: 0.9rem; }
#search .pagefind-ui__result-excerpt { font-size: 0.8rem; color: var(--c-text-2); }

.layout {
  display: flex;
  max-width: 1400px;
  margin: 0 auto;
}

.sidebar {
  width: 280px;
  flex-shrink: 0;
  padding: 1rem;
  border-right: 1px solid var(--c-divider);
  height: calc(100vh - 49px);
  overflow-y: auto;
  position: sticky;
  top: 49px;
  font-size: 0.9rem;
}

.sidebar ul { list-style: none; padding-left: 1rem; }
.sidebar > ul { padding-left: 0; }
.sidebar li { margin: 0.15rem 0; }
.sidebar a { display: block; padding: 0.15rem 0; }
.sidebar summary { cursor: pointer; font-weight: 600; padding: 0.15rem 0; }

.content {
  flex: 1;
  min-width: 0;
  padding: 2rem;
}

.content h1 { font-size: 1.8rem; margin-bottom: 1rem; border-bottom: 1px solid var(--c-divider); padding-bottom: 0.5rem; }
.content h2 { font-size: 1.4rem; margin: 2rem 0 0.75rem; }
.content p { margin: 0.5rem 0; }
.content ul, .content ol { margin: 0.5rem 0; padding-left: 1.5rem; }
.content pre { margin: 1rem 0; border-radius: 6px; overflow-x: auto; }
.content code { font-family: 'SF Mono', Monaco, Consolas, monospace; font-size: 0.9em; }
.content img { max-width: 100%; }
.content table { border-collapse: collapse; margin: 0.5rem 0; font-size: 0.9rem; }
.content th, .content td { border: 1px solid var(--c-divider); padding: 0.4rem 0.75rem; }
.content th { background: var(--c-bg-soft); }

.table-wrapper { overflow-x: auto; margin: 0.5rem 0; }

.verify-matrix { font-size: 0.75rem; }
.verify-matrix td, .verify-matrix th { white-space: nowrap; padding: 0.25rem 0.5rem; }
.verify-matrix .test-name-cell { max-width: 280px; padding: 0; }
.verify-matrix .test-name-scroll { overflow-x: auto; padding: 0.25rem 0.5rem; white-space: nowrap; scrollbar-width: none; }
.verify-matrix .test-name-scroll::-webkit-scrollbar { display: none; }
.result-table { font-size: 0.85rem; }
.result-table td, .result-table th { white-space: nowrap; }
.result-table td { font-variant-numeric: tabular-nums; }
details .result-table { font-size: 0.75rem; }
details .result-table td, details .result-table th { padding: 0.25rem 0.5rem; }

.status-ac { color: var(--c-ac); }
.status-fail { color: var(--c-fail); }
.status-warn { color: var(--c-warn); }
.status-gray { color: var(--c-gray); }

.dot { font-size: 0.6em; vertical-align: middle; }
.dot-ac { color: var(--c-ac); }
.dot-fail { color: var(--c-fail); }
.dot-warn { color: var(--c-warn); }
.dot-gray { color: var(--c-gray); }

.no-results { color: var(--c-text-2); font-size: 0.9rem; }

.hero { text-align: center; padding: 4rem 1rem; }
.hero h1 { font-size: 2.5rem; border: none; }
.hero p { color: var(--c-text-2); font-size: 1.2rem; margin: 1rem 0 2rem; }
.hero-actions { display: flex; gap: 1rem; justify-content: center; }
.btn { padding: 0.6rem 1.5rem; border-radius: 6px; font-weight: 600; border: 1px solid var(--c-divider); }
.btn-primary { background: var(--c-brand); color: #fff; border-color: var(--c-brand); }

details { margin: 0.5rem 0; }
summary { cursor: pointer; }

/* shiki dual theme
   ライト: インラインの color/background-color をそのまま使う
   ダーク: CSS変数 --shiki-dark で上書き */
.shiki-wrapper { position: relative; }
.shiki-wrapper .copy-btn {
  position: absolute; top: 0.5rem; right: 0.5rem;
  padding: 0.25rem 0.5rem; border: 1px solid var(--c-divider); border-radius: 4px;
  background: var(--c-bg); color: var(--c-text-2); cursor: pointer; font-size: 0.75rem;
  opacity: 0; transition: opacity 0.2s;
}
.shiki-wrapper:hover .copy-btn { opacity: 1; }
.bundle-btn {
  display: inline-block; margin-bottom: 0.5rem;
  padding: 0.25rem 0.75rem; border: 1px solid var(--c-divider); border-radius: 4px;
  background: var(--c-bg-soft); color: var(--c-text-2); cursor: pointer; font-size: 0.8rem;
}
.bundle-btn:hover { border-color: var(--c-brand); color: var(--c-brand); }
.shiki { padding: 1rem; border-radius: 6px; overflow-x: auto; font-size: 0.85rem; background: #f6f8fa !important; border: 1px solid var(--c-divider); counter-reset: line; }
.shiki .line { display: inline-block; width: 100%; }
.shiki .line::before {
  content: counter(line); counter-increment: line;
  display: inline-block; width: 2.5em; margin-right: 1em;
  text-align: right; color: var(--c-text-2); opacity: 0.5;
  font-size: 0.8em; user-select: none;
}
@media (prefers-color-scheme: dark) {
  .shiki { background: #24292e !important; border-color: var(--c-divider); }
  .shiki, .shiki span { color: var(--shiki-dark) !important; }
}

@media (max-width: 768px) {
  .sidebar { display: none; }
  .content { padding: 1rem; }
}
`

main().catch(console.error)
