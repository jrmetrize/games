#ifndef AUDIO_H
#define AUDIO_H

class AudioLayer
{
public:
  virtual
  ~AudioLayer() = 0;

  virtual void
  start() = 0;

  virtual void
  stop() = 0;
};

class AudioServer
{
  static AudioServer *instance;

  AudioLayer *backend;
public:
  AudioServer();

  ~AudioServer();

  static void
  set_instance(AudioServer *_instance);

  static AudioServer *
  get();
};

#endif
