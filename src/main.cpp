// -*- C++ -*-

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <TinyXML/tinyxml.h>

#include "room.h"
#include "shader.h"
#include "sound.h"
#include "texture.h"
#include "world.h"
#include "worlds.h"

using namespace mbostock;

static const int defaultWidth = 640;
static const int defaultHeight = 480;
static int screenWidth = 0;
static int screenHeight = 0;
static const int defaultX = 50;
static const int defaultY = 50;
static const float kd = .060f; // frame-rate dependent

static bool run = true;
static bool fullScreen = false;

static World* world = NULL;
static bool wireframe = false;

static Shader* shaders[] = {
  Shaders::defaultShader(),
  Shaders::wireframeShader(),
  Shaders::normalShader()
};

static int shaderi = 0;
static const int shadern = 3;

static Shader* shader() {
  return shaders[shaderi];
}

static void resizeSurface(int width, int height) {
  uint32_t flags = SDL_OPENGL | SDL_RESIZABLE;
  if (fullScreen) {
    flags |= SDL_FULLSCREEN;
  }
  SDL_SetVideoMode(width, height, 24, flags);

  /* Store the screen resolution when going into full screen. */
  if (width == 0) {
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    width = screenWidth = info->current_w;
    height = screenHeight = info->current_h;
  }

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.f, width / (float) height, 1.0f, 100.f);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.f, 0.f, 0.f, 0.f);

  shader()->initialize();
  Textures::initialize();
  world->model().initialize();
}

static void handleDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  world->simulate();

  const Vector& p = world->player().origin();
  const Vector& min = world->room().cameraBounds().min();
  const Vector& max = world->room().cameraBounds().max();

  /* Interpolate the eye location. */
  Vector ee(p.x, p.y + 4.f, p.z + 6.f);
  ee = Vector::min(Vector::max(min, ee), max);
  static Vector e = ee;
  e = e * (1.f - kd) + ee * kd;

  /* Interpolate the camera direction towards the player. */
  static Vector c = p;
  c = c * (1.f - kd) + p * kd;

  gluLookAt(e.x, e.y, e.z,
            c.x, c.y, c.z,
            0.f, 1.f, 0.f);

  shader()->display(world->model());
  SDL_GL_SwapBuffers();
}

static void toggleShader() {
  shaderi = (shaderi + 1) % shadern;
  shader()->initialize();
}

static void toggleFullScreen() {
  fullScreen = !fullScreen;
  if (fullScreen) {
    resizeSurface(screenWidth, screenHeight);
    SDL_ShowCursor(SDL_DISABLE);
  } else {
    resizeSurface(defaultWidth, defaultHeight);
    SDL_ShowCursor(SDL_ENABLE);
  }
}

static void handleKeyDown(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_LEFT: {
      if (event->key.keysym.mod & KMOD_META) {
        world->previousRoom();
      }
      break;
    }
    case SDLK_DOWN: {
      if (event->key.keysym.mod & KMOD_META) {
        world->resetPlayer();
      }
      break;
    }
    case SDLK_RIGHT: {
      if (event->key.keysym.mod & KMOD_META) {
        world->nextRoom();
      }
      break;
    }
    case SDLK_a: world->player().move(Player::LEFT); break;
    case SDLK_s: world->player().move(Player::BACKWARD); break;
    case SDLK_d: world->player().move(Player::RIGHT); break;
    case SDLK_w: world->player().move(Player::FORWARD); break;
  }
}

static void handleKeyUp(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_a: world->player().stop(Player::LEFT); break;
    case SDLK_s: world->player().stop(Player::BACKWARD); break;
    case SDLK_d: world->player().stop(Player::RIGHT); break;
    case SDLK_w: world->player().stop(Player::FORWARD); break;
    case SDLK_SPACE: world->togglePaused(); break;
    case SDLK_q: if (!(event->key.keysym.mod & KMOD_META)) break;
    case SDLK_ESCAPE: run = false; break;
    case SDLK_F9: toggleShader(); break;
    case SDLK_F10: world->toggleDebug(); break;
    case SDLK_F11: toggleFullScreen(); break;
  }
}

static void handleQuit() {
  Sounds::dispose();
  delete world;
  SDL_Quit();
}

static void eventLoop() {
  SDL_Event event;
  while (run) {
    handleDisplay();
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_VIDEORESIZE: {
          resizeSurface(event.resize.w, event.resize.h);
          break;
        }
        case SDL_KEYDOWN: {
          handleKeyDown(&event);
          break;
        }
        case SDL_KEYUP: {
          handleKeyUp(&event);
          break;
        }
        case SDL_QUIT: {
          run = false;
          break;
        }
      }
    }
    SDL_Delay(10);
  }
  handleQuit();
  return;
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  SDL_WM_SetCaption("POLLY-B-GONE", "POLLY-B-GONE");

  Sounds::initialize();
  world = Worlds::fromFile("world.xml");
  // resizeSurface(defaultWidth, defaultHeight);
  toggleFullScreen();
  eventLoop();

  return 0;
}
