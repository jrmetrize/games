#include "screens/level_screen.h"
#include "state.h"
#include "imgui/imgui.h"

LevelScreen::LevelScreen(GameState *_state) :
  Screen(_state)
{

}

LevelScreen::~LevelScreen()
{

}

void
LevelScreen::draw_custom(GraphicsServer *graphics_server)
{
  ImGui::Begin("Level Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    get_state()->level_screen_to_title_screen();
  }

  ImGui::End();
}
