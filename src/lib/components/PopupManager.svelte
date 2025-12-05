<script>
	import { onMount } from 'svelte';

	// Configuration for popups
	// Each popup can have multiple clickable link areas defined by style (top, left, width, height)
	const popups = [
		{
			src: 'HIRING_450x600.jpg',
			links: [
				{
					url: 'https://www.saramin.co.kr/zf_user/company-info/view-inner-recruit?csn=VkJIYUFuNkxQTXB0YWNWTmp0M0xpQT09',
					style: 'bottom: 15%; right: 15%; width: 30%; height: 10%;',
					alt: 'Go to Hiring Page'
				}
			]
		}
	];

	let activePopups = [];
	// Track "Don't show again" checkbox state per popup
	let dontShowState = {};

	onMount(() => {
		// Check each popup
		popups.forEach(popup => {
			const hideUntil = localStorage.getItem(`popup_${popup.src}_hideUntil`);
			if (hideUntil) {
				const hideTime = parseInt(hideUntil);
				if (Date.now() >= hideTime) {
					// 24 hours have passed, remove the flag and show popup
					localStorage.removeItem(`popup_${popup.src}_hideUntil`);
					activePopups.push(popup);
				}
			} else {
				// No flag set, show popup
				activePopups.push(popup);
			}
			// Initialize checkbox state
			dontShowState[popup.src] = false;
		});
		activePopups = activePopups; // Trigger reactivity
	});

	function closePopup(popupSrc) {
		// If checkbox is checked, save preference
		if (dontShowState[popupSrc]) {
			// Set timestamp for 24 hours from now
			const hideUntil = Date.now() + 24 * 60 * 60 * 1000;
			localStorage.setItem(`popup_${popupSrc}_hideUntil`, hideUntil.toString());
		}
		
		activePopups = activePopups.filter(p => p.src !== popupSrc);
	}
</script>

{#if activePopups.length > 0}
	<div class="popup-overlay" role="presentation">
		{#each activePopups as popup}
			<div class="popup-content">
				<div class="popup-inner">
					<div class="image-wrapper">
						<img src="/popup-image/{popup.src}" alt="Popup" class="popup-image" />
						{#if popup.links}
							{#each popup.links as link}
								<a
									href={link.url}
									target="_blank"
									class="popup-link"
									title={link.alt}
									aria-label={link.alt}
									style={link.style}
								></a>
							{/each}
						{/if}
					</div>
					<div class="popup-footer">
						<label class="dont-show-option">
							<input type="checkbox" bind:checked={dontShowState[popup.src]} />
							<span>Don't show for 1 day</span>
						</label>
						<button class="close-button" on:click={() => closePopup(popup.src)}>Close</button>
					</div>
				</div>
			</div>
		{/each}
	</div>
{/if}

<style>
	.popup-overlay {
		position: fixed;
		top: 0;
		left: 0;
		width: 100%;
		height: 100%;
		background-color: rgba(0, 0, 0, 0.7);
		display: flex;
		justify-content: center;
		align-items: center;
		gap: 2rem;
		z-index: 9999;
	}

	.popup-content {
		position: relative;
		background: transparent;
		padding: 0;
		border-radius: 8px;
		max-width: 90%;
		max-height: 90%;
		margin: 1rem;
		display: flex;
		flex-direction: column;
		align-items: center;
	}

	.popup-inner {
		position: relative;
		background: white;
		padding: 10px; /* Reduced padding */
		border-radius: 8px;
		box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
		display: flex;
		flex-direction: column;
	}

	.popup-footer {
		display: flex;
		justify-content: space-between;
		align-items: center;
		width: 100%;
		padding-top: 10px;
	}

	.dont-show-option {
		display: flex;
		align-items: center;
		gap: 0.5rem;
		font-size: 0.9rem;
		color: #333;
		cursor: pointer;
	}

	.close-button {
		background: none;
		border: 1px solid #ccc;
		border-radius: 4px;
		padding: 0.25rem 0.75rem;
		font-size: 0.9rem;
		cursor: pointer;
		color: #333;
	}

	.close-button:hover {
		background-color: #f0f0f0;
		color: #000;
	}

	.popup-image {
		max-width: 100%;
		max-height: 70vh;
		display: block;
		border-radius: 4px;
	}

	.image-wrapper {
		position: relative;
		display: inline-block;
	}

	.popup-link {
		position: absolute;
		cursor: pointer;
		z-index: 10;
	}
</style>
