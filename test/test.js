/* eslint-disable no-console */
const { G722Encoder, G722Decoder } = require('../cjs/index.cjs');

const sr = 16000, ms = 20, n = sr*ms/1000;
const pcm = Buffer.alloc(n*2);
let ph=0, w=2*Math.PI*1000/sr;
for (let i=0;i<n;i++){ pcm.writeInt16LE(Math.round(Math.sin(ph)*12000), i*2); ph+=w; }

const enc = new G722Encoder();
const dec = new G722Decoder();

const g = enc.encode(pcm);
const back = dec.decode(g);

console.log('encoded bytes ~160:', g.length);
console.log('decoded samples ~320:', back.length/2);