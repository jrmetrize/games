#ifndef IMG_EDITOR_HH
#define IMG_EDITOR_HH

#include <launcher/launcher.h>
#include <core/screen.h>
#include <core/graphics.h>

namespace ImgEditor
{

class ImgEditorScreen;

class ImgEditorState : public Launcher::LauncherGame
{
  ImgEditorScreen *screen;
public:
  ImgEditorState();

  ~ImgEditorState();

  void
  load();

  void
  start();

  void
  close();
};

class ImgEditorScreen : public Screen
{
  Texture *palette;
public:
  ImgEditorScreen();

  ~ImgEditorScreen();

  void
  update(float time_elapsed);

  void
  draw();
};

}

#endif
