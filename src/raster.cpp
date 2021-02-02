#include "raster.h"
#include "linear_algebra.h"
#include <functional>
#include <cmath>

struct RawBitmap
{
  unsigned int width;
  unsigned int height;
  unsigned int channels;
  unsigned char *data;
};

Vec2
json_to_vec2(const json &data)
{
  Vec2 v = {};
  v.x = data[0];
  v.y = data[1];
  return v;
}

Vec4
json_to_vec4(const json &data)
{
  Vec4 v = {};
  v.x = data[0];
  v.y = data[1];
  v.z = data[2];
  v.w = data[3];
  return v;
}

unsigned char
float_to_uc(float x)
{
  return (unsigned char)(255.0f * x);
}

// Drawing command implementations
void
triangle_command(RawBitmap &img, const json &cmd)
{
  Vec4 color = json_to_vec4(cmd["color"]);
  Vec2 v1 = json_to_vec2(cmd["vertices"][0]);
  Vec2 v2 = json_to_vec2(cmd["vertices"][1]);
  Vec2 v3 = json_to_vec2(cmd["vertices"][2]);

  Vec2 n1 = (v3 - v2).perp();
  Vec2 n2 = (v3 - v1).perp();
  Vec2 n3 = (v2 - v1).perp();

  Vec2 m1 = (1.0f / 2.0f) * (v3 + v2);
  Vec2 m2 = (1.0f / 2.0f) * (v3 + v1);
  Vec2 m3 = (1.0f / 2.0f) * (v2 + v1);

  {
    // The centroid is always inside a convex region, so this is *probably*
    // the optimal way to calculate inward facing normals without a specified
    // vertex order.
    Vec2 centroid = (1.0f / 3.0f) * (v1 + v2 + v3);
    n1 = ((centroid - m1) * n1) * n1;
    n2 = ((centroid - m2) * n2) * n2;
    n3 = ((centroid - m3) * n3) * n3;
  }

  // TODO: Find a bounding rectangle on the triangle, then fill.
  for (unsigned int i = 0; i < img.width; ++i)
  {
    for (unsigned int j = 0; j < img.height; ++j)
    {
      // All triangles are convex, so check if the pixel is inside by computing
      // inner products with the normal vectors.
      Vec2 pixel = Vec2(float(i) / float(img.width), float(img.height - j) / float(img.height));
      bool inside = (((pixel - m1) * n1) >= 0) && (((pixel - m2) * n2) >= 0)
        && (((pixel - m3) * n3) >= 0);
      if (inside)
      {
        unsigned int pixel_index = ((j * img.width) + i) * 4;
        unsigned char *pixel_data = &img.data[pixel_index];
        pixel_data[0] = float_to_uc(color.x);
        pixel_data[1] = float_to_uc(color.y);
        pixel_data[2] = float_to_uc(color.z);
        pixel_data[3] = float_to_uc(color.w);
      }
    }
  }
}

void
line_command(RawBitmap &img, const json &cmd)
{
  float width = cmd["width"];
  Vec4 color = json_to_vec4(cmd["color"]);
  Vec2 p1 = json_to_vec2(cmd["points"][0]);
  Vec2 p2 = json_to_vec2(cmd["points"][1]);
  float length = (p2 - p1).norm();
  Vec2 dir = (1 / length) * (p2 - p1);
  Vec2 n = dir.perp();

  // TODO: Find a bounding rectangle on the line, then fill.
  for (unsigned int i = 0; i < img.width; ++i)
  {
    for (unsigned int j = 0; j < img.height; ++j)
    {
      // All triangles are convex, so check if the pixel is inside by computing
      // inner products with the normal vectors.
      Vec2 pixel = Vec2(float(i) / float(img.width), float(img.height - j) / float(img.height));
      Vec2 projected_pt = pixel - (((pixel - p1) * n) * n);
      bool inside = (abs((pixel - projected_pt) * n) <= 0.5f * width)
        && ((projected_pt - p1) * dir >= 0) && ((projected_pt - p1) * dir <= length);
      if (inside)
      {
        unsigned int pixel_index = ((j * img.width) + i) * 4;
        unsigned char *pixel_data = &img.data[pixel_index];
        pixel_data[0] = float_to_uc(color.x);
        pixel_data[1] = float_to_uc(color.y);
        pixel_data[2] = float_to_uc(color.z);
        pixel_data[3] = float_to_uc(color.w);
      }
    }
  }
}

void
arc_command(RawBitmap &img, const json &cmd)
{
  float width = cmd["width"];
  float radius = cmd["radius"];
  float start_angle = (float)cmd["start_angle"] * 3.14159f;
  float end_angle = (float)cmd["end_angle"] * 3.14159f;
  Vec4 color = json_to_vec4(cmd["color"]);
  Vec2 center = json_to_vec2(cmd["center"]);

  // TODO: Find a bounding rectangle on the line, then fill.
  for (unsigned int i = 0; i < img.width; ++i)
  {
    for (unsigned int j = 0; j < img.height; ++j)
    {
      // All triangles are convex, so check if the pixel is inside by computing
      // inner products with the normal vectors.
      Vec2 pixel = Vec2(float(i) / float(img.width), float(img.height - j) / float(img.height));
      Vec2 r = pixel - center;
      Vec2 r_hat = r.normalized();
      float angle = acos(r_hat.x);
      if (r_hat.y < 0)
        angle = (2.0f * 3.14159f) - angle;

      bool inside = (abs(r.norm() - radius) <= 0.5f * width)
        && (angle >= start_angle) && (angle <= end_angle);
      if (inside)
      {
        unsigned int pixel_index = ((j * img.width) + i) * 4;
        unsigned char *pixel_data = &img.data[pixel_index];
        pixel_data[0] = float_to_uc(color.x);
        pixel_data[1] = float_to_uc(color.y);
        pixel_data[2] = float_to_uc(color.z);
        pixel_data[3] = float_to_uc(color.w);
      }
    }
  }
}

void
process_raster_command(RawBitmap &img, const json &cmd)
{
  std::string command_type = cmd["cmd"];
  if (command_type == "triangle")
  {
    triangle_command(img, cmd);
  }
  else if (command_type == "line")
  {
    line_command(img, cmd);
  }
  else if (command_type == "arc")
  {
    arc_command(img, cmd);
  }
}

Image *
render_bitmap_from_json(const json &data)
{
  // Render the image at 4x resolution, and then sample at the specified
  // resolution
  RawBitmap bitmap = {};
  bitmap.width = 4 * data["width"];
  bitmap.height = 4 * data["height"];
  bitmap.channels = 4;
  bitmap.data = new unsigned char[bitmap.width * bitmap.height * bitmap.channels];

  for (unsigned int i = 0; i < bitmap.width * bitmap.height * bitmap.channels; ++i)
  {
    bitmap.data[i] = 0x00;
  }

  // Setup is done, so loop through the commands
  for (const auto &command_kv : data["commands"].items())
  {
    process_raster_command(bitmap, command_kv.value());
  }

  RawBitmap sampled = {};
  sampled.width = data["width"];
  sampled.height = data["height"];
  sampled.channels = 4;
  sampled.data = new unsigned char[sampled.width * sampled.height * sampled.channels];

  for (unsigned int i = 0; i < sampled.width; ++i)
  {
    for (unsigned int j = 0; j < sampled.height; ++j)
    {
      Vec4 average = Vec4();
      for (unsigned int i2 = 0; i2 < 4; ++i2)
      {
        for (unsigned int j2 = 0; j2 < 4; ++j2)
        {
          unsigned int x = (4 * i) + i2;
          unsigned int y = (4 * j) + j2;
          unsigned int pixel_index = ((y * bitmap.width) + x) * 4;
          unsigned char *pixel_data = &bitmap.data[pixel_index];
          average += (1.0f / 16.0f) * Vec4(
            (1.0f / 255.0f) * float(pixel_data[0]),
            (1.0f / 255.0f) * float(pixel_data[1]),
            (1.0f / 255.0f) * float(pixel_data[2]),
            (1.0f / 255.0f) * float(pixel_data[3])
          );
        }
      }

      unsigned int pixel_index = ((j * sampled.width) + i) * 4;
      unsigned char *pixel_data = &sampled.data[pixel_index];
      pixel_data[0] = float_to_uc(average.x);
      pixel_data[1] = float_to_uc(average.y);
      pixel_data[2] = float_to_uc(average.z);
      pixel_data[3] = float_to_uc(average.w);
    }
  }

  Image *image_res = new Image(sampled.width, sampled.height,
    sampled.channels, sampled.data);
  delete[] bitmap.data;
  delete[] sampled.data;
  return image_res;
}
