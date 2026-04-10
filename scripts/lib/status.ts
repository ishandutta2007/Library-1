/**
 * hpp のステータス判定
 */

export function hppStatusIcon(hpp: string, testMap: Record<string, string[]>, resultsData: Record<string, any[]>): string {
  const tests = testMap[hpp]
  if (!tests || tests.length === 0) return '<span data-pagefind-ignore class="dot dot-gray">●</span>'
  const problems = resultsData[hpp]
  if (!problems || problems.length === 0) return '<span data-pagefind-ignore class="dot dot-gray">●</span>'
  let hasAC = false, hasFail = false
  for (const p of problems) {
    for (const [, env] of Object.entries(p.environments || {}) as [string, any][]) {
      if (env.status === 'AC' || env.status === 'IGNORE') hasAC = true
      else if (env.status) hasFail = true
    }
  }
  if (hasAC && !hasFail) return '<span data-pagefind-ignore class="dot dot-ac">●</span>'
  if (hasFail && !hasAC) return '<span data-pagefind-ignore class="dot dot-fail">●</span>'
  if (hasAC && hasFail) return '<span data-pagefind-ignore class="dot dot-warn">●</span>'
  return '<span data-pagefind-ignore class="dot dot-gray">●</span>'
}
