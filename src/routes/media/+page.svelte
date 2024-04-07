<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta
		name="media"
		content="media"
	/>
</svelte:head>

<script>
	import { onMount } from 'svelte';
	import news_data from "$lib/components/news/news.json"
	
	let news_data_shown = []
	let currentItems = 0;

	addMoreNews();

	function addMoreNews() {
		currentItems += 6;
		for (let i = currentItems-6; i < Math.min(news_data.news.length, currentItems); i++) {
			news_data_shown.push(news_data.news[i]);
		}
		news_data_shown = news_data_shown;
	}	
</script>

<div class="section_title">
	<h1>소식
	  <span>라이온로보틱스와 카이스트 라이랩 관련 소식</span>
	</h1>
</div>

<div id="news" class="news">	
	{#each news_data_shown as item}	
		<figure>
			<a href={item.url}>
				<img src={item.image} alt="news"/>
			</a>
			<figcaption class="font-noto">{item.text}</figcaption>
		</figure>
	{/each}
</div>

<div class="button-container">
	<div id="button" class="show-more-button">
		{#if currentItems < news_data.news.length}
		<button on:click={addMoreNews}
			id="loadmore"
			type="button"
			class="button-class"
			text-color="white"
			margin="auto"
			>더보기
		</button>
		{/if}
	</div><!--font-noto text-white bg-blue-400 hover:bg-blue-700 focus:ring-4 focus:outline-none focus:ring-purple-100 font-medium rounded-lg text-sm px-4 py-4 text-center mr-3 md:mr-0-->
</div>

<style>
	@import "src/styles/main_style.css";

	.news {
		width: 80vw;
		display: grid;
		margin: auto;
		padding: 10px;
		grid-template-columns: repeat(auto-fit, 320px);
		grid-gap: 10px;
		justify-content: center;
  		align-items: center;
	}

	.button-class {
		font-family: "Noto Sans";
		color: white;
		font-weight: bold;
		font-size: large;
		border-radius: 15px;
		background-color: rgb(15, 15, 97);
		padding-top: 7px;
		padding-bottom: 7px;
		padding-right: 40px;
		padding-left: 40px;
	}

	figure,
	img {
		width: 100%;
		aspect-ratio: 16/11;
		margin: 0;
	}

	.show-more-button {
		margin: auto;
  		position: relative;
		text-align: center;
	}

	.button-container { 
		height: 100px;
		position: relative;
		z-index: 20;
	}

	.font-noto {
		font-family: "Noto Sans";
	}
</style>