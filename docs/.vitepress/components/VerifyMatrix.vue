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
  'x64-g++': 'x64 g++',
  'x64-clang++': 'x64 clang++',
  'arm-g++': 'ARM g++',
  'arm-clang++': 'ARM clang++',
}

function statusLabel(status: string) {
  return status || '?'
}

function formatMemory(kb: number) {
  if (kb >= 1024) return `${(kb / 1024).toFixed(1)}MB`
  return `${kb}KB`
}

function testPageLink(file: string) {
  const base = import.meta.env.BASE_URL || '/'
  return base + file.replace(/\.cpp$/, '').replace(/^\//, '')
}

function testName(problem: ProblemResult) {
  // test/ を除いたパス（ディレクトリ付き）
  return problem.file.replace(/^test\//, '').replace(/\.test\.cpp$/, '')
}

onMounted(async () => {
  if (!hppPath.value) return
  try {
    const base = import.meta.env.BASE_URL || '/'
    const res = await fetch(`${base}results.json`)
    const data: Results = await res.json()
    results.value = data[hppPath.value] || []
  } catch {
    // results.json がない場合は何も表示しない
  }
})
</script>

<template>
  <div v-if="results.length > 0" class="verify-matrix">
    <details :open="results.length <= 20">
      <summary class="matrix-summary">
        {{ results.length }} tests ({{ results.filter(r => Object.values(r.environments).every(e => e.status === 'AC')).length }} AC)
      </summary>
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
              <td class="test-name" :title="problem.file">
                <a :href="testPageLink(problem.file)">{{ testName(problem) }}</a>
              </td>
            <td v-for="env in envNames" :key="env" class="env-cell">
              <template v-if="problem.environments[env]">
                <div class="env-line1">
                  <span class="status-label" :class="'status-' + problem.environments[env].status">{{ statusLabel(problem.environments[env].status) }}</span>
                  <span class="time">{{ problem.environments[env].summary.time_max_ms }}ms</span>
                </div>
                <div class="env-line2">
                  <span class="memory">{{ formatMemory(problem.environments[env].summary.memory_max_kb) }}</span>
                </div>
              </template>
              <template v-else>
                <span class="status-label status-unknown">?</span>
              </template>
            </td>
          </tr>
        </tbody>
        </table>
      </div>
    </details>
  </div>
</template>

<style scoped>
.verify-matrix {
  margin: 0.5rem 0;
}

.matrix-summary {
  cursor: pointer;
  color: var(--vp-c-text-2);
  font-size: 0.9rem;
  padding: 0.3rem 0;
}

.matrix-summary:hover {
  color: var(--vp-c-brand-1);
}

.matrix-wrapper {
  overflow-x: auto;
}

.matrix-table {
  min-width: 600px;
  border-collapse: collapse;
  font-size: 0.85rem;
  table-layout: fixed;
}

.matrix-table th,
.matrix-table td {
  border: 1px solid var(--vp-c-divider);
  padding: 0.35rem 0.5rem;
  text-align: left;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.matrix-table thead {
  background: var(--vp-c-bg-soft);
}

.test-col {
  width: 20%;
  max-width: 180px;
}

.env-col {
  text-align: center !important;
  font-size: 0.8rem;
}

.test-name {
  font-family: var(--vp-font-family-mono);
  font-size: 0.8rem;
  max-width: 180px;
  overflow-x: auto !important;
  overflow-y: hidden;
  text-overflow: clip !important;
  white-space: nowrap;
}

.test-name a {
  color: var(--vp-c-brand-1);
  text-decoration: none;
}

.test-name a:hover {
  text-decoration: underline;
}

.env-cell {
  font-variant-numeric: tabular-nums;
  text-align: right !important;
  white-space: normal !important;
}

.env-line1 {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.env-line2 {
  text-align: right;
}

.status-label {
  flex-shrink: 0;
  font-weight: 700;
  font-size: 0.75rem;
  padding: 0.1rem 0.3rem;
  border-radius: 3px;
}

.status-AC { color: #22863a; }
.status-WA { color: #cb2431; }
.status-TLE { color: #b08800; }
.status-MLE { color: #b08800; }
.status-RE { color: #cb2431; }
.status-CE { color: #6a737d; }
.status-IGNORE { color: #6a737d; }
.status-unknown { color: #6a737d; }

.env-cell .time {
  color: var(--vp-c-text-1);
}

.env-cell .memory {
  color: var(--vp-c-text-2);
  font-size: 0.75rem;
}
</style>
