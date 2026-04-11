/**
 * C++ バンドラー コアロジック
 * #include "..." を再帰的に展開して1ファイルにまとめる
 */
import fs from 'fs'
import path from 'path'

export interface BundleOptions {
  /** include 探索ディレクトリ (コンパイラの -I 相当) */
  includeDirs?: string[]
  /** この文字列を含む include は除去する */
  stripPatterns?: string[]
  /** 展開箇所に // --- path --- コメントを挿入する */
  commentMarkers?: boolean
  /** 連続空行を圧縮する */
  collapseBlankLines?: boolean
}

export function bundleCpp(filePath: string, options: BundleOptions = {}): string {
  const {
    includeDirs = [],
    stripPatterns = [],
    commentMarkers = false,
    collapseBlankLines = false,
  } = options

  const included = new Set<string>()

  function resolveInclude(includeArg: string, fromDir: string): string | null {
    // ファイルからの相対パス
    const relative = path.resolve(fromDir, includeArg)
    if (fs.existsSync(relative)) return fs.realpathSync(relative)

    // -I ディレクトリから探索
    for (const dir of includeDirs) {
      const resolved = path.resolve(dir, includeArg)
      if (fs.existsSync(resolved)) return fs.realpathSync(resolved)
    }

    return null
  }

  function shouldStrip(target: string): boolean {
    return stripPatterns.some(p => target.includes(p))
  }

  function expand(file: string): string {
    const realPath = fs.realpathSync(file)

    if (included.has(realPath)) return ''
    included.add(realPath)

    if (!fs.existsSync(file)) return `// [bundle error] file not found: ${file}\n`

    const lines = fs.readFileSync(file, 'utf-8').split('\n')
    const dir = path.dirname(file)
    const result: string[] = []

    for (const line of lines) {
      if (line.trim() === '#pragma once') continue

      const m = line.match(/^\s*#include\s+"([^"]+)"/)
      if (m) {
        const target = m[1]

        if (shouldStrip(target)) continue

        const resolved = resolveInclude(target, dir)
        if (resolved) {
          if (commentMarkers) result.push(`// --- ${target} ---`)
          result.push(expand(resolved))
          if (commentMarkers) result.push(`// --- end ${target} ---`)
          continue
        }
        // Not found — leave as-is
      }

      result.push(line)
    }

    return result.join('\n')
  }

  let output = expand(path.resolve(filePath))

  if (collapseBlankLines) {
    output = output.replace(/\n{3,}/g, '\n\n')
  }

  return output
}
