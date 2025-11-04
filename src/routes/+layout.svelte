<script>
	import '../app.postcss';
	import Footer from '$lib/components/Footer.svelte';
	import Header from '$lib/components/Header.svelte';
	import PopupManager from '$lib/components/PopupManager.svelte';
</script>

<PopupManager />

<Header />
<main>
	<slot />
</main>
<Footer />