<script>
	import { page } from '$app/stores';

	const isEnglishPath = (path) => path === '/en' || path.startsWith('/en/');

	const withLocale = (path, english) => {
		if (english) {
			if (path === '/') return '/en';
			return `/en${path}`;
		}
		return path;
	};

	$: english = isEnglishPath($page.url.pathname);
	$: teamHref = withLocale('/team', english);

	const info = english
		? {
				name: 'Raion Robotics Inc.',
				ceo: 'CEO: Jemin Hwangbo',
				address: '1F, 272-37 Munji-ro, Yuseong-gu, Daejeon, Republic of Korea',
				reg: 'Business Registration No.: 580-81-03287',
				sales: 'Sales',
				pr: 'PR',
				legal: 'Legal',
				privacy: 'Privacy Policy',
				terms: 'Terms & Conditions'
			}
		: {
				name: '(주)라이온로보틱스',
				ceo: '대표이사: 황보제민',
				address: '대전광역시 문지로 272-37(문지동) 1층',
				reg: '사업자등록번호: 580-81-03287',
				sales: '제품 문의',
				pr: '홍보 문의',
				legal: '법적 고지',
				privacy: '개인정보 처리방침',
				terms: '이용 약관'
			};
</script>

<footer class="footer-shell">
	<div class="footer-body page-shell">
		<div class="footer-grid">
			<div class="brand-block">
				<a href={teamHref} class="brand">
					<img src="/images/logo_f2-06.png" width="190" alt="Raion Robotics logo" />
				</a>
				<p class="brand-tagline">
					{english
						? 'Engineering the future of mobility with agile quadruped robotics.'
						: '민첩한 사족 보행 로봇 기술로 미래의 이동성을 설계합니다.'}
				</p>
			</div>

			<div class="info-block">
				<h3>{info.name}</h3>
				<ul>
					<li>{info.ceo}</li>
					<li>{info.address}</li>
					<li>{info.reg}</li>
					<li><a href="tel:0423852753">{english ? 'Tel: +82-42-385-2753' : '전화: 042-385-2753'}</a></li>
				</ul>
			</div>

			<div class="contact-block">
				<h3>{english ? 'Contact' : '문의'}</h3>
				<ul>
					<li>
						<span>{info.sales}:</span>
						<a href="mailto:sales@raionrobotics.com">sales@raionrobotics.com</a>
					</li>
					<li>
						<span>{info.pr}:</span>
						<a href="mailto:pr@raionrobotics.com">pr@raionrobotics.com</a>
					</li>
					<li class="legal-links">
						<a href="pp">{info.privacy}</a>
						<a href="tc">{info.terms}</a>
					</li>
				</ul>
			</div>
		</div>
		<div class="footer-meta">
			<p>© 2025 Raion Robotics. All rights reserved.</p>
		</div>
	</div>
</footer>

<style>
	.footer-shell {
		margin-top: 4rem;
		padding: 4.5rem 0 3rem;
		background: radial-gradient(circle at top right, rgba(55, 84, 255, 0.35), transparent 45%), #020617;
		color: rgba(255, 255, 255, 0.82);
		border-top-left-radius: 2.75rem;
		border-top-right-radius: 2.75rem;
		box-shadow: 0 -18px 50px rgba(15, 23, 42, 0.25);
	}

	.footer-body {
		display: flex;
		flex-direction: column;
		gap: 3rem;
	}

	.footer-grid {
		display: grid;
		gap: 2.5rem;
		grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
		align-items: start;
	}

	.brand {
		display: inline-flex;
		width: fit-content;
	}

	.brand-tagline {
		margin-top: 1rem;
		max-width: 300px;
		color: rgba(255, 255, 255, 0.65);
		font-size: 0.95rem;
		line-height: 1.6;
	}

	.info-block h3,
	.contact-block h3 {
		font-family: 'Noto Sans';
		font-size: 1rem;
		font-weight: 700;
		margin-bottom: 1rem;
		color: rgba(255, 255, 255, 0.92);
		text-transform: uppercase;
		letter-spacing: 0.08em;
	}

	.info-block ul,
	.contact-block ul {
		list-style: none;
		margin: 0;
		padding: 0;
		display: grid;
		gap: 0.55rem;
		font-size: 0.95rem;
	}

	.info-block li,
	.contact-block li {
		color: rgba(255, 255, 255, 0.7);
	}

	.contact-block span {
		display: inline-block;
		min-width: 92px;
		font-weight: 600;
	}

	a {
		color: rgba(255, 255, 255, 0.92);
		transition: opacity 0.2s ease;
	}

	a:hover,
	a:focus-visible {
		opacity: 0.75;
	}

	.legal-links {
		display: flex;
		gap: 1.25rem;
		margin-top: 0.5rem;
	}

	.footer-meta {
		display: flex;
		justify-content: center;
		font-size: 0.85rem;
		color: rgba(255, 255, 255, 0.55);
	}
</style>
