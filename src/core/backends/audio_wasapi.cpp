#include "core/backends/audio_wasapi.h"
#include <cstdint>
#include <limits>
#include <cmath>

AudioLayerWasapi::AudioLayerWasapi()
{
  CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
    __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&device_enumerator));
  auto result = device_enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE,
    &device_collection);
  result = device_enumerator->GetDefaultAudioEndpoint(eRender, eConsole,
    &device);
  /*result = device_collection->Item(0, &device);*/
  result = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
    reinterpret_cast<void **>(&client));

  {
    WAVEFORMATEX *mix_fmt;
    result = client->GetMixFormat(&mix_fmt);

    format = {};
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.nSamplesPerSec = 48000;
    format.wBitsPerSample = 16;
    format.nBlockAlign = (format.wBitsPerSample / 8) * format.nChannels;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

    WAVEFORMATEX *match;
    auto err = client->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED,
      &format, &match);
  }

  int buffer_length = 1 * (1000 * 1000 * 10);

  result = client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, buffer_length, 0, &format, nullptr);
  result = client->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void **>(&render_client));

  result = client->GetBufferSize(&buffer_frames);
}

AudioLayerWasapi::~AudioLayerWasapi()
{
  device_enumerator->Release();
  device_collection->Release();
  device->Release();
  client->Release();
}

void
AudioLayerWasapi::start()
{
  auto result = client->Reset();
  result = client->Start();
}

void
AudioLayerWasapi::stop()
{
  client->Stop();
}

void
AudioLayerWasapi::load_buffer(int16_t *buffer_data, uint32_t frames)
{
  BYTE *data;
  render_client->GetBuffer(frames, &data);
  memcpy(data, buffer_data, sizeof(int16_t) * 2 * frames);
  render_client->ReleaseBuffer(frames, 0);
}

uint32_t
AudioLayerWasapi::get_current_padding()
{
  uint32_t padding;
  client->GetCurrentPadding(&padding);
  return padding;
}
