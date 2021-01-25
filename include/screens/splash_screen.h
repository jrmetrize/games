#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "screen.h"
#include "input.h"

class SplashScreen : public Screen
{
  Listener listener;

  void
  key_pressed(Key key, bool pressed);
public:
  SplashScreen();

  ~SplashScreen();

  void
  update(float time_elapsed);

  void
  draw_custom();
};

#endif
