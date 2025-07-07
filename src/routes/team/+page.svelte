<script>
	import { onMount } from 'svelte';

	onMount(() => {
		// 지도 관련 라이브러리 사용이 없으므로 이 스크립트 블록에는 특별히 추가할 내용이 없습니다.
		// 만약 다른 비동기 로직이 필요하다면 여기에 추가할 수 있습니다.
	});
</script>

<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta
		name="team"
		content="team"
	/>
</svelte:head>


<div class="section_title">
	<h1>회사소개
		<span>est. 2023</span>
	</h1>
</div>

<section id="hero" style="width: 100%; max-width: 1024px; margin-left: auto; margin-right: auto; margin-top: 50px;" class="font-noto font-bold block text-2xl">
	<div style="margin: 10px;">
		<p>
			바퀴가 가진 한계를 극복하고 계단, 산길 등의 험지를 자유롭게 누비는 자율 보행 로봇 솔루션을 개발합니다.
		</p>
	</div>
</section>

<section id="hero" style="width: 100%; max-width: 1024px; margin-left: auto; margin-right: auto; margin-top: 30px;" class="font-noto-black block text-xl">
	<div style="margin: 10px; line-height: 2.0;">
		<h1> 라이온로보틱스는 KAIST 기계공학과 RAILAB에서 스핀오프하여 2023년 10월에 창업한 기업으로,
			<b>세계 최고 수준의 보행 기술과 내재화된 로봇 설계 기술</b>을 바탕으로 사족 보행 로봇 라이보를 개발하였습니다.
			<b>인간이 갈 수 있는 길을 함께 가며 어렵고 번거로운 작업들을 대신할 로봇</b>을 만드는 것이 저희의 미션입니다.
			이로써 <b>부족해지는 사람의 노동력을 보완하고, 보다 안전한 사회를 만들어나가는 데에 기여하는 기업</b>이 되고자 합니다.
		</h1>
	</div>

	<div style="margin: 10px; line-height: 2.0;">
		<h1 style="font-weight: bold; margin-top: 20px; font-size: 24px;"> 연혁 </h1>
		<h1> [2023.10.] 법인 설립 (본사: 대전 유성구) </h1>
		<h1> [2023.10.] 2023 도전! K-스타트업 예비창업리그 왕중왕전 대상 수상 (국무총리 상) </h1>
		<h1> [2024.03.] 기업부설연구소 설립 </h1>
		<h1> [2024.04.] 시드투자 유치 (컴퍼니케이파트너스, 퓨쳐플레이) </h1>
		<h1> [2024.04.] 벤처기업 인증 </h1>
		<h1> [2024.11.] 라이보2 세계 최초 마라톤 풀코스 완주. 기록: 4시간 19분 52초 </h1>
	</div>
</section>

<hr/>

<section style="width: 60vw; margin: 50px auto;">
	<h2 style="font-weight: bold; margin-bottom: 10px; font-size: 24px; text-align: center;">서울 오피스</h2>
	<div style="position: relative; width: 100%; height: 0; padding-bottom: 56.25%;">
		<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d2685.0428849179393!2d127.03148057863716!3d37.499588246247356!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x357ca1b7e60f557d%3A0xe73f82339d58eeef!2z7JaR7ZmU67mM65Sp!5e0!3m2!1sen!2skr!4v1751923479331!5m2!1sen!2skr" width="100%" height="100%" style="border:0; position: absolute; top: 0; left: 0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade"></iframe>
	</div>
</section>

<section style="width: 60vw; margin: 50px auto;">
	<h2 style="font-weight: bold; margin-bottom: 10px; font-size: 24px; text-align: center;">대전 본사</h2>
	<div style="position: relative; width: 100%; height: 0; padding-bottom: 56.25%;">
		<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3211.736324027796!2d127.40649461342935!3d36.3913766722514!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x3565490038f58d51%3A0x4a2a61ffb6aba551!2z65287J207JioIOuhnOuztO2LseyKpA!5e0!3m2!1sen!2skr!4v1751923393503!5m2!1sen!2skr" width="100%" height="100%" style="border:0; position: absolute; top: 0; left: 0;" allowfullscreen="" loading="lazy" referrerpolicy="no-referrer-when-downgrade"></iframe>
	</div>
</section>


<style>
    @import "src/styles/main_style.css";

    .font-noto {
        font-family: "Noto Sans";
        color: rgb(12, 12, 105);
    }

    .font-noto-black {
        font-family: "Noto Sans";
        color: rgb(0, 0, 0);
    }
</style>