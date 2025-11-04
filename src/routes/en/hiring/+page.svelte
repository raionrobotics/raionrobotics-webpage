<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta name="hiring" content="hiring" />
</svelte:head>

<script>
	import hiring_data from "$lib/components/hiring/hiring.en.json";

	let hiring_data_shown = [];
	let currentItems = 0;

	addMore();

	function addMore() {
		currentItems += 6;
		for (let i = currentItems - 6; i < Math.min(hiring_data.news.length, currentItems); i++) {
			hiring_data_shown.push(hiring_data.news[i]);
		}
		hiring_data_shown = hiring_data_shown;
	}
</script>

<div class="section_title">
	<h1>Careers
		<span>Join Raion Robotics and help build the future with technology.</span>
	</h1>
</div>

<section class="page-shell hiring-section">
	<div class="hiring-intro card">
		<h2>Let’s build robots that change how the world moves.</h2>
		<p>
			We are engineers and makers who thrive on solving tough, real-world problems. If you enjoy rapid iteration and
			field-ready innovation, we’d love to meet you.
		</p>
		<a class="cta-link" href="mailto:hr@raionrobotics.com">Email our recruiting team →</a>
	</div>

	<div class="hiring-grid">
		{#each hiring_data_shown as item}
			<article class="hiring-card">
				<header>
					<span class="pill">Open role</span>
					<h3>{item.title}</h3>
					<p class="openings">{item.number}</p>
				</header>
				<ul class="detail-list">
					<li>
						<strong>What you’ll do</strong>
						<p>{item.duty}</p>
					</li>
					<li>
						<strong>What we’re looking for</strong>
						<p>{item.requirements}</p>
					</li>
					<li>
						<strong>Nice to have</strong>
						<p>{item.advantages}</p>
					</li>
				</ul>
				<a href={item.link} class="btn-outline" target="_blank" rel="noopener noreferrer">View full posting</a>
			</article>
		{/each}
	</div>

	{#if currentItems < hiring_data.news.length}
		<div class="load-more">
			<button on:click={addMore} type="button" class="btn-primary ghost">Show more roles</button>
		</div>
	{/if}
</section>

<style>
	@import "../../../styles/main_style.css";

	.hiring-section {
		display: grid;
		gap: 2.5rem;
	}

	.hiring-intro h2 {
		margin: 0 0 1rem;
		font-size: clamp(1.6rem, 2.4vw, 2.1rem);
	}

	.hiring-intro p {
		margin: 0;
		color: var(--text-secondary);
		line-height: 1.7;
	}

	.cta-link {
		display: inline-flex;
		align-items: center;
		margin-top: 1.5rem;
		font-weight: 600;
		color: var(--primary-600);
	}

	.cta-link:hover {
		color: var(--primary-500);
	}

	.hiring-grid {
		display: grid;
		gap: 1.75rem;
		grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
	}

	.hiring-card {
		display: flex;
		flex-direction: column;
		gap: 1.25rem;
		background: #fff;
		border-radius: 1.5rem;
		padding: 1.75rem;
		box-shadow: 0 25px 50px rgba(15, 23, 42, 0.16);
		border: 1px solid rgba(15, 23, 42, 0.05);
	}

	.hiring-card header {
		display: flex;
		flex-direction: column;
		gap: 0.5rem;
	}

	.hiring-card h3 {
		margin: 0;
		font-size: 1.25rem;
	}

	.openings {
		margin: 0;
		color: var(--primary-600);
		font-weight: 600;
	}

	.detail-list {
		list-style: none;
		display: grid;
		gap: 1rem;
		margin: 0;
		padding: 0;
		color: var(--text-secondary);
		font-size: 0.95rem;
	}

	.detail-list strong {
		display: block;
		font-size: 0.85rem;
		text-transform: uppercase;
		letter-spacing: 0.08em;
		color: var(--text-primary);
		margin-bottom: 0.3rem;
	}

	.btn-outline {
		display: inline-flex;
		align-items: center;
		justify-content: center;
		gap: 0.4rem;
		font-weight: 600;
		border-radius: 999px;
		padding: 0.65rem 1.4rem;
		font-size: 0.95rem;
		border: 1px solid var(--primary-500);
		color: var(--primary-600);
		transition: background 0.2s ease, color 0.2s ease, transform 0.2s ease;
	}

	.btn-outline:hover,
	.btn-outline:focus-visible {
		background: var(--primary-100);
		transform: translateY(-1px);
	}

	.load-more {
		display: flex;
		justify-content: center;
	}

	.btn-primary.ghost {
		background: transparent;
		border: 1px solid rgba(55, 84, 255, 0.35);
		color: var(--primary-600);
		box-shadow: none;
	}

	.btn-primary.ghost:hover {
		background: rgba(55, 84, 255, 0.12);
		transform: translateY(-1px);
	}
</style>
