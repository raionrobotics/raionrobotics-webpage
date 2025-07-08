<script>
</script>

<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta
		name="home"
		content="home"
	/>
</svelte:head>

<!-- Optional: some overlay text to describe the video -->
<section class="video-wrapper">
	<video playsinline autoplay muted loop>
		<source src="/video/raibo_parkour.mp4" type="video/mp4">
		Your browser does not support the video tag.
	</video>
</section>

<style>
	video {
		position: relative;
		top: 0;
		left: 0;
		object-fit: contain; /* Changed from cover to contain */
		height: 100%;
		width: 100%;
		z-index: 0;
	}
	.video-wrapper {
		height: 100%;
		width: 100%;
		position: relative;
		overflow: hidden;
		text-align: center;
		display: flex;
		align-items: center;
		justify-content: center;
	}
</style>