#ifndef AUDIO_WASAPI_H
#define AUDIO_WASAPI_H

#include "audio.h"

#include <audioclient.h>
#include <Mmdeviceapi.h>

class AudioLayerWasapi : public AudioLayer
{
  IMMDeviceEnumerator *device_enumerator;
  IMMDeviceCollection *device_collection;
  IMMDevice *device;

  WAVEFORMATEX *format;

  IAudioClient *client;
  IAudioRenderClient *render_client;

  unsigned int buffer_frames;
public:
  AudioLayerWasapi();

  ~AudioLayerWasapi();

  void
  start();

  void
  stop();
};

#endif
