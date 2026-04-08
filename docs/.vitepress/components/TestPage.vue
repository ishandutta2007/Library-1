<script setup lang="ts">
import TestFileResults from './TestFileResults.vue'

interface DependInfo {
  icon: string
  title: string
  link: string
  hpp: string
}

const props = withDefaults(defineProps<{
  id?: string
  title?: string
  githubUrl?: string
  problem?: string | null
  depends?: DependInfo[]
  testResult?: any
  source?: string
}>(), {
  id: '',
  title: '',
  githubUrl: '',
  problem: null,
  depends: () => [],
  testResult: null,
  source: '',
})
</script>

<template>
  <h1>{{ title }}</h1>

  <ul>
    <li><a :href="githubUrl" target="_blank">View this file on GitHub</a></li>
    <li v-if="problem">Problem: <a :href="problem" target="_blank">{{ problem }}</a></li>
  </ul>

  <div v-if="depends.length > 0">
    <h2>Depends on</h2>
    <ul>
      <li v-for="dep in depends" :key="dep.hpp">
        {{ dep.icon }} <a :href="dep.link">{{ dep.title }}</a> ({{ dep.hpp }})
      </li>
    </ul>
  </div>

  <h2>Test Results</h2>
  <TestFileResults v-if="testResult" :data="testResult" />
  <p v-else class="no-results">結果データがありません</p>

  <h2>Code</h2>
  <div class="language-cpp vp-adaptive-theme">
    <button title="Copy Code" class="copy"></button>
    <span class="lang">cpp</span>
    <pre><code>{{ source }}</code></pre>
  </div>
</template>

<style scoped>
.no-results {
  color: var(--vp-c-text-2);
  font-size: 0.9rem;
}
</style>
