#ifndef SCREEN_H
#define SCREEN_H

class GameState;
class InputMonitor;
class GraphicsServer;

class Screen
{
  GameState *state;
protected:
  GameState *
  get_state();
public:
  Screen(GameState *_state);

  virtual void
  update(InputMonitor *input, float time_elapsed) = 0;

  virtual void
  draw_custom(GraphicsServer *graphics_server) = 0;
};

#endif
