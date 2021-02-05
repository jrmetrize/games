#include "audio.h"

#ifdef _WIN32
#include "backends/audio_wasapi.h"
#endif

AudioLayer::~AudioLayer()
{

}

AudioServer * AudioServer::instance = nullptr;

void
AudioServer::set_instance(AudioServer *_instance)
{
  instance = _instance;
}

AudioServer *
AudioServer::get()
{
  return instance;
}

AudioServer::AudioServer()
{
#ifdef _WIN32
  backend = new AudioLayerWasapi();
#endif
  //backend->start();
}

AudioServer::~AudioServer()
{
  //backend->stop();
  //delete backend;
}
