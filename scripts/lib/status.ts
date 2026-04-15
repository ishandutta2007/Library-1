/**
 * hpp のステータス判定
 */
import type { CompactResults } from "./results";

export function hppStatusIcon(hpp: string, results: CompactResults): string {
  const files = results.hpp_map[hpp]
  if (!files || files.length === 0) return '<span data-pagefind-ignore class="dot dot-gray">●</span>'
  let hasAC = false, hasFail = false
  for (const file of files) {
    const test = results.tests[file]
    if (!test) continue
    for (const env of Object.values(test.environments)) {
      if (env.status === 'AC' || env.status === 'IGNORE') hasAC = true
      else if (env.status) hasFail = true
    }
  }
  if (hasAC && !hasFail) return '<span data-pagefind-ignore class="dot dot-ac">●</span>'
  if (hasFail && !hasAC) return '<span data-pagefind-ignore class="dot dot-fail">●</span>'
  if (hasAC && hasFail) return '<span data-pagefind-ignore class="dot dot-warn">●</span>'
  return '<span data-pagefind-ignore class="dot dot-gray">●</span>'
}
