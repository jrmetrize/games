#ifndef AUDIO_H
#define AUDIO_H

#include <thread>
#include <vector>

class AudioTrack;

class AudioLayer
{
public:
  virtual
  ~AudioLayer() = 0;

  virtual void
  start() = 0;

  virtual void
  stop() = 0;

  virtual void
  load_buffer(int16_t *buffer_data, uint32_t frames) = 0;
};

class AudioServer
{
  static AudioServer *instance;

  AudioLayer *backend;

  bool is_playing;
  std::thread buffer_thread;

  struct PlayingAudio
  {
    AudioTrack *track;
    uint32_t offset;
    bool loop;
  };

  std::vector<PlayingAudio> playing;

  void
  buffer();
public:
  AudioServer();

  ~AudioServer();

  static void
  set_instance(AudioServer *_instance);

  static AudioServer *
  get();

  void
  start();

  void
  stop();

  void
  play(AudioTrack *track);
};

#endif
