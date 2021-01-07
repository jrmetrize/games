#include "screens/title_screen.h"
#include "imgui/imgui.h"
#include "state.h"

TitleScreen::TitleScreen(GameState *_state) :
  Screen(_state)
{

}

TitleScreen::~TitleScreen()
{

}

void
TitleScreen::update(InputMonitor *input, float time_elapsed)
{

}

void
TitleScreen::draw_custom(GraphicsServer *graphics_server)
{
  // TODO: actually render the menu through the engine and not imgui
  ImGui::Begin("Title Screen Menu");

  if (ImGui::Button("Play"))
  {
    get_state()->title_screen_to_level_screen();
  }

  if (ImGui::Button("Level Editor"))
  {
    get_state()->title_screen_to_level_editor_screen();
  }

  if (ImGui::Button("Options"))
  {
    get_state()->title_screen_to_options_screen();
  }

  if (ImGui::Button("Quit"))
  {
    get_state()->close_game();
  }

  ImGui::End();
}
