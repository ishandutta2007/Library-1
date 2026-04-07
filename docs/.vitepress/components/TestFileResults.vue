<script setup lang="ts">
import ResultTable from './ResultTable.vue'

const props = defineProps<{
  data?: any
}>()
</script>

<template>
  <ResultTable v-if="data" :result="data" :show-problem-link="false" />
  <p v-else class="no-results">結果データがありません</p>
</template>

<style scoped>
.no-results {
  color: var(--vp-c-text-2);
  font-size: 0.9rem;
}
</style>
