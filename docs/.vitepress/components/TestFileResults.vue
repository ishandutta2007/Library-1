<script setup lang="ts">
import { ref, onMounted } from 'vue'
import ResultTable from './ResultTable.vue'

const props = defineProps<{
  testFile: string
}>()

const testResult = ref<any>(null)

onMounted(async () => {
  try {
    const base = import.meta.env.BASE_URL || '/'
    const res = await fetch(`${base}results.json`)
    const data: Record<string, any[]> = await res.json()
    for (const problems of Object.values(data)) {
      for (const p of problems) {
        if (p.file === props.testFile) {
          testResult.value = p
          return
        }
      }
    }
  } catch {
    // results.json がない場合は何も表示しない
  }
})
</script>

<template>
  <ResultTable v-if="testResult" :result="testResult" :show-problem-link="false" />
  <p v-else class="no-results">結果データがありません</p>
</template>

<style scoped>
.no-results {
  color: var(--vp-c-text-2);
  font-size: 0.9rem;
}
</style>
