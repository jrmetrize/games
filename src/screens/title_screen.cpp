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

Texture *tex = nullptr;

void
TitleScreen::draw_custom(GraphicsServer *graphics_server)
{
  if (tex == nullptr)
  {
    ResourceBundle *bundle = new ResourceBundle("C:\\Users\\jonat\\code\\fp2d\\test_bundle.rb");
    FontFace *font = (FontFace *)bundle->get_resource("sans");
    Glyph g = font->get_glyph('a');
    tex = new Texture(g.bitmap_width, g.bitmap_height, 1, g.bitmap_data);
  }
  graphics_server->draw_texture_rect(Vec2(10, 10), Vec2(16, 16), *tex);

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
