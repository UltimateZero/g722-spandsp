#include <napi.h>

extern "C" {
  #include <spandsp/telephony.h>   // defines SPAN_DECLARE
  #include <spandsp/g722.h>
}

class G722Encoder : public Napi::ObjectWrap<G722Encoder> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::Function ctor = DefineClass(env, "G722Encoder", {
      InstanceMethod("encode", &G722Encoder::Encode),
      InstanceMethod("reset",  &G722Encoder::Reset)
    });
    exports.Set("G722Encoder", ctor);
    return exports;
  }

  G722Encoder(const Napi::CallbackInfo& info)
  : Napi::ObjectWrap<G722Encoder>(info), state_(nullptr) {
    state_ = g722_encode_init(nullptr, 64000, G722_PACKED);
  }

  ~G722Encoder() override {
    if (state_) {
      g722_encode_release(state_);
      g722_encode_free(state_);
      state_ = nullptr;
    }
  }

private:
  Napi::Value Encode(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || (!info[0].IsBuffer() && !info[0].IsTypedArray())) {
      Napi::TypeError::New(env, "encode(pcm16le Buffer|Uint8Array) required").ThrowAsJavaScriptException();
      return env.Null();
    }

    uint8_t* inPtr; size_t inLen;
    if (info[0].IsBuffer()) {
      auto b = info[0].As<Napi::Buffer<uint8_t>>();
      inPtr = b.Data(); inLen = b.Length();
    } else {
      auto u8 = info[0].As<Napi::Uint8Array>();
      inPtr = u8.Data(); inLen = u8.ByteLength();
    }

    if ((inLen & 1) != 0) {
      Napi::TypeError::New(env, "PCM16LE length must be even").ThrowAsJavaScriptException();
      return env.Null();
    }

    const int16_t* samples = reinterpret_cast<const int16_t*>(inPtr);
    int nsamples = static_cast<int>(inLen / 2);

    int maxBytes = nsamples / 2 + 8;
    std::vector<uint8_t> out(maxBytes);

    int n = g722_encode(state_, out.data(), samples, nsamples);
    if (n < 0) n = 0;

    return Napi::Buffer<uint8_t>::Copy(env, out.data(), static_cast<size_t>(n));
  }

  Napi::Value Reset(const Napi::CallbackInfo& info) {
    if (state_) {
      g722_encode_release(state_);
      g722_encode_free(state_);
    }
    state_ = g722_encode_init(nullptr, 64000, G722_PACKED);
    return info.Env().Undefined();
  }

  g722_encode_state_t* state_;
};

class G722Decoder : public Napi::ObjectWrap<G722Decoder> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::Function ctor = DefineClass(env, "G722Decoder", {
      InstanceMethod("decode", &G722Decoder::Decode),
      InstanceMethod("reset",  &G722Decoder::Reset)
    });
    exports.Set("G722Decoder", ctor);
    return exports;
  }

  G722Decoder(const Napi::CallbackInfo& info)
  : Napi::ObjectWrap<G722Decoder>(info), state_(nullptr) {
    state_ = g722_decode_init(nullptr, 64000, G722_PACKED);
  }

  ~G722Decoder() override {
    if (state_) {
      g722_decode_release(state_);
      g722_decode_free(state_);
      state_ = nullptr;
    }
  }

private:
  Napi::Value Decode(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || (!info[0].IsBuffer() && !info[0].IsTypedArray())) {
      Napi::TypeError::New(env, "decode(g722 Buffer|Uint8Array) required").ThrowAsJavaScriptException();
      return env.Null();
    }

    uint8_t* inPtr; size_t inLen;
    if (info[0].IsBuffer()) {
      auto b = info[0].As<Napi::Buffer<uint8_t>>();
      inPtr = b.Data(); inLen = b.Length();
    } else {
      auto u8 = info[0].As<Napi::Uint8Array>();
      inPtr = u8.Data(); inLen = u8.ByteLength();
    }

    int maxSamples = static_cast<int>(inLen) * 2 + 32;
    std::vector<int16_t> pcm(maxSamples);

    int nsamp = g722_decode(state_, pcm.data(), inPtr, static_cast<int>(inLen));
    if (nsamp < 0) nsamp = 0;

    const size_t outBytes = static_cast<size_t>(nsamp) * sizeof(int16_t);
    return Napi::Buffer<uint8_t>::Copy(env, reinterpret_cast<uint8_t*>(pcm.data()), outBytes);
  }

  Napi::Value Reset(const Napi::CallbackInfo& info) {
    if (state_) {
      g722_decode_release(state_);
      g722_decode_free(state_);
    }
    state_ = g722_decode_init(nullptr, 64000, G722_PACKED);
    return info.Env().Undefined();
  }

  g722_decode_state_t* state_;
};

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  G722Encoder::Init(env, exports);
  G722Decoder::Init(env, exports);
  return exports;
}

NODE_API_MODULE(g722, InitAll)