#ifndef AUDIO_PULSEAUDIO_H
#define AUDIO_PULSEAUDIO_H

#include "audio.h"
#include <pulse/pulseaudio.h>

class AudioLayerPulseAudio : public AudioLayer
{
  pa_stream *stream;
  pa_sample_spec format;

  unsigned int buffer_frames;
public:
  AudioLayerPulseAudio();

  ~AudioLayerPulseAudio();

  void
  start();

  void
  stop();

  void
  load_buffer(int16_t *buffer_data, uint32_t frames);

  uint32_t
  get_current_padding();
};

#endif
