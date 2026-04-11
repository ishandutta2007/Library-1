/**
 * C++ バンドラー CLI
 * #include "mylib/..." を再帰的に展開して1ファイルにまとめる
 *
 * Usage: npx tsx scripts/bundle.ts <input.cpp> [-I <dir>] [--strip <pattern>...]
 *
 * Examples:
 *   npx tsx scripts/bundle.ts main.cpp
 *   npx tsx scripts/bundle.ts main.cpp -I ../Library
 *   npx tsx scripts/bundle.ts main.cpp --strip 'some_header.hpp'
 */
import { bundleCpp } from './lib/bundle'

function parseArgs(args: string[]) {
  const includeDirs: string[] = []
  const stripPatterns: string[] = ['debug.hpp']
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

const { inputFile, includeDirs, stripPatterns } = parseArgs(process.argv.slice(2))
const result = bundleCpp(inputFile, {
  includeDirs,
  stripPatterns,
  collapseBlankLines: true,
})
process.stdout.write(result)
