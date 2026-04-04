<script setup lang="ts">
import { ref, onMounted, computed } from 'vue'
import { useData } from 'vitepress'
import ResultTable from './ResultTable.vue'

interface ProblemResult {
  problem: string
  file: string
  time_limit_ms: number
  environments: Record<string, any>
}

type Results = Record<string, ProblemResult[]>

const { frontmatter } = useData()
const results = ref<ProblemResult[]>([])

const hppPath = computed(() => {
  const docOf = frontmatter.value.documentation_of
  if (!docOf) return null
  return docOf.replace(/^(\.\.\/)+/, '')
})

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
    <ResultTable
      v-for="problem in results"
      :key="problem.problem"
      :result="problem"
      :show-problem-link="true"
    />
  </div>
  <p v-else-if="hppPath" class="no-results">結果データがありません</p>
</template>

<style scoped>
.no-results {
  color: var(--vp-c-text-2);
  font-size: 0.9rem;
}
</style>
