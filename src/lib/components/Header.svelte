<script>
	import Menu from './Menu.svelte';
	import MenuItem from './MenuItem.svelte';
	import '/src/styles/app.css';
	import { page } from '$app/stores';
	import { goto } from '$app/navigation';

	const isEnglishPath = (path) => path === '/en' || path.startsWith('/en/');

	const addEnglishPrefix = (path) => {
		if (path === '/en' || path.startsWith('/en/')) return path;
		if (path === '/') return '/en';
		return `/en${path}`;
	};

	const removeEnglishPrefix = (path) => {
		if (path === '/en') return '/';
		if (path.startsWith('/en/')) return path.slice(3);
		return path;
	};

	$: currentPath = $page.url.pathname;
	$: isEnglish = isEnglishPath(currentPath);
	$: koreanPath = removeEnglishPrefix(currentPath) || '/';
	$: toggleHref = isEnglish ? koreanPath : addEnglishPrefix(currentPath);

	$: labels = isEnglish
		? {
				home: 'Home',
				about: 'About',
				products: 'Products',
				raibo1: 'Raibo 1',
				raibo2: 'Raibo 2',
				raiway: 'Raibo with wheels',
				news: 'News',
				gallery: 'Gallery',
				careers: 'Careers',
				notices: 'Notices',
				doc: 'Docs',
				switchLabel: '한국어'
			}
		: {
				home: '홈',
				about: '회사소개',
				products: '제품',
				raibo1: '라이보1',
				raibo2: '라이보2',
				raiway: '라이보 with wheels',
				news: '뉴스',
				gallery: '갤러리',
				careers: '채용',
				notices: '공고',
				doc: '문서',
				switchLabel: 'English'
			};

	const localePrefix = () => (isEnglish ? '/en' : '');

	const buildHref = (path) => {
		const prefix = localePrefix();
		if (!prefix) return path;
		if (path === '/') return prefix;
		return `${prefix}${path}`;
	};

	const handleNav = (path, event) => {
		event?.preventDefault();
		let target = path;
		if (path.startsWith('/en') || path.startsWith('http')) {
			target = path;
		} else if (path.startsWith('/')) {
			target = buildHref(path);
		}
		goto(target);
	};

	const navItems = [
		{ href: '/', labelKey: 'home' },
		{ href: '/team', labelKey: 'about' },
		{ href: '/media', labelKey: 'news' },
		{ href: '/gallery', labelKey: 'gallery' },
		{ href: '/hiring', labelKey: 'careers' },
		{ href: '/announcements', labelKey: 'notices' },
		{ href: '/documentation', labelKey: 'doc' }
	];
</script>

<header class="sticky top-0 z-40">
	<nav class="backdrop-blur-xl bg-white/85 border-b border-slate-200/80 shadow-sm">
		<div class="max-w-6xl m-auto flex items-center justify-between px-5 sm:px-8 py-4 gap-6">
			<a href={isEnglish ? '/en' : '/'} class="flex items-center gap-3">
				<img src="/images/logo_f2-02.png" width="160" alt="Raion Robotics logo" class="drop-shadow-sm" />
			</a>

			<div class="hidden lg:flex items-center gap-6 xl:gap-8">
				{#each navItems as item}
					<a
						href={buildHref(item.href)}
						class="nav-link {currentPath === buildHref(item.href) ? 'active' : ''}"
						on:click={(event) => handleNav(item.href, event)}>
						{labels[item.labelKey]}
					</a>
				{/each}

				<Menu>
					<span slot="toggle">{labels.products}</span>
					<MenuItem>
						<a href={buildHref('/product/Raibo1')} on:click={(event) => handleNav('/product/Raibo1', event)}>{labels.raibo1}</a>
					</MenuItem>
					<MenuItem>
						<a href={buildHref('/product/Raibo2')} on:click={(event) => handleNav('/product/Raibo2', event)}>{labels.raibo2}</a>
					</MenuItem>
					<MenuItem>
						<a
							href={buildHref('/product/RaiboOnWheel')}
							on:click={(event) => handleNav('/product/RaiboOnWheel', event)}>{labels.raiway}</a>
					</MenuItem>
				</Menu>
			</div>

			<div class="flex items-center gap-3">
				<a
					class="lang-switch"
					href={toggleHref}
					on:click={(event) => {
						event.preventDefault();
						goto(toggleHref);
					}}>
					{labels.switchLabel}
				</a>
			</div>
		</div>
	</nav>
</header>

<style>
	.nav-link {
		position: relative;
		display: inline-flex;
		align-items: center;
		gap: 0.35rem;
		padding: 0.35rem 0;
		font-family: 'Noto Sans';
		font-size: 0.95rem;
		font-weight: 600;
		color: var(--text-secondary);
		transition: color 0.2s ease;
	}

	.nav-link::after {
		content: '';
		position: absolute;
		left: 0;
		bottom: -0.35rem;
		width: 100%;
		height: 3px;
		border-radius: 999px;
		background: var(--primary-500);
		opacity: 0;
		transform: translateY(6px);
		transition: opacity 0.2s ease, transform 0.2s ease;
	}

	.nav-link:hover,
	.nav-link:focus-visible {
		color: var(--primary-500);
	}

	.nav-link:hover::after,
	.nav-link:focus-visible::after,
	.nav-link.active::after {
		opacity: 1;
		transform: translateY(0);
	}

	.nav-link.active {
		color: var(--primary-500);
	}

	.lang-switch {
		display: inline-flex;
		align-items: center;
		justify-content: center;
		gap: 0.4rem;
		font-family: 'Noto Sans';
		font-weight: 600;
		font-size: 0.95rem;
		color: #fff;
		background: linear-gradient(135deg, var(--primary-500), var(--primary-600));
		border-radius: 999px;
		padding: 0.45rem 1rem;
		box-shadow: 0 15px 30px rgba(55, 84, 255, 0.25);
		transition: transform 0.2s ease, box-shadow 0.2s ease;
	}

	.lang-switch:hover,
	.lang-switch:focus-visible {
		transform: translateY(-1px);
		box-shadow: 0 18px 38px rgba(55, 84, 255, 0.3);
	}

	@media (max-width: 1024px) {
		nav {
			position: sticky;
		}
	}
</style>
