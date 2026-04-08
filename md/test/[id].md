---
title: "{{ $params.title }}"
---

<script setup>
import { useData } from 'vitepress'
const { params } = useData()
</script>

<TestPage v-bind="params" />
