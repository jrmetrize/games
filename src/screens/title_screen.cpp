#include "screens/title_screen.h"
#include "imgui/imgui.h"
#include "state.h"
#include "graphics.h"
#include "resource.h"

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

FontFace *f = nullptr;

void
TitleScreen::draw_custom(GraphicsServer *graphics_server)
{
  if (f == nullptr)
  {
    ResourceBundle *bundle = new ResourceBundle("C:\\Users\\jonat\\code\\fp2d\\test_bundle.rb");
    f = (FontFace *)bundle->get_resource("sans");
  }
  struct TextRenderRequest t = {};
  t.size = 20;
  t.color = Vec4(1, 0, 0, 1);
  t.font = f;
  t.text = "hello, world!";
  graphics_server->draw_text(t);

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
