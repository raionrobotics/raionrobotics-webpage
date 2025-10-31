<script>
	import { onMount } from 'svelte';

	// List of popup images
	const popupImages = [
	];

	let activePopups = [];

	onMount(() => {
		// Check each popup image
		popupImages.forEach(imageName => {
			const hideUntil = localStorage.getItem(`popup_${imageName}_hideUntil`);
			if (hideUntil) {
				const hideTime = parseInt(hideUntil);
				if (Date.now() >= hideTime) {
					// 24 hours have passed, remove the flag and show popup
					localStorage.removeItem(`popup_${imageName}_hideUntil`);
					activePopups.push(imageName);
				}
			} else {
				// No flag set, show popup
				activePopups.push(imageName);
			}
		});
		activePopups = activePopups; // Trigger reactivity
	});

	function closePopup(imageName) {
		activePopups = activePopups.filter(name => name !== imageName);
	}

	function hideForOneDay(imageName) {
		activePopups = activePopups.filter(name => name !== imageName);
		// Set timestamp for 24 hours from now
		const hideUntil = Date.now() + 24 * 60 * 60 * 1000;
		localStorage.setItem(`popup_${imageName}_hideUntil`, hideUntil.toString());
	}
</script>

{#if activePopups.length > 0}
	<div class="popup-overlay">
		{#each activePopups as imageName}
			<div class="popup-content" on:click|stopPropagation>
				<button class="close-button" on:click={() => closePopup(imageName)}>×</button>
				<img src="/popup-image/{imageName}" alt="Popup" class="popup-image" />
				<button class="hide-button" on:click={() => hideForOneDay(imageName)}>Don't show for 1 day</button>
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
		background: white;
		padding: 2rem;
		border-radius: 8px;
		max-width: 90%;
		max-height: 90%;
		box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
		margin: 1rem;
	}

	.close-button {
		position: absolute;
		top: 10px;
		right: 10px;
		background: none;
		border: none;
		font-size: 2rem;
		cursor: pointer;
		color: #333;
		line-height: 1;
		padding: 0;
		width: 30px;
		height: 30px;
	}

	.close-button:hover {
		color: #000;
	}

	.popup-image {
		max-width: 100%;
		max-height: 70vh;
		display: block;
		border-radius: 4px;
	}

	.hide-button {
		margin-top: 1rem;
		padding: 0.5rem 1rem;
		background-color: #333;
		color: white;
		border: none;
		border-radius: 4px;
		cursor: pointer;
		font-size: 0.9rem;
		width: 100%;
	}

	.hide-button:hover {
		background-color: #555;
	}
</style>