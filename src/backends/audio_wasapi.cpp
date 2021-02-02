#include "backends/audio_wasapi.h"
#include <cstdint>
#include <cmath>

#include <iostream>
void
log_err(HRESULT err)
{
  if (err == S_OK)
    std::cout << "ok\n";
  else
    std::cout << "err\n";
}

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
  result = client->GetMixFormat(&format);

  int buffer_length = 1 * (1000 * 1000 * 10);

  result = client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, buffer_length, 0, format, nullptr);
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

  {
    unsigned int buffer_padding;
    auto result = client->GetCurrentPadding(&buffer_padding);

    unsigned int frames_available = buffer_frames - buffer_padding;
    unsigned char *buffer_data;
    result = render_client->GetBuffer(frames_available, &buffer_data);

    for (unsigned int i = 0; i < frames_available; ++i)
    {
      float *left = reinterpret_cast<float *>(&buffer_data[8 * i]);
      float *right = reinterpret_cast<float *>(&buffer_data[(8 * i) + 4]);

      float val = sin((1.0f / 20.0f) * float(i));
      *left = val;
      *right = val;
    }

    result = render_client->ReleaseBuffer(frames_available, 0);
  }
}

void
AudioLayerWasapi::stop()
{
  client->Stop();
}
