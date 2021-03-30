#ifndef GAME_SELECT_H
#define GAME_SELECT_H

#include "core/screen.h"
#include <string>
#include <vector>

namespace Launcher
{

class GameEntry
{
  std::string title;
  std::string subtitle;
  std::string description;
  std::vector<std::string> tags;

  bool favorite;
public:
  GameEntry(std::string _title, std::string _subtitle, std::string _description,
    const std::vector<std::string> &_tags = {}, bool _favorite = false);

  std::string
  get_title() const;

  std::string
  get_subtitle() const;

  std::string
  get_description() const;

  const std::vector<std::string> &
  get_tags() const;

  bool
  is_favorite() const;

  void
  set_favorite(bool _favorite);

  static std::vector<GameEntry>
  get_game_list();
};

class GameEntryCard
{
  GameEntry &entry;

  Vec2 origin;
  Vec2 size;

  bool highlighted;
  bool pressed;
public:
  GameEntryCard(GameEntry &_entry);

  void
  set_origin(Vec2 _origin);

  void
  set_size(Vec2 _size);

  void
  update();

  void
  draw(Vec2 offset);
};

class GameSelectScreen : public Screen
{
  std::vector<GameEntry> games;

  std::vector<GameEntryCard> cards;

  Listener listener;
  float scroll_offset;
  float content_height;
public:
  GameSelectScreen();

  ~GameSelectScreen();

  void
  resize(Vec2 window_size);

  void
  to_appear();

  void
  to_disappear();

  void
  mouse_pressed(MouseButton button, bool pressed);

  void
  mouse_scrolled(Vec2 scroll);

  void
  update(float time_elapsed);

  void
  draw();
};

}

#endif
