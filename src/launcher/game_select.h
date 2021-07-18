#ifndef GAME_SELECT_H
#define GAME_SELECT_H

#include <core/screen.h>
#include <string>
#include <vector>

#include "launcher/launcher.h"

namespace Launcher
{

class GameEntry
{
  std::string id;
  std::string title;
  std::string subtitle;
  std::string description;
  std::vector<std::string> tags;

  bool favorite;
public:
  GameEntry(std::string _id, std::string _title, std::string _subtitle,
    std::string _description, const std::vector<std::string> &_tags = {},
    bool _favorite = false);

  std::string
  get_id() const;

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

class GameEntryCard : public MenuControl
{
  GameEntry &entry;

  bool highlighted;
  bool pressed;

  std::function<void(GameEntry &)> target;
public:
  GameEntryCard(GameEntry &_entry, std::function<void(GameEntry &)> _target);

  void
  handle_event(const MenuControlEvent *event);

  void
  update(float time_elapsed);

  void
  draw();
};

class GameSelectScreen : public Screen
{
  LauncherState *launcher;

  std::vector<GameEntry> games;

  std::vector<GameEntryCard *> cards;

  float scroll_offset;
  float content_height;

  void
  entry_selected(GameEntry &entry);
protected:
  void
  handle_event(const MenuControlEvent *event);
public:
  GameSelectScreen(LauncherState *_launcher);

  ~GameSelectScreen();

  void
  resize(Vec2 window_size);

  void
  to_appear();

  void
  to_disappear();

  void
  update(float time_elapsed);

  void
  draw();
};

}

#endif
