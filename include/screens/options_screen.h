#ifndef OPTIONS_SCREEN_H
#define OPTIONS_SCREEN_H

#include "screen.h"
#include "state.h"
#include <vector>
#include "json.hpp"
using json = nlohmann::json;

class OptionsScreen : public Screen
{
  struct OptionsEntry
  {
    virtual float
    draw(Vec2 offset) = 0;
  };

  struct OptionSelector : public OptionsEntry
  {
    std::string text;

    float
    draw(Vec2 offset);
  };

  struct OptionsList : public OptionsEntry
  {
    std::string name;
    std::vector<OptionsEntry *> entries;
    bool open;

    float
    draw(Vec2 offset);
  };

  Listener listener;

  OptionsList root;
  float list_height;
  float scroll_offset;

  void
  mouse_pressed(MouseButton button, bool pressed);

  void
  mouse_scrolled(Vec2 scroll);

  void
  parse_options_group(const json &group, OptionsList *parent,
    std::map<std::string, PropertyData> &properties);
public:
  OptionsScreen();

  ~OptionsScreen();

  void
  build_tree(const json &tree_structure,
    std::map<std::string, PropertyData> &properties);

  void
  update(float time_elapsed);

  void
  to_appear();

  void
  to_disappear();

  void
  draw_custom();
};

#endif
