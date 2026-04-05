<script setup lang="ts">
import { useData } from 'vitepress'
import { computed } from 'vue'

const { frontmatter } = useData()

const sourceCodeHtml = computed(() => frontmatter.value.sourceCodeHtml || null)
const hppPath = computed(() => frontmatter.value.hppPath || null)
const githubUrl = computed(() => {
  if (!hppPath.value) return null
  return `https://github.com/hashiryo/Library/blob/master/${hppPath.value}`
})
</script>

<template>
  <div v-if="sourceCodeHtml" class="source-code-section">
    <h2>Code</h2>
    <p class="source-path">
      <a :href="githubUrl!" target="_blank" rel="noopener">{{ hppPath }}</a>
    </p>
    <div class="vp-code-block" v-html="sourceCodeHtml" />
  </div>
</template>

<style scoped>
.source-code-section {
  margin-top: 2rem;
}

.source-path {
  font-size: 0.9rem;
  margin-bottom: 0.5rem;
}

.source-path a {
  color: var(--vp-c-brand-1);
  text-decoration: none;
  font-family: var(--vp-font-family-mono);
  font-size: 0.85rem;
}

.source-path a:hover {
  text-decoration: underline;
}

.vp-code-block :deep(pre) {
  padding: 1rem;
  border-radius: 8px;
  overflow-x: auto;
  font-size: 0.875rem;
  line-height: 1.6;
}
</style>
