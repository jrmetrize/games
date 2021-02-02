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

    virtual void
    update(float time_elapsed) = 0;

    virtual void
    mouse_pressed(MouseButton button, bool pressed) = 0;
  };

  struct OptionSelector : public OptionsEntry
  {
    PropertyData *game_property;
    std::string text;
    MenuControl *property_control;

    float
    draw(Vec2 offset);

    void
    update(float time_elapsed);

    void
    mouse_pressed(MouseButton button, bool pressed);

    void
    bool_switch_changed(bool value);

    void
    range_slider_changed(float value);
  };

  struct OptionsList : public OptionsEntry
  {
    std::string name;
    std::vector<OptionsEntry *> entries;
    bool open;
    MenuButton *arrow_button;

    float
    draw(Vec2 offset);

    void
    update(float time_elapsed);

    void
    mouse_pressed(MouseButton button, bool pressed);
  };

  Listener listener;
  MenuButton *back_button;

  OptionsList root;
  float list_height;
  float scroll_offset;

  void
  mouse_pressed(MouseButton button, bool pressed);

  void
  mouse_scrolled(Vec2 scroll);

  void
  toggle_list_open(OptionsList *list);

  void
  back_pressed();

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
