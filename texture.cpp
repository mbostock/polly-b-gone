// -*- C++ -*-

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <SDL/sdl.h>
#include <SDL_image/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "resource.h"
#include "texture.h"

using namespace mbostock;

class TextureImpl : public Texture {
public:
  TextureImpl(const char* path);
  virtual ~TextureImpl();

  void initialize();
  virtual void bind() const;

private:
  GLuint id_;
  bool alpha_;
  const std::string path_;
};

TextureImpl::TextureImpl(const char* path)
    : id_(GL_NONE), alpha_(false), path_(path) {
}

TextureImpl::~TextureImpl() {
  if (id_ != GL_NONE) {
    glDeleteTextures(1, &id_);
  }
}

void TextureImpl::initialize() {
  if (id_ != GL_NONE) {
    glDeleteTextures(1, &id_);
  }

  std::string path(Resources::path());
  path.append(path_);
  SDL_Surface* image = IMG_Load(path.c_str());
  if (image == NULL) {
    std::cerr << "Couldn't load " << path << ": " << SDL_GetError() << "\n";
    id_ = GL_NONE;
    return;
  }

	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  alpha_ = (image->format->BytesPerPixel == 4);
  GLenum byteOrder = alpha_
          ? ((image->format->Rmask == 0x000000FF) ? GL_RGBA : GL_BGRA)
          : ((image->format->Rmask == 0x000000FF) ? GL_RGB : GL_BGR);

	glTexImage2D(GL_TEXTURE_2D, 0,
      image->format->BytesPerPixel, image->w, image->h, 0,
      byteOrder, GL_UNSIGNED_BYTE, image->pixels);

  gluBuild2DMipmaps(GL_TEXTURE_2D,
      image->format->BytesPerPixel, image->w, image->h,
      byteOrder, GL_UNSIGNED_BYTE, image->pixels);

  SDL_FreeSurface(image);
}

void TextureImpl::bind() const {
  if (alpha_) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  glBindTexture(GL_TEXTURE_2D, id_);
}

static std::vector<TextureImpl*>& textures() {
  static std::vector<TextureImpl*> v;
  return v;
}

const Texture& Textures::fromFile(const char* path) {
  TextureImpl* texture = new TextureImpl(path);
  textures().push_back(texture);
  return *texture;
}

void Textures::initialize() {
  std::vector<TextureImpl*>::const_iterator i;
  for (i = textures().begin(); i != textures().end(); i++) {
    (*i)->initialize();
  }
}
