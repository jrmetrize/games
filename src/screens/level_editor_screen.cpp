#include "screens/level_editor_screen.h"
#include "state.h"
#include "level.h"
#include "graphics.h"
#include "imgui/imgui.h"

LevelEditorScreen::LevelEditorScreen(GameState *_state) :
  Screen(_state),
  paused(false)
{
  level = new Level();
  level_state = new LevelState(level);
}

LevelEditorScreen::~LevelEditorScreen()
{
  delete level;
  delete level_state;
}

void
LevelEditorScreen::update(InputMonitor *input, float time_elapsed)
{
  if (!paused)
    level_state->update(input, time_elapsed);
}

int camera_mode = 0;

void
LevelEditorScreen::draw_custom(GraphicsServer *graphics_server)
{
  // Draw the 2D representation of the level
  level_state->draw_editor_view_in_rect(graphics_server,
    Vec2(10, 10), Vec2(512, 512));

  char name_tmp[256];
  strcpy(name_tmp, level->get_name().c_str());
  float gravity_tmp = level->get_gravity();
  int camera_mode_tmp = level->get_camera_mode();

  // Draw GUI elements for the level editor
  ImGui::Begin("Level Editor Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    get_state()->level_editor_screen_to_title_screen();
  }

  if (ImGui::Button("Stop"))
  {
    paused = true;
  }

  ImGui::SameLine();
  if (ImGui::Button("Start"))
  {
    paused = false;
  }

  ImGui::SameLine();
  if (ImGui::Button("Reset"))
  {
    delete level_state;
    level_state = new LevelState(level);
  }

  ImGui::End();

  ImGui::Begin("Level Properties");
  ImGui::InputText("Level Name", name_tmp, 256);
  ImGui::InputFloat("Gravity", &gravity_tmp);
  ImGui::RadioButton("Vertical Camera", &camera_mode_tmp, 0);
  ImGui::RadioButton("Horizontal Camera", &camera_mode_tmp, 1);
  ImGui::End();

  ImGui::Begin("Info");
  {
    std::string str = "Player Location: "
      + level_state->get_player_position().string();
    ImGui::Text(str.c_str());
  }

  ImGui::End();

  level->set_name(std::string(name_tmp));
  level->set_gravity(gravity_tmp);
  level->set_camera_mode((CameraMode)camera_mode_tmp);
}
