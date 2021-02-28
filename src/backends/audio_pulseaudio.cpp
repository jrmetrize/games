#include "backends/audio_pulseaudio.h"

AudioLayerPulseAudio::AudioLayerPulseAudio()
{
  format.format = PA_SAMPLE_S16NE;
  format.channels = 2;
  format.rate = 48000;

  /*connection = pa_simple_new(nullptr, "fp2d", PA_STREAM_PLAYBACK, nullptr,
    "gameaudio", &format, nullptr, nullptr, nullptr);*/
}

AudioLayerPulseAudio::~AudioLayerPulseAudio()
{
  //pa_simple_free(connection);
}

void
AudioLayerPulseAudio::start()
{
  /*auto result = client->Reset();
  result = client->Start();*/
}

void
AudioLayerPulseAudio::stop()
{
  //client->Stop();
}

void
AudioLayerPulseAudio::load_buffer(int16_t *buffer_data, uint32_t frames)
{
  /*BYTE *data;
  render_client->GetBuffer(frames, &data);
  memcpy(data, buffer_data, sizeof(int16_t) * 2 * frames);
  render_client->ReleaseBuffer(frames, 0);*/
}

uint32_t
AudioLayerPulseAudio::get_current_padding()
{
  /*uint32_t padding;
  client->GetCurrentPadding(&padding);
  return padding;*/
  return 4096;
}
