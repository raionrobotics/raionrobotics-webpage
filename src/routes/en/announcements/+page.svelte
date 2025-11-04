<script>
	const newsList = [
		{
			title: "Notice of the 2-2 Extraordinary General Meeting of Shareholders",
			author: "Raion Robotics Inc.",
			date: "July 17, 2024",
			content: `
<b>Notice of the 2-2 Extraordinary General Meeting of Shareholders</b><br>
We hope our shareholders are keeping well.<br>
Pursuant to Article 363 of the Commercial Act and Article 27 of the Articles of Incorporation, we hereby convene the 2-2 Extraordinary General Meeting of Shareholders as follows.<br><br>
<b>- Agenda -</b><br><br>
1. Date: Thursday, August 1, 2024 at 10:00 a.m.<br><br>
2. Venue: Raion Robotics Inc. Conference Room,<br>
1F, 272-37 Munji-ro, Yuseong-gu, Daejeon, Korea<br><br>
3. Items to be discussed<br>
Item 1: Amendment to the Articles of Incorporation<br>
Item 2: Election of director(s)<br>
Item 3: Re-grant of stock options<br><br>
4. Documents to prepare<br>
- In person: ID card, notarised power of attorney, one seal certificate<br>
- By proxy: power of attorney (with seal certificate), proxy’s ID, notarised power of attorney<br><br>
July 17, 2024<br>
Raion Robotics Inc.<br>
			`
		}
	];
</script>

<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta name="media" content="media" />
</svelte:head>

<div class="section_title">
	<h1>Announcements
		<span>Information for Raion Robotics shareholders</span>
	</h1>
</div>

<div class="news-container">
	{#each newsList as news}
		<div class="news-card">
			<strong class="card-title">Title: {news.title}</strong>
			<p class="meta">Author: {news.author} | Date: {news.date}</p>
			<div class="content">
				{@html news.content}
			</div>
		</div>
	{/each}
</div>

<style>
	@import "../../../styles/main_style.css";

	.news-container {
		display: flex;
		flex-direction: column;
		align-items: center;
		padding: 1rem;
	}

	.news-card {
		width: 100%;
		max-width: 700px;
		background-color: #f5f5f5;
		border-radius: 12px;
		box-shadow: 0 4px 12px rgba(0, 0, 0, 0.08);
		padding: 2rem;
		margin-bottom: 2rem;
	}

	.card-title {
		display: block;
		font-size: 1.3rem;
		font-weight: bold;
		margin-bottom: 0.5rem;
		color: #222;
	}

	.meta {
		font-size: 0.9rem;
		color: #777;
		margin-bottom: 1rem;
	}

	.content {
		font-size: 1rem;
		line-height: 1.7;
		color: #444;
		white-space: normal;
		overflow: visible;
	}
</style>
