import fs from 'fs';
import path from 'path';

const docRoot = 'static/documentation';
const outputFile = 'src/lib/manual-list.json';
const allGroupedManuals = [];

console.log('Scanning for product manuals...');

try {
	// Get product folders like 'raibo', 'raion-x', etc.
	const productFolders = fs.readdirSync(docRoot, { withFileTypes: true })
		.filter(dirent => dirent.isDirectory())
		.map(dirent => dirent.name);

	for (const productName of productFolders) {
		const productPath = path.join(docRoot, productName);
		const manuals = [];

		// Get version folders like 'v_1.0', 'v_2.1', etc.
		const versionFolders = fs.readdirSync(productPath)
			.filter(name => name.startsWith('v_'));

		for (const versionName of versionFolders) {
			manuals.push({
				version: versionName.replace('v_', ''),
				path: `/documentation/${productName}/${versionName}/index.html`
			});
		}

		if (manuals.length > 0) {
			// Sort manuals for the current product, newest first
			manuals.sort((a, b) => b.version.localeCompare(a.version, undefined, { numeric: true }));

			allGroupedManuals.push({
				groupName: productName,
				manuals: manuals
			});
		}
	}

	// Sort the main product groups alphabetically
	allGroupedManuals.sort((a, b) => a.groupName.localeCompare(b.groupName));

	fs.writeFileSync(outputFile, JSON.stringify(allGroupedManuals, null, 2));
	console.log(`✅ Grouped manual list generated successfully at ${outputFile}`);

} catch (error) {
	console.error(`❌ Could not read manuals directory: ${docRoot}`);
	// Create an empty file on error so the build doesn't fail
	fs.writeFileSync(outputFile, JSON.stringify([], null, 2));
}