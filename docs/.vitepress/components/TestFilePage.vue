<script setup lang="ts">
import { useData } from 'vitepress'
import { computed, ref, onMounted } from 'vue'
import ResultTable from './ResultTable.vue'

const { params } = useData()

const testFile = computed(() => params.value?.testFile || '')
const problem = computed(() => params.value?.problem || null)
const depends = computed(() => (params.value?.depends || []) as string[])
const source = computed(() => params.value?.source || '')

const testResult = ref<any>(null)
const highlightedSource = ref('')

const githubUrl = computed(() =>
  `https://github.com/hashiryo/Library/blob/master/${testFile.value}`
)

function hppToDocLink(hpp: string) {
  return '/' + hpp.replace(/^src\//, '').replace(/\.hpp$/, '')
}

function hppTitle(hpp: string) {
  return hpp.split('/').pop()?.replace(/\.hpp$/, '') || hpp
}

onMounted(async () => {
  // results.json からこのテストファイルの結果を検索
  try {
    const base = import.meta.env.BASE_URL || '/'
    const res = await fetch(`${base}results.json`)
    const data: Record<string, any[]> = await res.json()
    for (const problems of Object.values(data)) {
      for (const p of problems) {
        if (p.file === testFile.value) {
          testResult.value = p
          break
        }
      }
      if (testResult.value) break
    }
  } catch {
    // results.json がない場合は何も表示しない
  }

  // シンタックスハイライト
  try {
    const { codeToHtml } = await import('shiki')
    highlightedSource.value = await codeToHtml(source.value, {
      lang: 'cpp',
      themes: { light: 'github-light', dark: 'github-dark' }
    })
  } catch {
    highlightedSource.value = `<pre><code>${source.value}</code></pre>`
  }
})
</script>

<template>
  <div class="test-file-page">
    <h1>{{ testFile }}</h1>

    <ul class="meta-list">
      <li>
        <a :href="githubUrl" target="_blank" rel="noopener">View this file on GitHub</a>
      </li>
      <li v-if="problem">
        Problem: <a :href="problem" target="_blank" rel="noopener">{{ problem }}</a>
      </li>
    </ul>

    <div v-if="depends.length > 0" class="depends-section">
      <h2>Depends on</h2>
      <ul>
        <li v-for="hpp in depends" :key="hpp">
          <a :href="hppToDocLink(hpp)">{{ hppTitle(hpp) }}</a>
          <span class="hpp-path">({{ hpp }})</span>
        </li>
      </ul>
    </div>

    <div v-if="testResult" class="results-section">
      <h2>Test Results</h2>
      <ResultTable :result="testResult" :show-problem-link="false" />
    </div>

    <div class="code-section">
      <h2>Code</h2>
      <div v-if="highlightedSource" class="vp-code-block" v-html="highlightedSource" />
      <div v-else>
        <pre><code>{{ source }}</code></pre>
      </div>
    </div>
  </div>
</template>

<style scoped>
.test-file-page h1 {
  font-size: 1.5rem;
}

.meta-list {
  list-style: disc;
  padding-left: 1.5rem;
  margin: 1rem 0;
}

.meta-list li {
  margin: 0.3rem 0;
}

.meta-list a {
  color: var(--vp-c-brand-1);
  text-decoration: none;
}

.meta-list a:hover {
  text-decoration: underline;
}

.depends-section {
  margin-top: 1.5rem;
}

.depends-section a {
  color: var(--vp-c-brand-1);
  text-decoration: none;
  font-weight: 500;
}

.depends-section a:hover {
  text-decoration: underline;
}

.hpp-path {
  font-size: 0.8rem;
  color: var(--vp-c-text-2);
  margin-left: 0.3rem;
}

.results-section {
  margin-top: 1.5rem;
}

.code-section {
  margin-top: 1.5rem;
}

.vp-code-block :deep(pre) {
  padding: 1rem;
  border-radius: 8px;
  overflow-x: auto;
  font-size: 0.875rem;
  line-height: 1.6;
}
</style>
