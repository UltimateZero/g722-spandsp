/// <reference types="node" />

export class G722Encoder {
    constructor();
    /** Encode PCM16LE (@16 kHz) into G.722 octet-aligned. */
    encode(input: Buffer | Uint8Array): Buffer;
    /** Reset the internal encoder state. */
    reset(): void;
  }
  
  export class G722Decoder {
    constructor();
    /** Decode G.722 octet-aligned into PCM16LE (@16 kHz). */
    decode(input: Buffer | Uint8Array): Buffer;
    /** Reset the internal decoder state. */
    reset(): void;
  }
  
  declare const _default: {
    G722Encoder: typeof G722Encoder;
    G722Decoder: typeof G722Decoder;
  };
  export default _default;
  