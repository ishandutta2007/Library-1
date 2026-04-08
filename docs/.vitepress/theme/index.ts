import DefaultTheme from 'vitepress/theme'
import 'katex/dist/katex.min.css'
import type { Theme } from 'vitepress'
import VerifyMatrix from '../components/VerifyMatrix.vue'
import TestFileResults from '../components/TestFileResults.vue'
import TestPage from '../components/TestPage.vue'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }) {
    app.component('VerifyMatrix', VerifyMatrix)
    app.component('TestFileResults', TestFileResults)
    app.component('TestPage', TestPage)
  }
} satisfies Theme
