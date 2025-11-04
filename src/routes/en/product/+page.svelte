<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta name="Raibo2" content="Raibo2" />
</svelte:head>

<section class="product-hero">
	<div class="product-hero__background"></div>
	<div class="product-hero__media">
		<img src="/images/Raibo2.JPG" alt="Raibo 2 navigating challenging terrain" loading="lazy" />
	</div>
	<div class="product-hero__content page-shell">
		<span class="eyebrow">Product Lineup</span>
		<h1>Unlock every path with agile quadruped robotics</h1>
		<p>
			Raion Robotics delivers a family of legged robots engineered for complex field missions. The Raibo series
			combines all-terrain mobility, endurance, and precise control to help teams operate in places wheels cannot go.
		</p>
		<div class="hero-actions">
			<a class="btn-primary" href="/en/product/Raibo2">View specifications</a>
			<a class="btn-ghost" href="/en/media">Watch Raibo in action</a>
		</div>
	</div>
</section>

<section class="page-shell">
	<div class="card-grid">
		<a class="card" href="/en/product/Raibo2">
			<span class="pill">Flagship</span>
			<h2>Raibo 2</h2>
			<p>
				Our industrial-grade quadruped robot built for outdoor deployments. Superior terrain handling and smart
				payload integration for mission-critical operations.
			</p>
		</a>
		<a class="card" href="/en/product/Raibo1">
			<span class="pill">Research</span>
			<h2>Raibo 1</h2>
			<p>
				The internal research platform for locomotion control experiments. Lightweight, modular, and ideal for rapid
				iteration in labs.
			</p>
		</a>
		<a class="card" href="/en/product/RaiboOnWheel">
			<span class="pill">Inspection</span>
			<h2>Raiway</h2>
			<p>
				A hybrid wheel-leg inspection solution. Efficient for large facility patrols and data capture with adaptable
				mobility.
			</p>
		</a>
	</div>
</section>

<style>
	.product-hero {
		position: relative;
		margin: 2rem 0 3.5rem;
	}

	.product-hero__background {
		position: absolute;
		inset: 0 0 -50%;
		background: radial-gradient(circle at top left, rgba(55, 84, 255, 0.2), transparent 55%);
		z-index: -2;
	}

	.product-hero__media {
		position: relative;
		max-width: 1200px;
		margin: 0 auto;
		border-radius: 2rem;
		overflow: hidden;
		box-shadow: 0 30px 70px rgba(15, 23, 42, 0.3);
	}

	.product-hero__media::after {
		content: '';
		position: absolute;
		inset: 0;
		background: linear-gradient(110deg, rgba(2, 6, 23, 0.82), rgba(2, 6, 23, 0.1) 58%);
	}

	.product-hero__media img {
		width: 100%;
		height: clamp(340px, 45vw, 560px);
		object-fit: cover;
		display: block;
	}

	.product-hero__content {
		position: absolute;
		inset: 0;
		display: flex;
		flex-direction: column;
		justify-content: center;
		gap: 1.5rem;
		padding: clamp(2.5rem, 6vw, 4rem);
		color: #fff;
		max-width: min(650px, 90%);
	}

	.product-hero__content h1 {
		font-size: clamp(2.4rem, 4vw, 3.4rem);
		line-height: 1.15;
		margin: 0;
	}

	.product-hero__content p {
		line-height: 1.7;
		color: rgba(241, 245, 249, 0.86);
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
</style>
