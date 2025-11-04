<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta
		name="hiring"
		content="hiring"
	/>
</svelte:head>

<script>
	import { onMount } from 'svelte';
	import hiring_data from "$lib/components/hiring/hiring.json"
	
	let hiring_data_shown = []
	let currentItems = 0;

	addMoreNews();

	function addMoreNews() {
		currentItems += 6;
		for (let i = currentItems-6; i < Math.min(hiring_data.news.length, currentItems); i++) {
			hiring_data_shown.push(hiring_data.news[i]);
		}
		hiring_data_shown = hiring_data_shown;
	}	
</script>

<div class="section_title">
	<h1>채용공고
	  <span>라이온로보틱스와 함께 기술로 미래를 만들 분을 모십니다.</span>
	</h1>
</div>

<div id="hiring" class="hiring">	
	{#each hiring_data_shown as item}
		<div1>
			<div>
				<h1>{item.title}</h1>
				<h2><b>업무</b>: {item.duty}</h2>
				<h2><b>채용인원</b>: {item.number}</h2>
				<h2><b>자격조건</b>: {item.requirements}</h2>
				<h2><b>우대조건</b>: {item.advantages}</h2>
			</div>
			<div2>
				<a href="{item.link}" class="button-class">공고보기</a>
			</div2>
		</div1>
	{/each}
</div>

<div class="button-container">
	<div id="button" class="show-more-button">
		{#if currentItems < hiring_data.news.length}
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
@import "../../styles/main_style.css";

.hiring {
	width: 80vw;
	display: grid;
	margin: auto;
	padding: 1px;
  	font-family: "Noto Sans";
	grid-template-columns: repeat(auto-fit, 350px);
	grid-gap: 5px;
	justify-content: center;
	align-items: center;
	padding-bottom: 20px;
}

.hiring div {
	border: 2px solid #aaa;
	border-radius: 10px;
	padding: 5px;
	height: 400px;
	width: 350px;
	padding-bottom: 10px;
}

.hiring div2 {
  position: relative;
  left: 20px;
  bottom: 45px;
}

.hiring h1 {
  font-size: 24px;
  font-weight: 900;
  text-transform: uppercase;
  letter-spacing: 0px;
  font-family: "Noto Sans";
  line-height: 2em;
  padding-left: 0.25em;
  text-align: center;
  color: rgba(0, 0, 0, 1);
  padding-bottom: 10px;
}

.hiring h2 {
  font-size: 14px;
  font-weight: 500;
  text-transform: uppercase;
  letter-spacing: 0px;
  font-family: "Noto Sans";
  line-height: 2em;
  padding-left: 0.25em;
  text-align: left;
  color: rgba(0, 0, 0, 1);
  padding-bottom: 10px;
}

.button-class {
	font-family: "Noto Sans";
	color: white;
	font-weight: bold;
	font-size: large;
	border-radius: 15px;
	background-color: rgb(42, 42, 42);
	padding-top: 7px;
	padding-bottom: 7px;
	padding-right: 40px;
	padding-left: 40px;
	justify-content: center;
	bottom: 50px;
}

</style>
