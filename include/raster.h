#ifndef RASTER_H
#define RASTER_H

#include "resource.h"
#include "json.hpp"
using json = nlohmann::json;

Image *
render_bitmap_from_json(const json &data);

#endif
