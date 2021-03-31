#ifndef RESOURCE_H
#define RESOURCE_H

#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "linear_algebra.h"

// TODO: move these to a util file?
uint32_t
host_to_nbo(const uint32_t &x);

int32_t
host_to_nbo(const int32_t &x);

uint32_t
nbo_to_host(const uint32_t &x);

int32_t
nbo_to_host(const int32_t &x);

#ifdef GAME
class Texture;
#endif

class Resource
{
public:
  virtual
  ~Resource() = 0;

  virtual Resource *
  duplicate() const = 0;

  virtual std::string
  get_type() const = 0;

#ifdef RESOURCE_IMPORTER
  virtual uint32_t // returns size
  append_to(std::ostream &out) const = 0;
#endif
};

class Image : public Resource
{
  uint32_t width;
  uint32_t height;
  uint32_t channels;
  unsigned char *data;

  bool stb_allocated;

#ifdef GAME
  Texture *texture;
#endif
public:
#ifdef RESOURCE_IMPORTER
  Image(std::string path);
#endif

  Image(uint32_t _width, uint32_t _height, uint32_t _channels,
    const unsigned char *_data);

  ~Image();

  Resource *
  duplicate() const;

  std::string
  get_type() const;

#ifdef GAME
  void
  generate_texture();

  const Texture *
  get_texture();
#endif

  static Image *
  from_data(const char *data, uint32_t length);

#ifdef RESOURCE_IMPORTER
  uint32_t
  append_to(std::ostream &out) const;
#endif
};

struct Glyph
{
  uint32_t bitmap_width;
  uint32_t bitmap_height;
  unsigned char *bitmap_data;

  int32_t width;
  int32_t height;

  int32_t horizontal_bearing_x;
  int32_t horizontal_bearing_y;
  int32_t horizontal_advance;

  int32_t vertical_bearing_x;
  int32_t vertical_bearing_y;
  int32_t vertical_advance;
};

class FontFace : public Resource
{
public:
  struct Kerning
  {
    int32_t x;
    int32_t y;
  };
private:
  const static std::string chars;

  std::map<char, Glyph> glyph_map;
  std::map<std::pair<char, char>, Kerning> kerning_table;
  #ifdef GAME
  std::map<char, Texture *> texture_map;
  #endif

  void
  add_glyph(const char &c, const Glyph &glyph);
public:
#ifdef RESOURCE_IMPORTER
  FontFace(std::string path);
#endif

  FontFace();

  ~FontFace();

  const Glyph &
  get_glyph(char c);

  const Kerning &
  get_kerning(char a, char b);

#ifdef GAME
  void
  generate_textures();

  const Texture *
  get_texture(char c);
#endif

  Resource *
  duplicate() const;

  std::string
  get_type() const;

  static FontFace *
  from_data(const char *data, uint32_t length);

#ifdef RESOURCE_IMPORTER
  uint32_t
  append_to(std::ostream &out) const;
#endif
};

class Text : public Resource
{
  std::string text;
public:
  Text(std::string path);

  Text();

  ~Text();

  const std::string &
  get_text() const;

  Resource *
  duplicate() const;

  std::string
  get_type() const;

  static Text *
  from_data(const char *data, uint32_t length);

#ifdef RESOURCE_IMPORTER
  uint32_t
  append_to(std::ostream &out) const;
#endif
};

class AudioTrack : public Resource
{
  // TODO: arbitrary sample rates and channels? For now, just assume mono/stereo
  // and 48khz
  unsigned int channels;
  std::vector<int16_t> samples;
  std::vector<unsigned char> ogg_data;
public:
#ifdef RESOURCE_IMPORTER
  AudioTrack(std::string path);
#endif

  AudioTrack();

  ~AudioTrack();

  Resource *
  duplicate() const;

  std::string
  get_type() const;

  unsigned int
  get_channels() const;

  unsigned int
  get_frames() const;

  const std::vector<int16_t> &
  get_samples() const;

  static AudioTrack *
  from_data(const char *data, uint32_t length);

#ifdef RESOURCE_IMPORTER
  uint32_t
  append_to(std::ostream &out) const;
#endif
};

struct Vertex
{
  Vec3 position;
  Vec2 texture_coordinates;

  Vec3 normal;

  Vertex(const Vec3 &_position, const Vec2 &_texture_coordinates);

  Vertex(Vec3 _position, Vec2 _texture_coordinates, Vec3 _normal);
};

using VertexVector = std::vector<Vertex>;
using IndexVector = std::vector<unsigned int>;

struct MaterialData
{
  Vec3 diffuse_color;
  uint32_t vertices;
};

struct Mesh
{
  VertexVector vertices;
  IndexVector indices;

  std::vector<MaterialData> materials;

  Mesh();

  Mesh(const VertexVector &_vertices, const IndexVector &_indices);

  static Mesh *
  primitive_quad();

  static Mesh *
  primitive_cube();
};

class Scene : public Resource
{
  Mesh data;
public:
#ifdef RESOURCE_IMPORTER
  Scene(std::string path);
#endif

  Scene();

  ~Scene();

  Resource *
  duplicate() const;

  std::string
  get_type() const;

  static Scene *
  from_data(const char *data, uint32_t length);

  Mesh *
  get_mesh();

#ifdef RESOURCE_IMPORTER
  uint32_t
  append_to(std::ostream &out) const;
#endif
};

class ResourceBundle
{
  std::map<std::string, Resource *> resources;

  struct HeaderResourceDescriptor
  {
    std::string resource_name;
    std::string resource_type;
    uint32_t offset;
    uint32_t compressed_size;
    uint32_t size;
  };

  struct Header
  {
    uint32_t bundle_version;
    std::vector<HeaderResourceDescriptor> entries;
  };
public:
  ResourceBundle();

  ResourceBundle(std::string path);

  ~ResourceBundle();

  Resource *
  get_resource(std::string name);

  void
  add_resource(std::string name, const Resource *resource);

#ifdef RESOURCE_IMPORTER
  void
  write_to(std::string path);

  void
  write_to(std::ostream &out);
#endif
};

#endif
