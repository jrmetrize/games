#ifndef LAUNCHER_LAUNCHER_H
#define LAUNCHER_LAUNCHER_H

#include <string>
#include <core/linear_algebra.h>

#define LAUNCHER_TITLE_CUBE_EDGE_SIZE 8

#define LAUNCHER_UI_GRAY Vec4(0.8f, 0.8f, 0.8f, 0.5f)
#define LAUNCHER_UI_HIGHLIGHT Vec4(1.0f, 0.8f, 0.8f, 0.5f)
#define LAUNCHER_UI_SELECT Vec4(1.0f, 1.0f, 0.8f, 0.5f)
#define LAUNCHER_UI_MARGIN 32.0f
#define LAUNCHER_UI_SPACING 16.0f

#define LAUNCHER_GRID_COLUMNS 3
#define LAUNCHER_CELL_IMG_ASPECT_RATIO (16.0f / 9.0f)

namespace Launcher
{
  class LauncherState
  {
  public:
    virtual void
    show_title_screen() = 0;

    virtual void
    show_game_select_screen() = 0;

    virtual void
    launch_game(std::string game) = 0;
  };

  class LauncherGame
  {
  public:
    virtual
    ~LauncherGame() = 0;

    virtual void
    load() = 0;

    virtual void
    start() = 0;

    virtual void
    close() = 0;
  };
}

#endif
