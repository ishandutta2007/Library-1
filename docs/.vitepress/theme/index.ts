import DefaultTheme from 'vitepress/theme'
import type { Theme } from 'vitepress'
import VerifyResults from '../components/VerifyResults.vue'
import TestFilePage from '../components/TestFilePage.vue'
import { h } from 'vue'

export default {
  extends: DefaultTheme,
  Layout() {
    return h(DefaultTheme.Layout, null, {
      'doc-after': () => h(VerifyResults)
    })
  },
  enhanceApp({ app }) {
    app.component('TestFilePage', TestFilePage)
  }
} satisfies Theme
