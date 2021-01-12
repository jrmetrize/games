#include "screens/level_screen.h"
#include "state.h"
#include "level.h"
#include "graphics.h"
#include "imgui/imgui.h"

LevelScreen::LevelScreen(GameState *_state) :
  Screen(_state)
{
  level = new Level();
  level_state = new LevelState(level);
}

LevelScreen::~LevelScreen()
{
  delete level;
  delete level_state;
}

void
LevelScreen::update(InputMonitor *input, float time_elapsed)
{
  level_state->update(input, time_elapsed);
}

void
LevelScreen::draw_custom(GraphicsServer *graphics_server)
{
  // Draw the 2D representation of the level
  RenderRequest req = {};
  req.camera_pos = level_state->get_player_position();
  req.camera_dir = level_state->get_player_camera_direction();
  req.tree = level_state->get_render_tree();
  if (level->get_camera_mode() == Vertical)
  {
    req.camera_mode = Vertical;
    graphics_server->render_to_rect(Vec2(600, 0), Vec2(64, 720), req);
  }
  else
  {
    req.camera_mode = Horizontal;
    graphics_server->render_to_rect(Vec2(0, 600), Vec2(720, 64), req);
  }

  ImGui::Begin("Level Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    get_state()->level_screen_to_title_screen();
  }

  ImGui::End();
}
