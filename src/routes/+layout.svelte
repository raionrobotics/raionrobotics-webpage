<script lang="ts">
	import { base } from '$app/paths';
	import '../app.postcss';
	import Footer from '$lib/components/Footer.svelte';
	import Header from '$lib/components/Header.svelte';
</script>

<nav>
	<a href="{base}/">Home</a>
	<a href="{base}/about">About</a>
</nav>

<Header />
<main>
	<slot />
</main>
<Footer />
