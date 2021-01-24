#include "screens/level_screen.h"
#include "state.h"
#include "level.h"
#include "graphics.h"
#include "imgui/imgui.h"
#include "levels/training_levels.h"

LevelScreen::LevelScreen(GameState *_state) :
  Screen(_state)
{
  level = new TrainingLevelController();
}

LevelScreen::~LevelScreen()
{
  delete level;
}

void
LevelScreen::update(InputMonitor *input, float time_elapsed)
{
  level->get_level_state()->update(input, time_elapsed);
}

void
LevelScreen::draw_custom(GraphicsServer *graphics_server)
{
  // Draw the 2D representation of the level
  // On the left, the visualizer thing (just cuteness, not anything important)
  {
    Vec2 screen_size = GraphicsServer::get()->get_framebuffer_size();
    float background_width = (3.0f / 8.0f) * screen_size.x;
    level->get_level_state()->draw_background_in_rect(graphics_server,
      Vec2(0, 0), Vec2(background_width, screen_size.y));
  }

  // In the center, the first person view
  {
    LevelState *level_state = level->get_level_state();
    RenderRequest req = {};
    req.camera_pos = level_state->get_player_position();
    req.camera_dir = level_state->get_player_camera_direction();
    req.tree = level_state->get_render_tree();
    req.camera_mode = Vertical;
    Vec2 screen_size = GraphicsServer::get()->get_framebuffer_size();
    float view_width = screen_size.x / 4.0f;
    graphics_server->render_to_rect(Vec2((screen_size.x - view_width) / 2.0f, 0),
      Vec2(view_width, screen_size.y), req);
  }

  // On the right, stats, menus, and stuff
  {
    Vec2 screen_size = GraphicsServer::get()->get_framebuffer_size();
    float width = (3.0f / 8.0f) * screen_size.x;
    float offset = (5.0f / 8.0f) * screen_size.x;
    level->get_level_state()->draw_stats_in_rect(graphics_server,
      Vec2(offset, 0), Vec2(width, screen_size.y));
  }

  ImGui::Begin("Level Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    get_state()->level_screen_to_title_screen();
  }

  ImGui::End();
}
