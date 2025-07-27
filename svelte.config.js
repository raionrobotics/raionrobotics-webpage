import adapter from '@sveltejs/adapter-cloudflare';

export default {
	kit: {
		adapter: adapter({
			routes: {
				include: ['/*'], // match all
				exclude: []      // match nothing else
			}
		})
	}
};
