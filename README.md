# g722-spandsp

Node-API bindings for **SpanDSP** G.722 (encode/decode).  
Minimal, fast, production-friendly. Works with **ESM** and **CommonJS**.

> ⚠️ This package **links dynamically** against system `libspandsp` (LGPL-2.1).  
> Your app remains MIT/closed as long as you **dynamically link** and ship the LGPL notice.

## Install

### macOS (Apple Silicon / Intel)
```bash
brew install spandsp pkg-config
npm i g722-spandsp
# or from source in this repo:
npm install
npm run build
```

### Debian/Ubuntu
```bash
sudo apt-get update
sudo apt-get install -y build-essential python3 pkg-config libspandsp-dev
npm i g722-spandsp
```

> Windows is not supported.

## Usage

### ESM
```js
import { G722Encoder, G722Decoder } from 'g722-spandsp';

// 20 ms of 1 kHz @ 16 kHz PCM16LE
const sr = 16000, ms = 20, n = sr*ms/1000;
const pcm = Buffer.alloc(n*2);
let ph=0, w=2*Math.PI*1000/sr;
for (let i=0;i<n;i++){ pcm.writeInt16LE(Math.round(Math.sin(ph)*12000), i*2); ph+=w; }

const enc = new G722Encoder();
const g = enc.encode(pcm);      // ~160 bytes (64 kbps)

const dec = new G722Decoder();
const back = dec.decode(g);     // ~320 samples PCM16
```

### CommonJS
```js
const { G722Encoder, G722Decoder } = require('g722-spandsp');
```

## API

- `new G722Encoder()`
  - `encode(pcm16le: Buffer|Uint8Array): Buffer` — encodes to **G.722 octet-aligned** (64 kbps).
  - `reset(): void`
- `new G722Decoder()`
  - `decode(g722: Buffer|Uint8Array): Buffer` — decodes to **PCM16LE @ 16 kHz**.
  - `reset(): void`

## Notes

- G.722 audio sample rate is **16 kHz**.  
  RTP timestamps tick at **8 kHz** (RFC quirk).
- The addon uses `G722_PACKED` (octet-aligned) payloads.

## Build from source (in this repo)

```bash
npm install
npm run build
node test/test.js
```

## License

- Code here: **MIT** (see `LICENSE`)
- Runtime dependency: **SpanDSP (LGPL-2.1)** — dynamically linked.
