#include "screens/options_screen.h"
#include "state.h"
#include "imgui/imgui.h"

OptionsScreen::OptionsScreen(GameState *_state) :
  Screen(_state)
{

}

OptionsScreen::~OptionsScreen()
{

}

void
OptionsScreen::update(InputMonitor *input, float time_elapsed)
{

}

void
OptionsScreen::draw_custom(GraphicsServer *graphics_server)
{
  ImGui::Begin("Options Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    get_state()->options_screen_to_title_screen();
  }

  ImGui::End();
}
