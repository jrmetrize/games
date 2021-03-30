#include "launcher/game_select.h"
#include "launcher/launcher.h"

#include <core/graphics.h>

namespace Launcher
{

GameEntry::GameEntry(std::string _title, std::string _subtitle, std::string _description,
  const std::vector<std::string> &_tags, bool _favorite) :
  title(_title), subtitle(_subtitle), description(_description), tags(_tags),
  favorite(_favorite)
{

}

std::string
GameEntry::get_title() const
{
  return title;
}

std::string
GameEntry::get_subtitle() const
{
  return subtitle;
}

std::string
GameEntry::get_description() const
{
  return description;
}

const std::vector<std::string> &
GameEntry::get_tags() const
{
  return tags;
}

bool
GameEntry::is_favorite() const
{
  return favorite;
}

void
GameEntry::set_favorite(bool _favorite)
{
  favorite = _favorite;
}

std::vector<GameEntry>
GameEntry::get_game_list()
{
  std::vector<GameEntry> games = {
    GameEntry("Agorafold", "First-person 2D adventure game", ""),
    GameEntry("2", "", ""),
    GameEntry("3", "", ""),
    GameEntry("4", "", "")
  };

  return games;
}

GameEntryCard::GameEntryCard(GameEntry &_entry, Vec2 _origin, Vec2 _size) :
  entry(_entry), origin(_origin), size(_size), highlighted(false),
  pressed(false)
{

}

void
GameEntryCard::update()
{
  /*
  if (!InputMonitor::get()->is_left_mouse_down())
  {
    bool prev_highlighted = highlighted;
    highlighted = InputMonitor::get()->get_mouse_position().inside_rect(
      origin, size);
    if (prev_highlighted == false && highlighted)
    {
      play_highlight_sound();
    }
  }
  */
}

void
GameEntryCard::draw(Vec2 offset, Vec2 size)
{
  GraphicsServer::get()->draw_color_rect(offset, size, LAUNCHER_UI_GRAY);

  TextRenderRequest req = {};
  req.bounding_box_origin = offset;
  req.bounding_box_size = Vec2(size.x, 20);
  req.text = entry.get_title();
  req.color = Vec4(0, 0, 0, 1);
  req.size = 12;
  req.font = GameState::get()->get_serif();
  req.center = false;
  req.center_vertical = false;
  GraphicsServer::get()->draw_text(req);
}

GameSelectScreen::GameSelectScreen() :
  games(GameEntry::get_game_list()), cards(), listener(),
  scroll_offset(0.0), content_height(0.0f)
{
  for (GameEntry &entry : games)
    cards.push_back(GameEntryCard(entry, Vec2(), Vec2()));

  listener.mouse_button_handle = std::bind(&GameSelectScreen::mouse_pressed, this,
    std::placeholders::_1, std::placeholders::_2);
  listener.scroll_handle = std::bind(&GameSelectScreen::mouse_scrolled, this,
    std::placeholders::_1);
}

GameSelectScreen::~GameSelectScreen()
{

}

void
GameSelectScreen::to_appear()
{
  InputMonitor::get()->install_listener(&listener);
}

void
GameSelectScreen::to_disappear()
{
  InputMonitor::get()->remove_listener(&listener);
}

void
GameSelectScreen::mouse_pressed(MouseButton button, bool pressed)
{
  /*root.mouse_pressed(button, pressed);
  back_button->mouse_pressed(button, pressed);*/
}

void
GameSelectScreen::mouse_scrolled(Vec2 scroll)
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  scroll_offset += -1.0f * 3.0f * scroll.y;
  if (scroll_offset < 0.0f)
    scroll_offset = 0.0f;

  if (content_height <= window_size.y)
    scroll_offset = 0.0f;
  else if (scroll_offset > content_height - window_size.y)
    scroll_offset = content_height - window_size.y;
}

void
GameSelectScreen::update(float time_elapsed)
{
  for (GameEntryCard &card : cards)
    card.update();
}

void
GameSelectScreen::draw_custom()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();

  float cell_width = floor((window_size.x
    - (2.0 * LAUNCHER_UI_MARGIN)
    - (float(LAUNCHER_GRID_COLUMNS - 1) * LAUNCHER_UI_SPACING))
    / float(LAUNCHER_GRID_COLUMNS));
  float cell_height = floor(cell_width / float(LAUNCHER_CELL_IMG_ASPECT_RATIO));

  /* Now that all the relevant metrics are known, draw the cells in a grid. */
  /* TODO: Cache the grid coordinates of each cell? I doubt the launcher would
     ever need optimization, but try this just in case. */
  for (uint32_t i = 0; i < cards.size(); ++i)
  {
    uint32_t row = i / LAUNCHER_GRID_COLUMNS;
    uint32_t col = i % LAUNCHER_GRID_COLUMNS;

    Vec2 cell_pos = Vec2(
      LAUNCHER_UI_MARGIN + (float(col) * cell_width) + (float(col) * LAUNCHER_UI_SPACING),
      window_size.y - LAUNCHER_UI_MARGIN - (float(row + 1) * cell_height) - (float(row) * LAUNCHER_UI_SPACING) + scroll_offset);
    cards[i].draw(cell_pos, Vec2(cell_width, cell_height));
  }

  /* Calculate the total number of rows. */
  uint32_t rows = cards.size() / LAUNCHER_GRID_COLUMNS;
  if (cards.size() % LAUNCHER_GRID_COLUMNS > 0)
    rows += 1;

  if (rows > 0)
    content_height = (2.0f * LAUNCHER_UI_MARGIN) + (float(rows) * cell_height)
      + (float(rows - 1) * LAUNCHER_UI_SPACING);
  else
    content_height = 0.0f;
}

}
