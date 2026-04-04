<script setup lang="ts">
import { useData } from 'vitepress'
import { computed } from 'vue'

const { frontmatter } = useData()

const testFiles = computed(() => frontmatter.value.testFiles || [])

function testPageLink(testFile: string) {
  // test/yosupo/point_add_range_sum.BIT.test.cpp → /test/yosupo/point_add_range_sum.BIT.test
  return '/' + testFile.replace(/\.cpp$/, '')
}
</script>

<template>
  <div v-if="testFiles.length > 0" class="verified-with-section">
    <h2>Verified with</h2>
    <ul class="test-list">
      <li v-for="file in testFiles" :key="file">
        <span class="status-icon">✅</span>
        <a :href="testPageLink(file)">{{ file }}</a>
      </li>
    </ul>
  </div>
</template>

<style scoped>
.verified-with-section {
  margin-top: 2rem;
}

.test-list {
  list-style: none;
  padding: 0;
}

.test-list li {
  padding: 0.25rem 0;
  font-size: 0.9rem;
}

.status-icon {
  margin-right: 0.4rem;
}

.test-list a {
  color: var(--vp-c-brand-1);
  text-decoration: none;
  font-family: var(--vp-font-family-mono);
  font-size: 0.85rem;
}

.test-list a:hover {
  text-decoration: underline;
}
</style>
