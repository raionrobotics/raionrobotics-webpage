<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta name="team" content="team" />
</svelte:head>

<section class="page-shell about-hero">
	<div class="about-copy">
		<span class="eyebrow">About Raion Robotics</span>
		<h1>We build robots that travel the hardest paths alongside people.</h1>
		<p>
			Raion Robotics spun out of the RAILAB at KAIST in 2023 to bring state-of-the-art locomotion research into the
			field. By combining world-class control algorithms with fully in-house hardware design, we deliver quadruped
			platforms that can operate where traditional automation cannot.
		</p>
	</div>
	<div class="about-highlight card">
		<h3>Our mission</h3>
		<ul>
			<li>Deploy autonomous robots to handle complex and hazardous site operations</li>
			<li>Provide reliable hardware and control software for extreme environments</li>
			<li>Enhance industrial safety and productivity through advanced robotics</li>
		</ul>
	</div>
</section>

<section class="page-shell">
	<div class="card-grid timeline-grid">
		<div class="card timeline-card">
			<span class="pill">2023.10</span>
			<h4>Incorporation</h4>
			<p>Established headquarters in Daejeon, expanding RAILAB research into industry-ready products.</p>
		</div>
		<div class="card timeline-card">
			<span class="pill">2023.10</span>
			<h4>K-Startup Grand Champion</h4>
			<p>Won the Prime Minister’s Award at Challenge! K-Startup 2023, recognising our technological potential.</p>
		</div>
		<div class="card timeline-card">
			<span class="pill">2024.03</span>
			<h4>Corporate R&D Center</h4>
			<p>Established an internal research organisation to accelerate core technology development.</p>
		</div>
		<div class="card timeline-card">
			<span class="pill">2024.04</span>
			<h4>Seed Investment</h4>
			<p>Secured funding from Company K Partners and FuturePlay to strengthen product development and production.</p>
		</div>
		<div class="card timeline-card">
			<span class="pill">2024.11</span>
			<h4>Marathon Milestone</h4>
			<p>Raibo 2 became the first quadruped robot to finish a full marathon without recharging (4h 19m 52s).</p>
		</div>
	</div>
</section>

<section class="office-section">
	<div class="office-card">
		<div class="office-info">
			<h3>Seoul Office</h3>
			<p>Our Seoul hub supports customer meetings, demos, and strategic partnerships.</p>
		</div>
		<div class="map-wrapper">
			<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d2685.0428849179393!2d127.03148057863716!3d37.499588246247356!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x357ca1b7e60f557d%3A0xe73f82339d58eeef!2z7JaR7ZmU67mM65Sp!5e0!3m2!1sen!2skr!4v1751923479331!5m2!1sen!2skr" width="100%" height="100%" style="border:0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade" title="Seoul office location"></iframe>
		</div>
	</div>
	<div class="office-card">
		<div class="office-info">
			<h3>Daejeon Headquarters</h3>
			<p>The heart of Raion Robotics, where research, development, and production come together.</p>
		</div>
		<div class="map-wrapper">
			<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3211.736324027796!2d127.40649461342935!3d36.3913766722514!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x3565490038f58d51%3A0x4a2a61ffb6aba551!2z65287J207JioIOuhnOuztO2LseyKpA!5e0!3m2!1sen!2skr!4v1751923393503!5m2!1sen!2skr" width="100%" height="100%" style="border:0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade" title="Daejeon HQ location"></iframe>
		</div>
	</div>
</section>

<style>
	@import "../../../styles/main_style.css";

	.about-hero {
		display: grid;
		gap: 2rem;
		margin: 3rem auto;
		align-items: start;
	}

	@media (min-width: 960px) {
		.about-hero {
			grid-template-columns: 1.2fr 0.8fr;
		}
	}

	.about-copy h1 {
		margin: 1rem 0 1.5rem;
		font-size: clamp(2.2rem, 3vw, 3rem);
	}

	.about-copy p {
		font-size: 1.05rem;
		line-height: 1.8;
		color: var(--text-secondary);
		margin: 0;
	}

	.about-highlight h3 {
		font-size: 1.1rem;
		margin-bottom: 1rem;
	}

	.about-highlight {
		padding: 2rem;
		min-height: 100%;
		display: flex;
		flex-direction: column;
		gap: 1.1rem;
	}

	.about-highlight ul {
		list-style: none;
		display: grid;
		gap: 0.9rem;
		padding: 0;
		margin: 0;
		color: var(--text-secondary);
		font-size: 1rem;
		line-height: 1.7;
	}

	.timeline-grid {
		margin: 2.5rem auto 3rem;
	}

	.timeline-card {
		display: flex;
		flex-direction: column;
		gap: 0.75rem;
	}

	.timeline-card h4 {
		margin: 0;
		font-size: 1.1rem;
	}

	.timeline-card p {
		margin: 0;
		font-size: 0.95rem;
		line-height: 1.6;
		color: var(--text-secondary);
	}

	.office-section {
		display: grid;
		gap: 2rem;
		margin: 0 auto 4rem;
		width: min(1100px, 92vw);
	}

	@media (min-width: 960px) {
		.office-section {
			grid-template-columns: repeat(2, minmax(0, 1fr));
		}
	}

	.office-card {
		display: grid;
		gap: 1rem;
		background: #fff;
		padding: 1.5rem;
		border-radius: 1.5rem;
		box-shadow: 0 25px 50px rgba(15, 23, 42, 0.14);
		border: 1px solid rgba(15, 23, 42, 0.06);
	}

	.office-info h3 {
		margin: 0 0 0.5rem;
		font-size: 1.2rem;
	}

	.office-info p {
		margin: 0;
		color: var(--text-secondary);
		line-height: 1.6;
	}

	.map-wrapper {
		position: relative;
		padding-bottom: 56.25%;
		border-radius: 1rem;
		overflow: hidden;
	}

	.map-wrapper iframe {
		position: absolute;
		inset: 0;
		width: 100%;
		height: 100%;
		border: 0;
	}
</style>
