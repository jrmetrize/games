#ifndef Z2D_Z2D_HH
#define Z2D_Z2D_HH

#include <core/screen.h>

#include <launcher/launcher.h>

namespace Z2D
{

class TitleScreen : public Screen
{

};

class Z2DState : public Launcher::LauncherGame
{
public:
  Z2DState();

  ~Z2DState();

  void
  load();

  void
  start();

  void
  close();
};

}

#endif
