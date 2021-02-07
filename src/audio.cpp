#include "audio.h"
#include "resource.h"
#include <chrono>
#include <algorithm>

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

AudioServer::AudioServer() :
  is_playing(false), playing()
{
#ifdef _WIN32
  backend = new AudioLayerWasapi();
#endif
}

AudioServer::~AudioServer()
{
  stop();
  delete backend;
}

void
AudioServer::buffer()
{
  uint32_t buffer_frames = 48000 / (1000 / 20);

  while (is_playing)
  {
    // Buffer for 20ms exactly, so get the time before we do any work,
    // and then wait until 20ms after to loop again.
    auto begin = std::chrono::steady_clock::now();
    auto next = begin + std::chrono::milliseconds(20);

    // Do work
    std::vector<int16_t> stereo_buffer = std::vector<int16_t>(buffer_frames * 2);

    std::vector<PlayingAudio> still_playing = std::vector<PlayingAudio>();
    for (unsigned int i = 0; i < playing.size(); ++i)
    {
      playing[i].offset += buffer_frames;

      // Find the number of frames available and load them, possibly converting
      // mono to stereo.
      uint32_t frames_available = playing[i].track->get_frames() - playing[i].offset;
      uint32_t to_copy = min(buffer_frames, frames_available);
      if (playing[i].track->get_channels() == 1)
      {
        for (uint32_t j = 0; j < to_copy; ++j)
        {
          stereo_buffer[j * 2] = playing[i].track->get_samples()[playing[i].offset + j];
          stereo_buffer[(j * 2) + 1] = stereo_buffer[j * 2];
        }
      }
      else if (playing[i].track->get_channels() == 2)
      {
        // We can mix stereo audio directly
        for (uint32_t j = 0; j < to_copy; ++j)
        {
          stereo_buffer[j * 2] = playing[i].track->get_samples()[(playing[i].offset + j) * 2];
          stereo_buffer[(j * 2) + 1] = playing[i].track->get_samples()[((playing[i].offset + j) * 2) + 1];
        }
      }

      if (frames_available - buffer_frames > 0)
      {
        // This track still has frames to play, so keep playing it the next time around
        still_playing.push_back(playing[i]);
      }
    }
    playing = still_playing;

    backend->load_buffer(stereo_buffer.data(), buffer_frames);

    // Sleep
    std::this_thread::sleep_until(next);
  }
}

void
AudioServer::start()
{
  backend->start();
  is_playing = true;
  buffer_thread = std::thread(&AudioServer::buffer, this);
}

void
AudioServer::stop()
{
  is_playing = false;
  buffer_thread.join();
  backend->stop();
}

void
AudioServer::play(AudioTrack *track)
{
  PlayingAudio audio = {};
  audio.track = track;
  audio.offset = 0;
  audio.loop = false;
  playing.push_back(audio);
}
