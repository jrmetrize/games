#ifndef RESOURCE_EDITOR_HH
#define RESOURCE_EDITOR_HH

#include <launcher/launcher.h>
#include <core/screen.h>
#include <core/graphics.h>

#define IMG_EDITOR_BG_COLOR Vec4(0.3, 0.3, 0.3, 1.0)
#define IMG_EDITOR_IMG_BG Vec4(0.1, 0.1, 0.1, 1.0)

namespace ResourceEditor
{

class ResourceEditorScreen;
class ResourceEditorState;

class ResourceEditorMenu : public MenuControl
{
  bool collapsed;

  MenuButton *open_button;
  MenuButton *new_button;

  ResourceEditorScreen *screen;

  void
  open_pressed();

  void
  new_pressed();
public:
  ResourceEditorMenu(ResourceEditorScreen *_screen);

  ~ResourceEditorMenu();

  void
  update(float time_elapsed);

  void
  draw();

  void
  mouse_pressed(MouseButton button, bool button_pressed);
};

class NewBufferDialog : public MenuControl
{
  enum DialogStage
  {
    DialogStageRoot,
    DialogStageOpenResource,
    DialogStageNew
  };
  DialogStage stage;

  ResourceEditorScreen *screen;

  /* Root */
  MenuButton *open_button;
  MenuButton *new_button;

  /* Open */

  /* New */
  MenuDropdownSelector *new_type_select;
  MenuButton *create_button;

  void
  open_pressed();

  void
  new_pressed();

  void
  create_pressed();
public:
  NewBufferDialog(ResourceEditorScreen *_screen);

  ~NewBufferDialog();

  void
  update(float time_elapsed);

  void
  draw();
};

class ResourceEditorScreen : public Screen
{
  ResourceEditorMenu *menu;
  NewBufferDialog *dialog;

  ResourceEditorState *state;
public:
  ResourceEditorScreen(ResourceEditorState *_state);

  ~ResourceEditorScreen();

  void
  to_appear();

  void
  to_disappear();

  void
  update(float time_elapsed);

  void
  draw();

  void
  show_new_dialog();
};

class Buffer
{
};

class ResourceEditorState : public Launcher::LauncherGame
{
  ResourceEditorScreen *screen;

  std::vector<Buffer *> buffers;
public:
  ResourceEditorState();

  ~ResourceEditorState();

  void
  load();

  void
  start();

  void
  close();
};

}

#endif
