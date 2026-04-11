/**
 * C++ バンドラー CLI
 * #include "mylib/..." を再帰的に展開して1ファイルにまとめる
 *
 * Usage: npx tsx scripts/bundle.ts <input.cpp> [-I <dir>] [--strip <pattern>...]
 *
 * Examples:
 *   npx tsx scripts/bundle.ts main.cpp
 *   npx tsx scripts/bundle.ts main.cpp -I ../Library
 *   npx tsx scripts/bundle.ts main.cpp --strip 'template.hpp'
 */
import fs from 'fs'
import path from 'path'

// --- CLI args ---

function parseArgs(args: string[]) {
  const includeDirs: string[] = []
  const stripPatterns: string[] = ['template.hpp']
  let inputFile = ''

  let i = 0
  while (i < args.length) {
    if (args[i] === '-I' && i + 1 < args.length) {
      includeDirs.push(args[++i])
    } else if (args[i] === '--strip' && i + 1 < args.length) {
      stripPatterns.push(args[++i])
    } else if (args[i] === '--no-default-strip') {
      stripPatterns.length = 0
    } else if (!args[i].startsWith('-')) {
      inputFile = args[i]
    }
    i++
  }

  if (!inputFile) {
    console.error('Usage: npx tsx scripts/bundle.ts <input.cpp> [-I <dir>] [--strip <pattern>...]')
    process.exit(1)
  }

  return { inputFile, includeDirs, stripPatterns }
}

// --- Bundler ---

function bundle(inputFile: string, includeDirs: string[], stripPatterns: string[]): string {
  const included = new Set<string>()

  function resolveInclude(includeArg: string, fromDir: string): string | null {
    // Try relative to the file first
    const relative = path.resolve(fromDir, includeArg)
    if (fs.existsSync(relative)) return fs.realpathSync(relative)

    // Try each -I directory
    for (const dir of includeDirs) {
      const resolved = path.resolve(dir, includeArg)
      if (fs.existsSync(resolved)) return fs.realpathSync(resolved)
    }

    return null
  }

  function shouldStrip(includePath: string): boolean {
    return stripPatterns.some(p => includePath.includes(p))
  }

  function expand(filePath: string): string {
    const realPath = fs.realpathSync(filePath)

    if (included.has(realPath)) return ''
    included.add(realPath)

    const lines = fs.readFileSync(filePath, 'utf-8').split('\n')
    const dir = path.dirname(filePath)
    const result: string[] = []

    for (const line of lines) {
      if (line.trim() === '#pragma once') continue

      // #include "..." (quoted includes only, not <system>)
      const m = line.match(/^\s*#include\s+"([^"]+)"/)
      if (m) {
        const target = m[1]

        if (shouldStrip(target)) continue

        const resolved = resolveInclude(target, dir)
        if (resolved) {
          result.push(expand(resolved))
          continue
        }
        // Not found — leave as-is (might be resolved by compiler)
      }

      result.push(line)
    }

    return result.join('\n')
  }

  const absInput = path.resolve(inputFile)
  if (!fs.existsSync(absInput)) {
    console.error(`File not found: ${inputFile}`)
    process.exit(1)
  }

  let output = expand(absInput)

  // Collapse multiple blank lines
  output = output.replace(/\n{3,}/g, '\n\n')

  return output
}

// --- Main ---

const { inputFile, includeDirs, stripPatterns } = parseArgs(process.argv.slice(2))
const result = bundle(inputFile, includeDirs, stripPatterns)
process.stdout.write(result)
