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

  virtual uint32_t
  get_current_padding() = 0;
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
  PlayingAudio music;
  bool music_on;

  bool
  mix_to_buffer(PlayingAudio *audio, std::vector<int16_t> &stereo_buffer,
    uint32_t buffer_frames);

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

  void
  set_music(AudioTrack *_music);

  void
  start_music();

  void
  stop_music();
};

#endif
