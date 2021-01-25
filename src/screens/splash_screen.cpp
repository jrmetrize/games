#include "screens/splash_screen.h"
#include "graphics.h"
#include "state.h"

SplashScreen::SplashScreen()
{
  listener.key_handle = std::bind(&SplashScreen::key_pressed, this,
    std::placeholders::_1, std::placeholders::_2);

  InputMonitor::get()->install_listener(&listener);
}

SplashScreen::~SplashScreen()
{
  InputMonitor::get()->remove_listener(&listener);
}

void
SplashScreen::key_pressed(Key key, bool pressed)
{
  // TODO: should probably play a sound here
  GameState::get()->switch_to_screen(GameState::get()->get_title_screen());
}

void
SplashScreen::update(float time_elapsed)
{

}

void
SplashScreen::draw_custom()
{
  {
    // Blinking start text
    Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
    float sine = sin(3.0f * GameState::get()->get_time());
    float brightness = 0.7f + (0.3f * sine);

    TextRenderRequest req = {};
    req.bounding_box_origin = Vec2(0, 100);
    req.bounding_box_size = Vec2(window_size.x, 50);
    req.text = "Press any key/button to start.";
    req.color = Vec4(brightness, brightness, brightness, 1);
    req.size = 36;
    req.font = GameState::get()->get_serif();
    req.center = true;

    GraphicsServer::get()->draw_text(req);
  }
}
