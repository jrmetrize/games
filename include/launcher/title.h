#ifndef TITLE_H
#define TITLE_H

#include <core/screen.h>
#include <core/graphics.h>

namespace Launcher
{

class TitleScreen : public Screen
{
  Listener listener;

  Camera camera;
  DirectionalLight *light;
  SceneObject *obj;
  Scene3D *scene;
public:
  TitleScreen();

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
