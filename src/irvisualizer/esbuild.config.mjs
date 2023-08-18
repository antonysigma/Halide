import process from 'process';
import esbuild from 'esbuild';

const settings = {
	entryPoints: ['StmtToHTML.js'],
	bundle: true,
	minify: true,
	sourcemap: true,
	outdir: './dist/',
	target: ['chrome114'],
	logLevel: 'info',
	loader: {
		'.svg': 'dataurl',
		'.woff2': 'file',
		'.woff': 'empty',
		'.ttf': 'empty',
		'.eot': 'empty',
	},
};

const is_watch = process.argv.includes('--watch');

if(!is_watch) {
	esbuild.build(settings);
} else {
	const ctx = await esbuild.context(settings);
	ctx.watch();

	ctx.serve({
		servedir: settings.outdir + '/../',
	});
}