/**
 * Work around Cloudflare dropping the `.html` suffix for Sphinx search results.
 * If we detect a documentation search request without the HTML extension,
 * proxy it internally to the generated `search.html` file.
 */
export async function handle({ event, resolve }) {
	const url = new URL(event.request.url);

	if (
		url.pathname.startsWith('/documentation/') &&
		url.searchParams.has('q') &&
		!url.pathname.endsWith('.html')
	) {
		const htmlUrl = new URL(`${url.pathname}.html${url.search}`, url.origin);
		const proxyRequest = new Request(htmlUrl, event.request);
		const response = await event.fetch(proxyRequest);

		if (response.ok || response.type === 'opaqueredirect') {
			return response;
		}
	}

	return resolve(event);
}

