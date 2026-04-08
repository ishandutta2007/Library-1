<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { useData, withBase } from 'vitepress'
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
}>(), {
  id: '',
  title: '',
  githubUrl: '',
  problem: null,
  depends: () => [],
})

const testResult = ref<any>(null)
const source = ref<string>('')
const loading = ref(true)

onMounted(async () => {
  if (!props.id) return
  try {
    const res = await fetch(withBase(`/test-data/${props.id}.json`))
    if (res.ok) {
      const data = await res.json()
      testResult.value = data.testResult
      source.value = data.source
    }
  } catch {}
  loading.value = false
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
  <div v-if="loading" class="loading">読み込み中...</div>
  <TestFileResults v-else-if="testResult" :data="testResult" />
  <p v-else class="no-results">結果データがありません</p>

  <h2>Code</h2>
  <div v-if="loading" class="loading">読み込み中...</div>
  <div v-else class="language-cpp vp-adaptive-theme">
    <button title="Copy Code" class="copy"></button>
    <span class="lang">cpp</span>
    <pre><code>{{ source }}</code></pre>
  </div>
</template>

<style scoped>
.no-results,
.loading {
  color: var(--vp-c-text-2);
  font-size: 0.9rem;
}
</style>
