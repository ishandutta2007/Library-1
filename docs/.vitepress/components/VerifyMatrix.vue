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

const hppPath = computed(() => {
  const docOf = frontmatter.value.documentation_of
  if (!docOf) return null
  return docOf.replace(/^(\.\.\/)+/, '')
})

const envNames = computed(() => {
  if (results.value.length === 0) return []
  return Object.keys(results.value[0].environments)
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
  if (kb >= 1024) return `${(kb / 1024).toFixed(1)}MB`
  return `${kb}KB`
}

function testPageLink(file: string) {
  return '/' + file.replace(/\.cpp$/, '')
}

function testName(problem: ProblemResult) {
  // ファイル名からテスト名を抽出
  const name = problem.file.split('/').pop()?.replace(/\.test\.cpp$/, '') || problem.file
  return name
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
  <div v-if="results.length > 0" class="verify-matrix">
    <div class="matrix-wrapper">
      <table class="matrix-table">
        <thead>
          <tr>
            <th class="test-col">Test</th>
            <th v-for="env in envNames" :key="env" class="env-col" colspan="1">
              {{ envLabels[env] || env }}
            </th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="problem in results" :key="problem.file">
            <td class="test-name">
              <a :href="testPageLink(problem.file)">{{ testName(problem) }}</a>
            </td>
            <td v-for="env in envNames" :key="env" class="env-cell">
              <template v-if="problem.environments[env]">
                <span class="status">{{ statusIcon(problem.environments[env].status) }}</span>
                <span class="time">{{ problem.environments[env].summary.time_max_ms }}ms</span>
                <span class="memory">{{ formatMemory(problem.environments[env].summary.memory_max_kb) }}</span>
              </template>
              <template v-else>
                <span class="status">❓</span>
              </template>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</template>

<style scoped>
.verify-matrix {
  margin: 0.5rem 0;
}

.matrix-wrapper {
  overflow-x: auto;
}

.matrix-table {
  width: 100%;
  border-collapse: collapse;
  font-size: 0.85rem;
}

.matrix-table th,
.matrix-table td {
  border: 1px solid var(--vp-c-divider);
  padding: 0.35rem 0.5rem;
  text-align: left;
  white-space: nowrap;
}

.matrix-table thead {
  background: var(--vp-c-bg-soft);
}

.test-col {
  min-width: 160px;
}

.env-col {
  text-align: center !important;
  font-size: 0.8rem;
}

.test-name {
  font-family: var(--vp-font-family-mono);
  font-size: 0.8rem;
}

.test-name a {
  color: var(--vp-c-brand-1);
  text-decoration: none;
}

.test-name a:hover {
  text-decoration: underline;
}

.env-cell {
  text-align: center !important;
  font-variant-numeric: tabular-nums;
}

.env-cell .status {
  margin-right: 0.2rem;
}

.env-cell .time {
  color: var(--vp-c-text-1);
  margin-right: 0.3rem;
}

.env-cell .memory {
  color: var(--vp-c-text-2);
  font-size: 0.75rem;
}
</style>
