<script setup lang="ts">
import { ref, onMounted, computed } from 'vue'
import { useData } from 'vitepress'

interface TestCase {
  name: string
  status: string
  time_ms: number
  memory_kb: number
}

interface EnvResult {
  status: string
  summary: { time_max_ms: number; time_total_ms: number; memory_max_kb: number }
  cases: TestCase[]
}

interface ProblemResult {
  problem: string
  file: string
  time_limit_ms: number
  environments: Record<string, EnvResult>
}

type Results = Record<string, ProblemResult[]>

const { frontmatter } = useData()
const results = ref<ProblemResult[]>([])
const expandedProblems = ref<Set<string>>(new Set())

// documentation_of から対応する hpp パスを取得
const hppPath = computed(() => {
  const docOf = frontmatter.value.documentation_of
  if (!docOf) return null
  // "../../src/DataStructure/BinaryIndexedTree.hpp" → "src/DataStructure/BinaryIndexedTree.hpp"
  return docOf.replace(/^(\.\.\/)+/, '')
})

const envNames = computed(() => {
  if (results.value.length === 0) return []
  const first = results.value[0]
  return Object.keys(first.environments)
})

const envLabels: Record<string, string> = {
  'ubuntu-g++': 'Ubuntu g++',
  'ubuntu-clang++': 'Ubuntu clang++',
  'macos-g++': 'macOS g++',
  'macos-clang++': 'macOS clang++',
}

function statusIcon(status: string) {
  switch (status) {
    case 'AC': return '✅'
    case 'WA': return '❌'
    case 'TLE': return '⏰'
    case 'MLE': return '💾'
    case 'RE': return '💥'
    default: return '❓'
  }
}

function formatMemory(kb: number) {
  if (kb >= 1024) return `${(kb / 1024).toFixed(1)} MB`
  return `${kb} KB`
}

function problemName(url: string) {
  // URLから問題名を抽出
  const match = url.match(/\/problem\/(.+)$/) || url.match(/\/problems\/(.+)$/) || url.match(/\/p\/(.+)$/)
  return match ? match[1] : url.split('/').pop() || url
}

function toggleProblem(key: string) {
  if (expandedProblems.value.has(key)) {
    expandedProblems.value.delete(key)
  } else {
    expandedProblems.value.add(key)
  }
}

function caseNames(problem: ProblemResult) {
  const firstEnv = Object.values(problem.environments)[0]
  return firstEnv?.cases?.map(c => c.name) || []
}

onMounted(async () => {
  if (!hppPath.value) return
  try {
    const res = await fetch('/results.json')
    const data: Results = await res.json()
    results.value = data[hppPath.value] || []
  } catch {
    // results.json がない場合は何も表示しない
  }
})
</script>

<template>
  <div v-if="results.length > 0" class="verify-results">
    <h2>Verify Results</h2>

    <div v-for="problem in results" :key="problem.problem" class="problem-block">
      <h3 class="problem-title">
        <a :href="problem.problem" target="_blank" rel="noopener">{{ problemName(problem.problem) }}</a>
        <span v-if="problem.time_limit_ms" class="time-limit">TL: {{ problem.time_limit_ms }}ms</span>
        <span class="case-count">{{ caseNames(problem).length }} cases</span>
      </h3>

      <!-- 集約テーブル -->
      <table class="summary-table">
        <thead>
          <tr>
            <th>Environment</th>
            <th>Status</th>
            <th>Time (max / total)</th>
            <th>Memory (max)</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="env in envNames" :key="env">
            <td class="env-name">{{ envLabels[env] || env }}</td>
            <td class="status">{{ statusIcon(problem.environments[env]?.status) }} {{ problem.environments[env]?.status }}</td>
            <td class="time">
              {{ problem.environments[env]?.summary.time_max_ms }} ms / {{ problem.environments[env]?.summary.time_total_ms }} ms
            </td>
            <td class="memory">{{ formatMemory(problem.environments[env]?.summary.memory_max_kb) }}</td>
          </tr>
        </tbody>
      </table>

      <!-- 折りたたみ詳細 -->
      <details class="case-details" @toggle="toggleProblem(problem.problem)">
        <summary>テストケース詳細</summary>
        <div class="case-table-wrapper">
          <table class="case-table">
            <thead>
              <tr>
                <th>Case</th>
                <th v-for="env in envNames" :key="env" colspan="2">{{ envLabels[env] || env }}</th>
              </tr>
              <tr>
                <th></th>
                <template v-for="env in envNames" :key="env">
                  <th class="sub-header">Time</th>
                  <th class="sub-header">Mem</th>
                </template>
              </tr>
            </thead>
            <tbody>
              <tr v-for="caseName in caseNames(problem)" :key="caseName">
                <td class="case-name">{{ caseName }}</td>
                <template v-for="env in envNames" :key="env">
                  <td class="time" :class="{ 'is-max': problem.environments[env]?.cases.find(c => c.name === caseName)?.time_ms === problem.environments[env]?.summary.time_max_ms }">
                    {{ problem.environments[env]?.cases.find(c => c.name === caseName)?.time_ms }} ms
                  </td>
                  <td class="memory">
                    {{ formatMemory(problem.environments[env]?.cases.find(c => c.name === caseName)?.memory_kb || 0) }}
                  </td>
                </template>
              </tr>
            </tbody>
          </table>
        </div>
      </details>
    </div>
  </div>
</template>

<style scoped>
.verify-results {
  margin-top: 2rem;
}

.problem-block {
  margin-bottom: 2rem;
}

.problem-title {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  flex-wrap: wrap;
}

.problem-title a {
  color: var(--vp-c-brand-1);
  text-decoration: none;
}

.problem-title a:hover {
  text-decoration: underline;
}

.time-limit, .case-count {
  font-size: 0.8rem;
  font-weight: normal;
  color: var(--vp-c-text-2);
  background: var(--vp-c-bg-soft);
  padding: 0.15rem 0.5rem;
  border-radius: 4px;
}

.summary-table, .case-table {
  width: 100%;
  border-collapse: collapse;
  font-size: 0.9rem;
  margin: 0.5rem 0;
}

.summary-table th, .summary-table td,
.case-table th, .case-table td {
  border: 1px solid var(--vp-c-divider);
  padding: 0.4rem 0.6rem;
  text-align: left;
}

.summary-table thead, .case-table thead {
  background: var(--vp-c-bg-soft);
}

.sub-header {
  font-size: 0.75rem;
  font-weight: normal;
  color: var(--vp-c-text-2);
}

.env-name {
  font-weight: 500;
}

.status {
  white-space: nowrap;
}

.time {
  font-variant-numeric: tabular-nums;
  white-space: nowrap;
}

.time.is-max {
  font-weight: bold;
  color: var(--vp-c-warning-1);
}

.memory {
  font-variant-numeric: tabular-nums;
  white-space: nowrap;
}

.case-name {
  font-family: var(--vp-font-family-mono);
  font-size: 0.85rem;
}

.case-details {
  margin-top: 0.5rem;
}

.case-details summary {
  cursor: pointer;
  color: var(--vp-c-text-2);
  font-size: 0.9rem;
}

.case-details summary:hover {
  color: var(--vp-c-brand-1);
}

.case-table-wrapper {
  overflow-x: auto;
  margin-top: 0.5rem;
}
</style>
