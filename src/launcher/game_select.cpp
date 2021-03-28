#include "launcher/game_select.h"

#include <core/graphics.h>

namespace Launcher
{

GameSelectScreen::GameSelectScreen()
{

}

GameSelectScreen::~GameSelectScreen()
{

}

void
GameSelectScreen::update(float time_elapsed)
{

}

void
GameSelectScreen::draw_custom()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();

  GraphicsServer::get()->draw_color_rect(Vec2(0), Vec2(100), Vec4(1, 0, 0, 1));
}

}
