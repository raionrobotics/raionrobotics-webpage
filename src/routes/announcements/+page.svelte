<script>
	const newsList = [
		{
			title: "제2-2기 임시주주총회 소집통지",
			author: "㈜라이온로보틱스",
			date: "2024년 7월 17일",
			content: `
<b>제2-2기 임시주주총회 소집통지</b><br>
주주 여러분의 행복과 평안하심을 진심으로 기원합니다.<br>
상법 제363조와 당사 정관 제 27조에 의하여 제2-2기 임시주주총회를 아래와 같이 소집하오니 참석하여 주시기 바랍니다.<br><br>
-아 래-<br><br>
1. 일 시 : 2024년 08월 01일(목요일) 오전 10시 00분<br><br>
2. 장 소 : 대전광역시 유성구 문지로 272-37, 1층<br>
(주)라이온로보틱스 회의실<br><br>
3. 회의목적사항<br>
가. 부의안건<br>
제1호 의안 : 정관 변경의 건<br>
제2호 의안 : 이사 선임의 건<br>
제3호 의안 : 주식매수선택권 재부여<br><br>
4. 주주총회 참석 시 준비물<br>
- 직접행사 : 신분증, 공증위임장, 인감증명서1부<br>
- 대리행사 : 위임장(위임자 인감증명 첨부), 대리인의 신분증, 공증위임장<br><br>
2024년 07월 17일<br>
(주)라이온로보틱스
			`
		}
	];
</script>

<svelte:head>
	<title>Raion Robotics Inc.</title>
	<meta name="media" content="media" />
</svelte:head>

<!-- 기존 스타일 유지 -->
<div class="section_title">
	<h1>공고
		<span>(주)라이온로보틱스 주주를 위한 안내</span>
	</h1>
</div>

<!-- 카드 형태로 공고 출력 -->
<div class="news-container">
	{#each newsList as news}
		<div class="news-card">
			<strong class="card-title">제목 : {news.title}</strong>
			<p class="meta">작성자 : {news.author} | 등록일 : {news.date}</p>
			<div class="content">
				{@html news.content}
			</div>
		</div>
	{/each}
</div>

<style>
    @import "src/styles/main_style.css";

    .news-container {
        display: flex;
        flex-direction: column;
        align-items: center;
        padding: 1rem;
    }

    .news-card {
        width: 75vw;
        background-color: #f5f5f5; /* ✅ 연한 회색으로 수정 */
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
