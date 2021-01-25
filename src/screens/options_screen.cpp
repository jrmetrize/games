#include "screens/options_screen.h"
#include "state.h"
#include "imgui/imgui.h"

OptionsScreen::OptionsScreen()
{

}

OptionsScreen::~OptionsScreen()
{

}

void
OptionsScreen::update(float time_elapsed)
{

}

void
OptionsScreen::draw_custom()
{
  ImGui::Begin("Options Menu");

  if (ImGui::Button("Back to Main Menu"))
  {
    GameState::get()->switch_to_screen(GameState::get()->get_title_screen());
  }

  ImGui::End();
}
