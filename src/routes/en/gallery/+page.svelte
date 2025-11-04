<script>
</script>

<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta name="gallery" content="gallery" />
</svelte:head>

<div class="section_title">
	<h1>Gallery
		<span>Raion Robotics and our robots</span>
	</h1>
</div>

<div id="news" class="gallery">
	<iframe src="https://www.youtube.com/embed/EZbM594T3c4?si=fmYfR68oW-rNJMWQ" title="YouTube video player" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
	<iframe src="https://www.youtube.com/embed/P8Tu_BCYbEQ?si=adzYkuVwPgPpPiy_" title="YouTube video player" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
	<iframe src="https://www.youtube.com/embed/EUVoH-wA-lA?si=URuEtakNhaG-ryIS" title="YouTube video player" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
	<iframe src="https://www.youtube.com/embed/yV-tP3DHXes?si=63h6jaclVHazXDjg" title="YouTube video player" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
	<iframe src="https://www.youtube.com/embed/dt1u8zwUMok?si=PZ5ejWCwmTOcgtrL" title="YouTube video player" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
	<iframe src="https://www.youtube.com/embed/fO_PVr27QxU?si=GUmFv5RD6bYKk-6k" title="YouTube video player" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
	<iframe src="https://www.youtube.com/embed/f696z7Mx6Ls?si=53sZUsRoBDQ_JCiU" title="YouTube video player" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
</div>

<style>
	@import "../../../styles/main_style.css";

	.gallery {
		width: 80vw;
		margin: auto;
		display: grid;
		padding: 20px;
		grid-template-columns: repeat(auto-fit, 336px);
		grid-gap: 20px;
		justify-content: center;
  		align-items: center;
	}

	iframe {
		width: 336px;
		height: 189px;
	}
</style>
