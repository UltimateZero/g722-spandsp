import { fileURLToPath } from 'node:url';
import path from 'node:path';
import { createRequire } from 'node:module';

const require = createRequire(import.meta.url);
const nodeGypBuild = require('node-gyp-build');

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const addon = nodeGypBuild(path.join(__dirname, '..'));

export const { G722Encoder, G722Decoder } = addon;
export default addon;