#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "core/screen.h"
#include "core/input.h"

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
  to_appear();

  void
  to_disappear();

  void
  draw_custom();
};

#endif
