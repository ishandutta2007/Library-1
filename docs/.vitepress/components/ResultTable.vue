<script setup lang="ts">
import { computed } from 'vue'

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

export interface ProblemResult {
  problem: string
  file: string
  time_limit_ms: number
  environments: Record<string, EnvResult>
}

const props = defineProps<{
  result: ProblemResult
  showProblemLink?: boolean
}>()

const envNames = computed(() => Object.keys(props.result.environments))

const envLabels: Record<string, string> = {
  'x64-g++': 'x64 g++',
  'x64-clang++': 'x64 clang++',
  'arm-g++': 'ARM g++',
  'arm-clang++': 'ARM clang++',
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
  const match = url.match(/\/problem\/(.+)$/) || url.match(/\/problems\/(.+)$/) || url.match(/\/p\/(.+)$/)
  return match ? match[1] : url.split('/').pop() || url
}

const caseNames = computed(() => {
  const firstEnv = Object.values(props.result.environments)[0]
  return firstEnv?.cases?.map(c => c.name) || []
})
</script>

<template>
  <div class="result-block">
    <h3 v-if="showProblemLink" class="problem-title">
      <a :href="result.problem" target="_blank" rel="noopener">{{ problemName(result.problem) }}</a>
      <span v-if="result.time_limit_ms" class="badge">TL: {{ result.time_limit_ms }}ms</span>
      <span class="badge">{{ caseNames.length }} cases</span>
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
          <td class="status">{{ statusIcon(result.environments[env]?.status) }} {{ result.environments[env]?.status }}</td>
          <td class="time">
            {{ result.environments[env]?.summary.time_max_ms }} ms / {{ result.environments[env]?.summary.time_total_ms }} ms
          </td>
          <td class="memory">{{ formatMemory(result.environments[env]?.summary.memory_max_kb) }}</td>
        </tr>
      </tbody>
    </table>

    <!-- 折りたたみ詳細 -->
    <details class="case-details">
      <summary>テストケース詳細</summary>
      <div class="case-table-wrapper">
        <table class="case-table">
          <thead>
            <tr>
              <th>Case</th>
              <th v-for="env in envNames" :key="env" colspan="3">{{ envLabels[env] || env }}</th>
            </tr>
            <tr>
              <th></th>
              <template v-for="env in envNames" :key="env">
                <th class="sub-header">Status</th>
                <th class="sub-header">Time</th>
                <th class="sub-header">Mem</th>
              </template>
            </tr>
          </thead>
          <tbody>
            <tr v-for="caseName in caseNames" :key="caseName">
              <td class="case-name">{{ caseName }}</td>
              <template v-for="env in envNames" :key="env">
                <td class="case-status" :class="'case-status-' + (result.environments[env]?.cases?.find(c => c.name === caseName)?.status || '')">
                  {{ result.environments[env]?.cases?.find(c => c.name === caseName)?.status || '' }}
                </td>
                <td class="time" :class="{ 'is-max': result.environments[env]?.cases?.find(c => c.name === caseName)?.time_ms === result.environments[env]?.summary.time_max_ms }">
                  {{ result.environments[env]?.cases?.find(c => c.name === caseName)?.time_ms }} ms
                </td>
                <td class="memory">
                  {{ formatMemory(result.environments[env]?.cases?.find(c => c.name === caseName)?.memory_kb || 0) }}
                </td>
              </template>
            </tr>
          </tbody>
        </table>
      </div>
    </details>
  </div>
</template>

<style scoped>
.result-block {
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

.badge {
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

.case-status {
  font-weight: 700;
  font-size: 0.8rem;
  white-space: nowrap;
}

.case-status-AC { color: #22863a; }
.case-status-WA { color: #cb2431; }
.case-status-TLE { color: #b08800; }
.case-status-MLE { color: #b08800; }
.case-status-RE { color: #cb2431; }
</style>
