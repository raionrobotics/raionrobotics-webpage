<script>
</script>

<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta name="home" content="home" />
</svelte:head>
<section class="hero">
	<video class="hero-video" playsinline autoplay muted loop>
		<source src="/video/raibo_parkour.mp4" type="video/mp4" />
		Your browser does not support the video tag.
	</video>
	<div class="hero-overlay"></div>
	<div class="hero-content page-shell">
		<span class="eyebrow">Quadruped Robotics</span>
		<h1>Raibo, the agile legged robot built for the real world</h1>
		<p>
			Raion Robotics delivers next-generation quadruped platforms that stay sure-footed on the toughest terrain.
			Deploy Raibo to take on demanding industrial inspections, logistics, and first-response operations.
		</p>
		<div class="hero-actions">
			<a class="btn-primary" href="/en/product/Raibo2">Explore products</a>
			<a class="btn-ghost" href="/en/documentation">View documentation</a>
		</div>
	</div>
</section>

<style>
	.hero {
		position: relative;
		border-radius: 2rem;
		margin: 2.5rem auto 3rem;
		min-height: clamp(420px, 60vw, 620px);
		overflow: hidden;
		box-shadow: 0 35px 80px rgba(15, 23, 42, 0.35);
		max-width: 1200px;
	}

	.hero-video {
		position: absolute;
		inset: 0;
		width: 100%;
		height: 100%;
		object-fit: cover;
		filter: saturate(1.15);
	}

	.hero-overlay {
		position: absolute;
		inset: 0;
		background: linear-gradient(120deg, rgba(3, 7, 18, 0.76), rgba(15, 23, 42, 0.15) 55%);
	}

	.hero-content {
		position: relative;
		z-index: 2;
		display: flex;
		flex-direction: column;
		gap: 1.5rem;
		padding: clamp(2.5rem, 6vw, 4.5rem);
		color: #fff;
	}

	.hero-content h1 {
		font-size: clamp(2.8rem, 4vw, 3.75rem);
		line-height: 1.1;
		max-width: 620px;
	}

	.hero-content p {
		max-width: 520px;
		font-size: 1.05rem;
		color: rgba(241, 245, 249, 0.88);
		line-height: 1.7;
	}

	.hero-actions {
		display: flex;
		flex-wrap: wrap;
		gap: 1rem;
	}

	.btn-primary,
	.btn-ghost {
		display: inline-flex;
		align-items: center;
		gap: 0.5rem;
		font-weight: 600;
		border-radius: 999px;
		padding: 0.75rem 1.6rem;
		font-size: 1rem;
		transition: transform 0.2s ease, box-shadow 0.2s ease, background 0.2s ease;
	}

	.btn-primary {
		background: linear-gradient(135deg, var(--primary-500), var(--primary-600));
		color: #fff;
		box-shadow: 0 20px 45px rgba(55, 84, 255, 0.35);
	}

	.btn-primary:hover,
	.btn-primary:focus-visible {
		transform: translateY(-2px);
		box-shadow: 0 24px 60px rgba(55, 84, 255, 0.4);
	}

	.btn-ghost {
		color: rgba(255, 255, 255, 0.85);
		border: 1px solid rgba(255, 255, 255, 0.45);
		background: rgba(255, 255, 255, 0.06);
	}

	.btn-ghost:hover,
	.btn-ghost:focus-visible {
		background: rgba(255, 255, 255, 0.18);
		transform: translateY(-2px);
	}

	@media (max-width: 768px) {
		.hero {
			border-radius: 1.5rem;
			margin: 1.5rem auto;
		}

		.hero-content {
			padding: 2.25rem;
		}
	}
</style>
