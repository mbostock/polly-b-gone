// -*- C++ -*-

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "tinyxml/tinyxml.h"

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
static int windowWidth = defaultWidth;
static int windowHeight = defaultHeight;
static const float kd = .060f; // frame-rate dependent

static bool run = true;
static bool fullScreen = false;

static World* world = NULL;
static bool wireframe = false;

static SDL_Window* window = NULL;
static SDL_GLContext glContext = NULL;

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
  if (width == 0 || height == 0) {
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    width = screenWidth = mode.w;
    height = screenHeight = mode.h;
  }

  windowWidth = width;
  windowHeight = height;

  // Get actual drawable size (may differ from window size on HiDPI displays)
  int drawableWidth, drawableHeight;
  SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);

  glViewport(0, 0, drawableWidth, drawableHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.f, drawableWidth / (float) drawableHeight, 1.0f, 100.f);
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
  SDL_GL_SwapWindow(window);
}

static void toggleShader() {
  shaderi = (shaderi + 1) % shadern;
  shader()->initialize();
}

static void toggleFullScreen() {
  fullScreen = !fullScreen;
  if (fullScreen) {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_GL_GetDrawableSize(window, &windowWidth, &windowHeight);
    resizeSurface(windowWidth, windowHeight);
  } else {
    SDL_SetWindowFullscreen(window, 0);
    SDL_SetWindowSize(window, defaultWidth, defaultHeight);
    SDL_ShowCursor(SDL_ENABLE);
    resizeSurface(defaultWidth, defaultHeight);
  }
}

static void handleKeyDown(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_LEFT: {
      if (event->key.keysym.mod & KMOD_GUI) {
        world->previousRoom();
      }
      break;
    }
    case SDLK_DOWN: {
      if (event->key.keysym.mod & KMOD_GUI) {
        world->resetPlayer();
      }
      break;
    }
    case SDLK_RIGHT: {
      if (event->key.keysym.mod & KMOD_GUI) {
        world->nextRoom();
      }
      break;
    }
    case SDLK_a: world->player().move(Player::LEFT); break;
    case SDLK_s: world->player().move(Player::BACKWARD); break;
    case SDLK_d: world->player().move(Player::RIGHT); break;
    case SDLK_w: world->player().move(Player::FORWARD); break;
    default: break;
  }
}

static void handleKeyUp(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    case SDLK_a: world->player().stop(Player::LEFT); break;
    case SDLK_s: world->player().stop(Player::BACKWARD); break;
    case SDLK_d: world->player().stop(Player::RIGHT); break;
    case SDLK_w: world->player().stop(Player::FORWARD); break;
    case SDLK_SPACE: world->togglePaused(); break;
    case SDLK_q: if (!(event->key.keysym.mod & KMOD_GUI)) break;
    case SDLK_ESCAPE: run = false; break;
    case SDLK_F9: toggleShader(); break;
    case SDLK_F10: world->toggleDebug(); break;
    case SDLK_F11: toggleFullScreen(); break;
    default: break;
  }
}

static void handleMouseDown(SDL_Event* event) {
  int x = event->button.x;
  int y = event->button.y;
  int centerX = windowWidth / 2;
  int centerY = windowHeight / 2;

  // Horizontal: left/right turning
  if (x < centerX - windowWidth / 6) {
    world->player().move(Player::LEFT);
  } else if (x > centerX + windowWidth / 6) {
    world->player().move(Player::RIGHT);
  }

  // Vertical: forward/backward movement
  if (y < centerY - windowHeight / 6) {
    world->player().move(Player::FORWARD);
  } else if (y > centerY + windowHeight / 6) {
    world->player().move(Player::BACKWARD);
  }
}

static void handleMouseUp(SDL_Event* event) {
  world->player().stop();
}

static void handleQuit() {
  Sounds::dispose();
  delete world;
  if (glContext) SDL_GL_DeleteContext(glContext);
  if (window) SDL_DestroyWindow(window);
  SDL_Quit();
}

static void eventLoop() {
  SDL_Event event;
  while (run) {
    handleDisplay();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_WINDOWEVENT: {
          if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            resizeSurface(event.window.data1, event.window.data2);
          }
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
        case SDL_MOUSEBUTTONDOWN: {
          handleMouseDown(&event);
          break;
        }
        case SDL_MOUSEBUTTONUP: {
          handleMouseUp(&event);
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

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  // Request legacy OpenGL profile for compatibility
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

  window = SDL_CreateWindow(
    "POLLY-B-GONE",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    defaultWidth, defaultHeight,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
  );

  if (!window) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return 1;
  }

  glContext = SDL_GL_CreateContext(window);
  if (!glContext) {
    fprintf(stderr, "Failed to create GL context: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GL_SetSwapInterval(1);

  Sounds::initialize();
  world = Worlds::fromFile("world.xml");
  resizeSurface(defaultWidth, defaultHeight);
  eventLoop();

  return 0;
}
