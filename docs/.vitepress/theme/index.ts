import DefaultTheme from 'vitepress/theme'
import type { Theme } from 'vitepress'
import VerifyMatrix from '../components/VerifyMatrix.vue'
import TestFileResults from '../components/TestFileResults.vue'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }) {
    app.component('VerifyMatrix', VerifyMatrix)
    app.component('TestFileResults', TestFileResults)
  }
} satisfies Theme
