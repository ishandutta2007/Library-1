import DefaultTheme from 'vitepress/theme'
import type { Theme } from 'vitepress'
import VerifyResults from '../components/VerifyResults.vue'
import TestFileResults from '../components/TestFileResults.vue'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }) {
    app.component('VerifyResults', VerifyResults)
    app.component('TestFileResults', TestFileResults)
  }
} satisfies Theme
