<script>
	// This file is created automatically when you run `node generate-manuals.js` or `npm run build`.
	import groupedManuals from '$lib/manual-list.json';
</script>

<svelte:head>
	<title>Product Manuals</title>
	<meta name="manuals" content="List of Product Manuals" />
</svelte:head>

<section class="container">
	<div class="section_title">
		<h1>Product Manuals
			<span>Select a product to view available manuals.</span>
		</h1>
	</div>

	{#each groupedManuals as group}
		<div class="product-group">
			<h2 class="product-header">{group.groupName}</h2>
			<div class="manuals-list">
				{#each group.manuals as manual}
					<a class="manual-item" href={manual.path} target="_blank" rel="noopener noreferrer">
						<span class="version-icon">📄</span>
						<span>Version {manual.version}</span>
					</a>
				{:else}
					<p>No versions found for this product.</p>
				{/each}
			</div>
		</div>
	{:else}
		<p>No product manuals were found.</p>
	{/each}
</section>

<style>
    .container {
        width: 80vw;
        max-width: 900px;
        margin: auto;
        padding: 20px 0;
        font-family: sans-serif;
    }
    .section_title {
        text-align: left;
        margin-bottom: 2rem;
    }
    .section_title h1 {
        font-size: 2.2rem;
        font-weight: bold;
        color: #2d3748;
    }
    .section_title span {
        display: block;
        font-size: 1rem;
        color: #718096;
        margin-top: 0.5rem;
    }
    .product-group {
        margin-bottom: 2.5rem;
    }
    .product-header {
        font-size: 1.5rem;
        font-weight: 600;
        color: #4a5568;
        text-transform: capitalize;
        padding-bottom: 0.5rem;
        border-bottom: 2px solid #e2e8f0;
        margin-bottom: 1rem;
    }
    .manuals-list {
        display: grid;
        grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
        gap: 1rem;
    }
    .manual-item {
        background-color: #f7fafc;
        border: 1px solid #e2e8f0;
        border-radius: 8px;
        padding: 15px 20px;
        text-decoration: none;
        color: #2d3748;
        font-size: 1rem;
        display: flex;
        align-items: center;
        transition: all 0.2s ease-in-out;
    }
    .manual-item:hover {
        border-color: #4299e1;
        background-color: #ebf8ff;
        transform: translateY(-2px);
        box-shadow: 0 4px 14px 0 rgba(0, 0, 0, 0.05);
    }
    .version-icon {
        margin-right: 12px;
        font-size: 1.2rem;
    }
</style>