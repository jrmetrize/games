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

bool
AudioServer::mix_to_buffer(PlayingAudio *audio,
  std::vector<int16_t> &stereo_buffer, uint32_t buffer_frames)
{
  uint32_t audio_length = audio->track->get_frames();
  uint32_t data_points = audio_length * audio->track->get_channels();

  // Find the number of frames available and load them, possibly converting
  // mono to stereo.
  uint32_t frames_available = audio_length - audio->offset;
  uint32_t to_copy = min(buffer_frames, frames_available);
  // If looping, always fill the buffer
  if (audio->loop)
    to_copy = buffer_frames;
  if (audio->track->get_channels() == 1)
  {
    for (uint32_t j = 0; j < to_copy; ++j)
    {
      stereo_buffer[j * 2] += audio->track->get_samples()[(audio->offset + j) % data_points];
      stereo_buffer[(j * 2) + 1] += stereo_buffer[j * 2];
    }
  }
  else if (audio->track->get_channels() == 2)
  {
    // We can mix stereo audio directly
    for (uint32_t j = 0; j < to_copy; ++j)
    {
      stereo_buffer[j * 2] += audio->track->get_samples()[((audio->offset + j) * 2) % data_points];
      stereo_buffer[(j * 2) + 1] += audio->track->get_samples()[(((audio->offset + j) * 2) + 1) % data_points];
    }
  }

  audio->offset += buffer_frames;
  audio->offset = audio->offset % audio_length;

  if (frames_available - buffer_frames > 0 || audio->loop)
  {
    return true;
  }
  return false;
}

void
AudioServer::buffer()
{
  uint32_t buffer_frames = 48000 / (1000 / 20);

  float usec_per_sample = 1000000.0f / 48000.0f;
  float duration_90 = 0.9f * float(buffer_frames) * usec_per_sample;

  while (is_playing)
  {
    // Buffer for 20ms exactly, so get the time before we do any work,
    // and then wait until 20ms after to loop again.

    // FIXME: instead, wake up when about 10% of the buffer is left and fill
    // in the next part to avoid timing inconsistencies and pops.
    auto begin = std::chrono::steady_clock::now();
    uint32_t padding = backend->get_current_padding();
    float to_wait = usec_per_sample * float(padding);
    to_wait += duration_90;
    auto next = begin + std::chrono::microseconds(uint32_t(to_wait));

    // Do work
    std::vector<int16_t> stereo_buffer = std::vector<int16_t>(buffer_frames * 2);

    std::vector<PlayingAudio> still_playing = std::vector<PlayingAudio>();
    for (unsigned int i = 0; i < playing.size(); ++i)
    {
      bool continue_playing =
        mix_to_buffer(&playing[i], stereo_buffer, buffer_frames);
      if (continue_playing)
        still_playing.push_back(playing[i]);
    }
    playing = still_playing;

    if (music_on)
      mix_to_buffer(&music, stereo_buffer, buffer_frames);

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

void
AudioServer::set_music(AudioTrack *_music)
{
  music.track = _music;
  music.offset = 0;
  music.loop = true;
}

void
AudioServer::start_music()
{
  music_on = true;
}

void
AudioServer::stop_music()
{
  music_on = false;
}
