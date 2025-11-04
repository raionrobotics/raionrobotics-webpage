<div class="menu-wrapper">
	<button class="menu-toggle" type="button">
		<span class="menu-label"><slot name="toggle" /></span>
		<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 20 20" fill="currentColor" class="chevron">
			<path fill-rule="evenodd" d="M5.23 7.21a.75.75 0 011.06.02L10 10.939l3.71-3.71a.75.75 0 111.06 1.061l-4.24 4.239a.75.75 0 01-1.06 0L5.21 8.29a.75.75 0 01.02-1.08z" clip-rule="evenodd" />
		</svg>
	</button>
	<div class="menu-panel" role="menu">
		<slot />
	</div>
</div>

<style>
	.menu-wrapper {
		position: relative;
		display: none;
		align-items: center;
	}

	@media (min-width: 1024px) {
		.menu-wrapper {
			display: inline-flex;
		}
	}

	.menu-toggle {
		display: inline-flex;
		align-items: center;
		gap: 0.4rem;
		border: none;
		background: transparent;
		font-family: 'Noto Sans';
		font-weight: 600;
		color: var(--text-secondary);
		font-size: 0.95rem;
		padding: 0.35rem 0;
		cursor: pointer;
		position: relative;
		transition: color 0.2s ease;
	}

	.menu-toggle::after {
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

	.menu-toggle:hover,
	.menu-toggle:focus-visible {
		color: var(--primary-500);
	}

	.menu-toggle:hover::after,
	.menu-toggle:focus-visible::after {
		opacity: 1;
		transform: translateY(0);
	}

	.chevron {
		width: 18px;
		height: 18px;
		transition: transform 0.2s ease;
	}

	.menu-wrapper:hover .chevron,
	.menu-wrapper:focus-within .chevron {
		transform: rotate(180deg);
	}

	.menu-panel {
		position: absolute;
		top: calc(100% + 1rem);
		right: 0;
		display: flex;
		flex-direction: column;
		min-width: 220px;
		margin-block-start: 0;
		padding: 1rem;
		border-radius: 1rem;
		background: #fff;
		box-shadow: 0 25px 60px rgba(15, 23, 42, 0.18);
		border: 1px solid rgba(15, 23, 42, 0.05);
		opacity: 0;
		pointer-events: none;
		transform: translateY(-0.5rem);
		transition: opacity 0.25s ease, transform 0.25s ease;
	}

	.menu-wrapper:hover .menu-panel,
	.menu-wrapper:focus-within .menu-panel {
		opacity: 1;
		pointer-events: auto;
		transform: translateY(0);
	}
</style>
