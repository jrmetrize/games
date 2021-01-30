#include "resource.h"

#ifdef GAME
#include "graphics.h"
#endif

#include <sstream>
#include <bit>
#include "picosha2.h"

#ifdef RESOURCE_IMPORTER
#define STB_IMAGE_IMPLEMENTATION

#include <ft2build.h>
#include FT_FREETYPE_H
#endif
#include "stb_image.h"

#include <zlib.h>

uint32_t
host_to_nbo(const uint32_t &x)
{
  if constexpr (std::endian::native == std::endian::big)
  {
    return x;
  }
  else if constexpr (std::endian::native == std::endian::little)
  {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&x);
    uint32_t result = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8)
      | bytes[3];
    return result;
  }
}

int32_t
host_to_nbo(const int32_t &x)
{
  if constexpr (std::endian::native == std::endian::big)
  {
    return x;
  }
  else if constexpr (std::endian::native == std::endian::little)
  {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&x);
    uint32_t result = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8)
      | bytes[3];
    return result;
  }
}

uint32_t
nbo_to_host(const uint32_t &x)
{
  if constexpr (std::endian::native == std::endian::big)
  {
    return x;
  }
  else if constexpr (std::endian::native == std::endian::little)
  {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&x);
    uint32_t result = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8)
      | bytes[3];
    return result;
  }
}

int32_t
nbo_to_host(const int32_t &x)
{
  if constexpr (std::endian::native == std::endian::big)
  {
    return x;
  }
  else if constexpr (std::endian::native == std::endian::little)
  {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&x);
    uint32_t result = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8)
      | bytes[3];
    return result;
  }
}

Resource::~Resource()
{

}

#ifdef RESOURCE_IMPORTER
Image::Image(std::string path) :
  stb_allocated(true)
{
  {
    // TODO: handle errors
    int x;
    int y;
    int n;
    data = stbi_load(path.c_str(), &x, &y, &n, 0);
    width = uint32_t(x);
    height = uint32_t(y);
    channels = uint32_t(n);
  }
}
#endif

Image::Image(uint32_t _width, uint32_t _height, uint32_t _channels,
  const unsigned char *_data) :
#ifdef GAME
  width(_width), height(_height), channels(_channels), stb_allocated(false),
  texture(nullptr)
#else
  width(_width), height(_height), channels(_channels), stb_allocated(false)
#endif
{
  data = new unsigned char[width * height * channels];
  memcpy(data, _data, sizeof(unsigned char) * width * height * channels);
}

Image::~Image()
{
  if (stb_allocated)
    stbi_image_free(data);
  else
    delete[] data;
#ifdef GAME
  if (texture != nullptr)
    delete texture;
#endif
}

Resource *
Image::duplicate() const
{
  return new Image(width, height, channels, data);
}

std::string
Image::get_type() const
{
  return "image";
}

#ifdef GAME
void
Image::generate_texture()
{
  if (texture == nullptr)
    texture = new Texture(width, height, channels, data);
}

const Texture *
Image::get_texture()
{
  return texture;
}
#endif

Image *
Image::from_data(const char *data, uint32_t length)
{
  uint32_t width_nbo = *reinterpret_cast<const uint32_t *>(&data[0]);
  uint32_t height_nbo = *reinterpret_cast<const uint32_t *>(&data[4]);
  uint32_t channels_nbo = *reinterpret_cast<const uint32_t *>(&data[8]);
  const unsigned char *img_data =
    reinterpret_cast<const unsigned char *>(&data[12]);
  return new Image(nbo_to_host(width_nbo), nbo_to_host(height_nbo),
    nbo_to_host(channels_nbo), img_data);
}

#ifdef RESOURCE_IMPORTER
uint32_t
Image::append_to(std::ostream &out) const
{
  {
    uint32_t width_nbo = host_to_nbo(width);
    out.write(reinterpret_cast<const char *>(&width_nbo), sizeof(width_nbo));
  }
  {
    uint32_t height_nbo = host_to_nbo(height);
    out.write(reinterpret_cast<const char *>(&height_nbo), sizeof(height_nbo));
  }
  {
    uint32_t channels_nbo = host_to_nbo(channels);
    out.write(reinterpret_cast<const char *>(&channels_nbo), sizeof(channels_nbo));
  }
  out.write(reinterpret_cast<const char *>(data),
    sizeof(unsigned char) * width * height * channels);
  return 4 + 4 + 4 + (width * height * channels);
}
#endif

const std::string FontFace::chars
  = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

#ifdef RESOURCE_IMPORTER
FontFace::FontFace(std::string path) :
#ifdef GAME
  glyph_map(),
  kerning_table(),
  texture_map()
#else
  glyph_map(),
  kerning_table()
#endif
{
  // TODO: handle freetype errors
  FT_Library font_library;
  int error = FT_Init_FreeType(&font_library);

  FT_Face font_face;
  error = FT_New_Face(font_library, path.c_str(), 0, &font_face);

  // Render at 64 pixels for SDFs. This should be fine for all scales.
  error = FT_Set_Pixel_Sizes(font_face, 0, 64);

  for (unsigned int i = 0; i < chars.length(); ++i)
  {
    char c = chars[i];
    unsigned int glyph_index = FT_Get_Char_Index(font_face, c);
    error = FT_Load_Glyph(font_face, glyph_index, 0);
    error = FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_SDF);

    Glyph glyph = {};
    glyph.bitmap_width = font_face->glyph->bitmap.width;
    glyph.bitmap_height = font_face->glyph->bitmap.rows;
    uint32_t bitmap_size = glyph.bitmap_width * glyph.bitmap_height;
    glyph.bitmap_data = new unsigned char[bitmap_size];

    // If we are rendering SDFs, the FT bitmap has 16 bit gray values, so
    // we need to convert these to 8 bit gray values
    if (true)
    {
      for (unsigned int i = 0; i < glyph.bitmap_width; ++i)
      {
        for (unsigned int j = 0; j < glyph.bitmap_height; ++j)
        {
          int16_t gray_16 = reinterpret_cast<uint16_t *>(font_face->glyph->bitmap.buffer)[i + (glyph.bitmap_width * j)];
          glyph.bitmap_data[i + (glyph.bitmap_width * j)] = (gray_16 + 32768) >> 8;
        }
      }
    }
    else
    {
      memcpy(glyph.bitmap_data, font_face->glyph->bitmap.buffer, bitmap_size);
    }

    glyph.width = font_face->glyph->metrics.width;
    glyph.height = font_face->glyph->metrics.height;

    glyph.horizontal_bearing_x = font_face->glyph->metrics.horiBearingX;
    glyph.horizontal_bearing_y = font_face->glyph->metrics.horiBearingY;
    glyph.horizontal_advance = font_face->glyph->metrics.horiAdvance;

    glyph.vertical_bearing_x = font_face->glyph->metrics.vertBearingX;
    glyph.vertical_bearing_y = font_face->glyph->metrics.vertBearingY;
    glyph.vertical_advance = font_face->glyph->metrics.vertAdvance;

    glyph_map[c] = glyph;

    // Loop through all the characters again to get kerning values
    for (unsigned int j = 0; j < chars.length(); ++j)
    {
      char c2 = chars[j];
      FT_Vector v = {};
      unsigned int glyph2_index = FT_Get_Char_Index(font_face, c2);

      FT_Get_Kerning(font_face, glyph_index, glyph2_index, FT_KERNING_UNFITTED, &v);

      Kerning entry = {};
      entry.x = v.x;
      entry.y = v.y;

      kerning_table[std::pair<char, char>(c, c2)] = entry;
    }
  }

  FT_Done_Face(font_face);
  FT_Done_FreeType(font_library);
}
#endif

FontFace::FontFace() :
#ifdef GAME
  glyph_map(),
  kerning_table(),
  texture_map()
#else
  glyph_map(),
  kerning_table()
#endif
{

}

FontFace::~FontFace()
{
  for (const std::pair<char, Glyph> &x : glyph_map)
  {
    if (x.second.bitmap_data != nullptr)
      delete[] x.second.bitmap_data;
  }
#ifdef GAME
  for (const std::pair<char, Texture *> &x : texture_map)
  {
    if (x.second != nullptr)
      delete x.second;
  }
#endif
}

const Glyph &
FontFace::get_glyph(char c)
{
  return glyph_map[c];
}

const FontFace::Kerning &
FontFace::get_kerning(char a, char b)
{
  return kerning_table[std::pair<char, char>(a, b)];
}

#ifdef GAME
void
FontFace::generate_textures()
{
  for (const std::pair<char, Glyph> &x : glyph_map)
  {
    if (texture_map.find(x.first) != texture_map.end())
      continue;
    texture_map[x.first] = new Texture(x.second.bitmap_width, x.second.bitmap_height, 1, x.second.bitmap_data);
  }
}

const Texture *
FontFace::get_texture(char c)
{
  return texture_map[c];
}
#endif

void
FontFace::add_glyph(const char &c, const Glyph &glyph)
{
  // First, duplicate the bitmap, then add the glyph to the map
  Glyph g = Glyph(glyph);
  g.bitmap_data = new unsigned char[g.bitmap_width * g.bitmap_height];
  memcpy(g.bitmap_data, glyph.bitmap_data,
    sizeof(unsigned char) * g.bitmap_width * g.bitmap_height);
  glyph_map[c] = g;
}

Resource *
FontFace::duplicate() const
{
  FontFace *font = new FontFace();
  for (const std::pair<char, Glyph> &x : glyph_map)
    font->add_glyph(x.first, x.second);
  return font;
}

std::string
FontFace::get_type() const
{
  return "font_face";
}

FontFace *
FontFace::from_data(const char *data, uint32_t length)
{
  FontFace *font = new FontFace();

  uint32_t num_chars = nbo_to_host(*reinterpret_cast<const uint32_t *>(&data[0]));
  uint32_t current_offset = 4;
  for (unsigned int i = 0; i < num_chars; ++i)
  {
    Glyph g = {};

    char c = data[current_offset];
    uint32_t offset = nbo_to_host(*reinterpret_cast<const uint32_t *>(&data[current_offset + 1]));
    g.bitmap_width = nbo_to_host(*reinterpret_cast<const uint32_t *>(&data[current_offset + 5]));
    g.bitmap_height = nbo_to_host(*reinterpret_cast<const uint32_t *>(&data[current_offset + 9]));
    g.width = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 13]));
    g.height = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 17]));
    g.horizontal_bearing_x = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 21]));
    g.horizontal_bearing_y = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 25]));
    g.horizontal_advance = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 29]));
    g.vertical_bearing_x = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 33]));
    g.vertical_bearing_y = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 37]));
    g.vertical_advance = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 41]));

    uint32_t bitmap_size = g.bitmap_width * g.bitmap_height;

    g.bitmap_data = new unsigned char[bitmap_size];
    memcpy(g.bitmap_data, &data[offset], bitmap_size);

    font->glyph_map[c] = g;

    current_offset += 1 + (4 * 11);
  }

  // Extract kerning table data
  uint32_t kerning_table_length = nbo_to_host(*reinterpret_cast<const uint32_t *>(&data[current_offset]));
  current_offset += 4;
  for (unsigned int i = 0; i < kerning_table_length; ++i)
  {
    char c1 = data[current_offset];
    char c2 = data[current_offset + 1];
    int32_t x = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 2]));
    int32_t y = nbo_to_host(*reinterpret_cast<const int32_t *>(&data[current_offset + 6]));

    Kerning k = {};
    k.x = x;
    k.y = y;
    font->kerning_table[std::pair<char, char>(c1, c2)] = k;

    current_offset += 10;
  }

  return font;
}

#ifdef RESOURCE_IMPORTER
uint32_t
FontFace::append_to(std::ostream &out) const
{
  // We will add a table at the beginning that describes the metrics of each glyph,
  // but we need to know the various offsets and sizes, so we will just allocate
  // space now and fill it in at the end.
  uint32_t header_size = 4 + (glyph_map.size() * (1 + (4 * 11)));
  uint32_t binary_size = header_size;
  for (uint32_t i = 0; i < header_size; ++i)
    out.put(0x00);

  // After the header, write the kerning table (10 bytes per pair)
  uint32_t current_offset = header_size + 4 + (10 * kerning_table.size());
  binary_size += 4 + (10 * kerning_table.size());
  {
    uint32_t kerning_table_length_nbo = host_to_nbo(uint32_t(kerning_table.size()));
    out.write(reinterpret_cast<char *>(&kerning_table_length_nbo), sizeof(kerning_table_length_nbo));
  }
  for (const std::pair<std::pair<char, char>, Kerning> &x : kerning_table)
  {
    // Put the two characters
    out.write(&x.first.first, sizeof(char));
    out.write(&x.first.second, sizeof(char));
    {
      int32_t x_nbo = host_to_nbo(x.second.x);
      out.write(reinterpret_cast<char *>(&x_nbo), sizeof(x_nbo));
    }
    {
      int32_t y_nbo = host_to_nbo(x.second.y);
      out.write(reinterpret_cast<char *>(&y_nbo), sizeof(y_nbo));
    }
  }

  // Now, write the bitmaps
  std::map<char, uint32_t> offsets = std::map<char, uint32_t>();
  for (const std::pair<char, Glyph> &x : glyph_map)
  {
    offsets[x.first] = current_offset;
    uint32_t bitmap_size = sizeof(unsigned char) * x.second.bitmap_width * x.second.bitmap_height;
    out.write(reinterpret_cast<char *>(x.second.bitmap_data), bitmap_size);
    current_offset += bitmap_size;
    binary_size += bitmap_size;
  }

  out.seekp(0);
  {
    uint32_t chars_nbo = host_to_nbo(uint32_t(chars.length()));
    out.write(reinterpret_cast<char *>(&chars_nbo), sizeof(chars_nbo));
  }
  for (const std::pair<char, Glyph> &x : glyph_map)
  {
    // Write the character, metrics, bitmap offset, and size
    out.write(&x.first, sizeof(char));
    // Offset first, then the order in the declaration of Glyph
    {
      uint32_t offset_nbo = host_to_nbo(offsets[x.first]);
      out.write(reinterpret_cast<char *>(&offset_nbo), sizeof(offset_nbo));
    }
    {
      uint32_t bitmap_width_nbo = host_to_nbo(x.second.bitmap_width);
      out.write(reinterpret_cast<char *>(&bitmap_width_nbo), sizeof(bitmap_width_nbo));
    }
    {
      uint32_t bitmap_height_nbo = host_to_nbo(x.second.bitmap_height);
      out.write(reinterpret_cast<char *>(&bitmap_height_nbo), sizeof(bitmap_height_nbo));
    }
    {
      int32_t width_nbo = host_to_nbo(x.second.width);
      out.write(reinterpret_cast<char *>(&width_nbo), sizeof(width_nbo));
    }
    {
      int32_t height_nbo = host_to_nbo(x.second.height);
      out.write(reinterpret_cast<char *>(&height_nbo), sizeof(height_nbo));
    }
    {
      int32_t horizontal_bearing_x_nbo = host_to_nbo(x.second.horizontal_bearing_x);
      out.write(reinterpret_cast<char *>(&horizontal_bearing_x_nbo), sizeof(horizontal_bearing_x_nbo));
    }
    {
      int32_t horizontal_bearing_y_nbo = host_to_nbo(x.second.horizontal_bearing_y);
      out.write(reinterpret_cast<char *>(&horizontal_bearing_y_nbo), sizeof(horizontal_bearing_y_nbo));
    }
    {
      int32_t horizontal_advance_nbo = host_to_nbo(x.second.horizontal_advance);
      out.write(reinterpret_cast<char *>(&horizontal_advance_nbo), sizeof(horizontal_advance_nbo));
    }
    {
      int32_t vertical_bearing_x_nbo = host_to_nbo(x.second.vertical_bearing_x);
      out.write(reinterpret_cast<char *>(&vertical_bearing_x_nbo), sizeof(vertical_bearing_x_nbo));
    }
    {
      int32_t vertical_bearing_y_nbo = host_to_nbo(x.second.vertical_bearing_y);
      out.write(reinterpret_cast<char *>(&vertical_bearing_y_nbo), sizeof(vertical_bearing_y_nbo));
    }
    {
      int32_t vertical_advance_nbo = host_to_nbo(x.second.vertical_advance);
      out.write(reinterpret_cast<char *>(&vertical_advance_nbo), sizeof(vertical_advance_nbo));
    }
  }
  return binary_size;
}
#endif

Text::Text(std::string path)
{
  std::ifstream file = std::ifstream(path);
  std::stringstream buffer = std::stringstream();
  buffer << file.rdbuf();
  text = buffer.str();
  file.close();
}

Text::Text() :
  text()
{

}

Text::~Text()
{

}

const std::string &
Text::get_text() const
{
  return text;
}

Resource *
Text::duplicate() const
{
  Text *t = new Text();
  t->text = text;
  return t;
}

std::string
Text::get_type() const
{
  return "text";
}

Text *
Text::from_data(const char *data, uint32_t length)
{
  uint32_t text_length = nbo_to_host(*reinterpret_cast<const uint32_t *>(&data[0]));
  Text *t = new Text();
  t->text = std::string(&data[4], text_length);
  return t;
}

#ifdef RESOURCE_IMPORTER
uint32_t
Text::append_to(std::ostream &out) const
{
  uint32_t length_nbo = host_to_nbo(uint32_t(text.length()));

  out.write(reinterpret_cast<char *>(&length_nbo), sizeof(length_nbo));
  out.write(text.data(), text.length());

  return 4 + text.length();
}
#endif

ResourceBundle::ResourceBundle() :
  resources()
{

}

ResourceBundle::ResourceBundle(std::string path) :
  resources()
{
  std::ifstream file = std::ifstream(path, std::ios::binary);

  // First, parse the header
  uint32_t table_length;
  Header header = {};
  {
    uint32_t magic_number_nbo;
    file.read(reinterpret_cast<char *>(&magic_number_nbo), sizeof(magic_number_nbo));
  }
  {
    uint32_t version_nbo;
    file.read(reinterpret_cast<char *>(&version_nbo), sizeof(version_nbo));
    header.bundle_version = nbo_to_host(version_nbo);
  }
  {
    uint32_t table_length_nbo;
    file.read(reinterpret_cast<char *>(&table_length_nbo), sizeof(table_length_nbo));
    table_length = nbo_to_host(table_length_nbo);
  }

  for (unsigned int i = 0; i < table_length; ++i)
  {
    HeaderResourceDescriptor entry = {};
    {
      uint32_t name_length_nbo;
      file.read(reinterpret_cast<char *>(&name_length_nbo), sizeof(name_length_nbo));
      uint32_t name_length = nbo_to_host(name_length_nbo);
      char *name_buf = new char[name_length];
      file.read(name_buf, name_length);
      entry.resource_name = std::string(name_buf);
      delete[] name_buf;
    }
    {
      uint32_t type_length_nbo;
      file.read(reinterpret_cast<char *>(&type_length_nbo), sizeof(type_length_nbo));
      uint32_t type_length = nbo_to_host(type_length_nbo);
      char *type_buf = new char[type_length];
      file.read(type_buf, type_length);
      entry.resource_type = std::string(type_buf);
      delete[] type_buf;
    }
    {
      uint32_t offset_nbo;
      file.read(reinterpret_cast<char *>(&offset_nbo), sizeof(offset_nbo));
      entry.offset = nbo_to_host(offset_nbo);
    }
    {
      uint32_t compressed_size_nbo;
      file.read(reinterpret_cast<char *>(&compressed_size_nbo), sizeof(compressed_size_nbo));
      entry.compressed_size = nbo_to_host(compressed_size_nbo);
    }
    {
      uint32_t size_nbo;
      file.read(reinterpret_cast<char *>(&size_nbo), sizeof(size_nbo));
      entry.size = nbo_to_host(size_nbo);
    }
    header.entries.push_back(entry);
  }

  // We have all the information necessary to decompress each resource
  for (unsigned int i = 0; i < header.entries.size(); ++i)
  {
    file.seekg(header.entries[i].offset);
    unsigned char *uncompressed = new unsigned char[header.entries[i].size];
    unsigned long uncompressed_size = header.entries[i].size;

    char *compressed = new char[header.entries[i].compressed_size];
    unsigned long compressed_size = header.entries[i].compressed_size;

    file.read(compressed, header.entries[i].compressed_size);

    int err = uncompress2(uncompressed, &uncompressed_size,
      reinterpret_cast<unsigned char *>(compressed), &compressed_size);

    if (header.entries[i].resource_type == "image")
    {
      resources[header.entries[i].resource_name] =
        Image::from_data(reinterpret_cast<char *>(uncompressed), header.entries[i].size);
    }
    else if (header.entries[i].resource_type == "font_face")
    {
      resources[header.entries[i].resource_name] =
        FontFace::from_data(reinterpret_cast<char *>(uncompressed), header.entries[i].size);
    }
    else if (header.entries[i].resource_type == "text")
    {
      resources[header.entries[i].resource_name] =
        Text::from_data(reinterpret_cast<char *>(uncompressed), header.entries[i].size);
    }
    else
    {
      // TODO: handle unsupported types
    }

    delete[] uncompressed;
    delete[] compressed;
  }

  file.close();
}

ResourceBundle::~ResourceBundle()
{
  for (const std::pair<std::string, Resource *> &x : resources)
    delete x.second;
}

Resource *
ResourceBundle::get_resource(std::string name)
{
  if (resources.find(name) == resources.end())
    return nullptr;
  return resources[name];
}

void
ResourceBundle::add_resource(std::string name, const Resource *resource)
{
  // TODO: search the map for something with this name already and throw
  // an exception if this already exists
  resources[name] = resource->duplicate();
}

#ifdef RESOURCE_IMPORTER
void
ResourceBundle::write_to(std::string path)
{
  std::ofstream file = std::ofstream();
  file.open(path, std::ios_base::binary);
  write_to(file);
  file.close();
}

void
ResourceBundle::write_to(std::ostream &out)
{
  uint32_t header_size = 4 + 4 + 4;
  Header header = {};
  header.bundle_version = 1;
  for (const std::pair<std::string, Resource *> &x : resources)
  {
    HeaderResourceDescriptor entry = {};
    entry.resource_name = x.first;
    entry.resource_type = x.second->get_type();
    entry.offset = 0;
    entry.compressed_size = 0;
    entry.size = 0;

    header.entries.push_back(entry);

    header_size += x.first.length() + 5;
    header_size += x.second->get_type().length() + 5;
    header_size += 4 + 4 + 4;
  }

  // Temporarily zero out the header
  for (unsigned int i = 0; i < header_size; ++i)
    out.put(0x00);

  // Compress each resource and write it to the body
  uint32_t current_offset = header_size;
  for (unsigned int i = 0; i < header.entries.size(); ++i)
  {
    Resource *r = resources[header.entries[i].resource_name];
    std::stringstream resource_out = std::stringstream();

    uint32_t raw_size = r->append_to(resource_out);
    unsigned char *raw_data = new unsigned char[raw_size];
    memcpy(raw_data, resource_out.str().data(), raw_size);

    unsigned long compressed_size = compressBound((unsigned long)raw_size);
    unsigned char *compressed = new unsigned char[compressed_size];
    compress2(compressed, &compressed_size, raw_data, (unsigned long)raw_size, 9);

    delete[] raw_data;

    out.write(reinterpret_cast<const char *>(compressed), compressed_size);

    delete[] compressed;

    header.entries[i].offset = current_offset;
    header.entries[i].compressed_size = uint32_t(compressed_size);
    header.entries[i].size = raw_size;

    current_offset += compressed_size;
  }

  // Now that we have the data, go back and fill in the header with sizes
  out.seekp(0);

  // Magic number
  out.put(0x46);
  out.put(0x50);
  out.put(0x32);
  out.put(0x44);

  // Version
  {
    uint32_t version_nbo = host_to_nbo(header.bundle_version);
    out.write(reinterpret_cast<const char *>(&version_nbo), sizeof(version_nbo));
  }

  // Table describing resources
  {
    uint32_t table_length_nbo = host_to_nbo(uint32_t(header.entries.size()));
    out.write(reinterpret_cast<const char *>(&table_length_nbo),
      sizeof(table_length_nbo));
  }

  for (unsigned int i = 0; i < header.entries.size(); ++i)
  {
    {
      uint32_t name_length_nbo = host_to_nbo(uint32_t(header.entries[i].resource_name.length() + 1));
      out.write(reinterpret_cast<const char *>(&name_length_nbo),
        sizeof(name_length_nbo));
    }
    out.write(reinterpret_cast<const char *>(header.entries[i].resource_name.c_str()),
      header.entries[i].resource_name.length() + 1);
    {
      uint32_t type_length_nbo = host_to_nbo(uint32_t(header.entries[i].resource_type.length() + 1));
      out.write(reinterpret_cast<const char *>(&type_length_nbo),
        sizeof(type_length_nbo));
    }
    out.write(reinterpret_cast<const char *>(header.entries[i].resource_type.c_str()),
      header.entries[i].resource_type.length() + 1);
    {
      uint32_t offset_nbo = host_to_nbo(header.entries[i].offset);
      out.write(reinterpret_cast<const char *>(&offset_nbo),
        sizeof(offset_nbo));
    }
    {
      uint32_t compressized_size_nbo = host_to_nbo(header.entries[i].compressed_size);
      out.write(reinterpret_cast<const char *>(&compressized_size_nbo),
        sizeof(compressized_size_nbo));
    }
    {
      uint32_t size_nbo = host_to_nbo(header.entries[i].size);
      out.write(reinterpret_cast<const char *>(&size_nbo),
        sizeof(size_nbo));
    }
  }
}
#endif
