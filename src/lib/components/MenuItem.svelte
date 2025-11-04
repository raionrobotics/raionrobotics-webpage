<li class="menu-item">
	<slot />
</li>

<style>
	.menu-item {
		list-style: none;
		font-family: 'Noto Sans';
		font-size: 0.95rem;
		font-weight: 600;
		color: var(--text-primary);
		padding: 0.6rem 0.85rem;
		border-radius: 0.85rem;
		transition: background 0.2s ease, color 0.2s ease, transform 0.2s ease;
		cursor: pointer;
	}

	.menu-item:hover,
	.menu-item:focus-visible {
		background: var(--primary-100);
		color: var(--primary-600);
		transform: translateX(4px);
	}

	:global(.menu-wrapper a) {
		color: inherit;
		text-decoration: none;
		display: block;
		width: 100%;
	}
</style>
