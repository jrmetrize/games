#include "screens/level_editor_screen.h"
#include "state.h"
#include "imgui/imgui.h"

LevelEditorScreen::LevelEditorScreen(GameState *_state) :
  Screen(_state)
{

}

LevelEditorScreen::~LevelEditorScreen()
{

}

void
LevelEditorScreen::draw_custom(GraphicsServer *graphics_server)
{
  ImGui::Begin("Level Editor Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    get_state()->level_editor_screen_to_title_screen();
  }

  ImGui::End();
}
