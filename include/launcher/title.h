#ifndef TITLE_H
#define TITLE_H

#include <core/screen.h>
#include <core/graphics.h>

#include "launcher/launcher.h"

namespace Launcher
{

class TitleScreen : public Screen
{
  LauncherState *launcher;

  Listener listener;

  struct Cubelet
  {
    SceneObject *object;
    Vec3 position;
  };

  Camera camera;
  DirectionalLight *light;
  std::vector<Cubelet> cubes;
  Scene3D *scene;

  void
  key_pressed(Key key, bool pressed);
public:
  TitleScreen(LauncherState *_launcher);

  ~TitleScreen();

  void
  resize(Vec2 window_size);

  void
  to_appear();

  void
  to_disappear();

  void
  update(float time_elapsed);

  void
  draw();
};

}

#endif
