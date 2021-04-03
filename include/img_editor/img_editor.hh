#ifndef IMG_EDITOR_HH
#define IMG_EDITOR_HH

#include <launcher/launcher.h>
#include <core/screen.h>

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
