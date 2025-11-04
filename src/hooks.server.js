/**
 * Ensure Cloudflare serves the generated Sphinx search pages.
 * Cloudflare strips the `.html` suffix when handling requests that include `?q=`,
 * which makes `/documentation/.../search` miss the static `search.html`.
 * We detect that pattern here and redirect back to the correct HTML asset.
 */
export async function handle({ event, resolve }) {
	const url = new URL(event.request.url);

	if (
		url.pathname.startsWith('/documentation/') &&
		url.searchParams.has('q') &&
		!url.pathname.endsWith('.html')
	) {
		const redirectPath = `${url.pathname}.html${url.search}`;
		return new Response(null, {
			status: 308,
			headers: {
				location: redirectPath
			}
		});
	}

	return resolve(event);
}

