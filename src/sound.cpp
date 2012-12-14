// -*- C++ -*-

#include <SDL/SDL_error.h>
#include <SDL_mixer/SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>

#include "resource.h"
#include "sound.h"

using namespace mbostock;

namespace mbostock {

  class SoundImpl : public Sound {
  public:
    SoundImpl(const char* path);

    inline const char* path() const { return path_.c_str(); }

  private:
    std::string path_;
  };

  class MusicImpl : public SoundImpl {
  public:
    MusicImpl(const char* path);
    virtual ~MusicImpl();

    virtual void play(int loops) const;
    virtual void stop() const;

  private:
    Mix_Music* music_;
  };

  class ChunkImpl : public SoundImpl {
  public:
    ChunkImpl(const char* path);
    virtual ~ChunkImpl();

    virtual void play(int loops) const;
    virtual void stop() const;

  private:
    Mix_Chunk* chunk_;
    mutable int channel_;
  };

}

SoundImpl::SoundImpl(const char* path)
    : path_(path) {
}

MusicImpl::MusicImpl(const char* path)
    : SoundImpl(path) {
  std::string fullPath(Resources::path());
  fullPath.append(path);
  music_ = Mix_LoadMUS(fullPath.c_str());
  if (music_ == NULL) {
    std::cerr << SDL_GetError() << "\n";
  }
}

MusicImpl::~MusicImpl() {
  if (music_ != NULL) {
    Mix_FreeMusic(music_);
  }
}

void MusicImpl::play(int loops) const {
  stop();
  if (music_ != NULL) {
    Mix_PlayMusic(music_, loops);
  }
}

void MusicImpl::stop() const {
  Mix_HaltMusic();
}

ChunkImpl::ChunkImpl(const char* path)
    : SoundImpl(path), channel_(-1) {
  std::string fullPath(Resources::path());
  fullPath.append(path);
  chunk_ = Mix_LoadWAV(fullPath.c_str());
  if (chunk_ == NULL) {
    std::cerr << SDL_GetError() << "\n";
  }
}

ChunkImpl::~ChunkImpl() {
  if (chunk_ != NULL) {
    Mix_FreeChunk(chunk_);
  }
}

void ChunkImpl::play(int loops) const {
  stop();
  if (chunk_ != NULL) {
    channel_ = Mix_PlayChannel(-1, chunk_, loops);
  }
}

void ChunkImpl::stop() const {
  if (channel_ != -1) {
    Mix_HaltChannel(channel_);
    channel_ = -1;
  }
}

static std::vector<SoundImpl*>& sounds() {
  static std::vector<SoundImpl*> v;
  return v;
}

void Sounds::initialize() {
  Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
}

void Sounds::dispose() {
  Mix_HaltMusic();
  Mix_HaltChannel(-1);
  std::vector<SoundImpl*>::const_iterator i;
  for (i = sounds().begin(); i != sounds().end(); i++) {
    delete *i;
  }
  Mix_CloseAudio();
}

Sound& Sounds::fromFile(const char* path) {
  /* First check to see if we've loaded this sound already. */
  std::vector<SoundImpl*>::const_iterator i;
  for (i = sounds().begin(); i != sounds().end(); i++) {
    SoundImpl& s = **i;
    if (strcmp(s.path(), path) == 0) {
      return s;
    }
  }

  /* If not, load the new sound. */
  std::string spath(path);
  SoundImpl* sound =
      ((spath.rfind(".mid", std::string::npos, 4) != -1)
       || (spath.rfind(".ogg", std::string::npos, 4) != -1)
       || (spath.rfind(".mp3", std::string::npos, 4) != -1))
          ? (SoundImpl*) new MusicImpl(path)
          : (SoundImpl*) new ChunkImpl(path);
  sounds().push_back(sound);
  return *sound;
}

/* It appears that Mix_PauseMusic is unsupported for MIDI. :\ */

void Sounds::pause() {
  Mix_PauseMusic();
  Mix_Pause(-1);
}

void Sounds::resume() {
  Mix_ResumeMusic();
  Mix_Resume(-1);
}
