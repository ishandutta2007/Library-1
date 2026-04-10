/**
 * .verify-results/results.json の読み込み
 * コンパクト形式 → 従来形式 (hpp → テスト結果配列) に変換
 */
import fs from 'fs'
import path from 'path'

const ROOT = path.resolve(__dirname, '../..')

export function loadResults(): Record<string, any[]> {
  const resultsPath = path.join(ROOT, '.verify-results', 'results.json')
  if (!fs.existsSync(resultsPath)) return {}
  try {
    const raw = JSON.parse(fs.readFileSync(resultsPath, 'utf-8'))
    if (raw.tests && raw.hpp_map) {
      const data: Record<string, any[]> = {}
      for (const [hpp, files] of Object.entries(raw.hpp_map) as [string, string[]][]) {
        data[hpp] = files.map(f => ({
          file: f,
          problem: raw.tests[f]?.problem || '',
          time_limit_ms: raw.tests[f]?.time_limit_ms || 0,
          environments: raw.tests[f]?.environments || {},
        }))
      }
      return data
    }
    return raw
  } catch { return {} }
}
