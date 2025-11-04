export const prerender = false;

/**
 * Cloudflare strips the `.html` suffix from Sphinx search requests when a query
 * string is present. This endpoint proxies those requests to the actual static
 * file so we don't have to modify the generated documentation assets.
 */
export async function GET({ params, url, fetch }) {
	const slug = params.slug ?? '';

	// Only handle documentation search requests missing their .html suffix.
	if (!slug || url.pathname.endsWith('.html') || !url.searchParams.has('q')) {
		return new Response(null, { status: 404 });
	}

	const htmlUrl = new URL(url);
	htmlUrl.pathname = `${htmlUrl.pathname}.html`;

	const response = await fetch(htmlUrl.toString(), {
		headers: {
			accept: 'text/html,application/xhtml+xml'
		}
	});

	// Return the static search page if it exists; otherwise bubble up the status.
	if (response.ok) {
		return response;
	}

	return new Response(response.body, {
		status: response.status,
		headers: response.headers
	});
}
