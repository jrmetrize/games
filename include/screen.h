#ifndef SCREEN_H
#define SCREEN_H

class GameState;
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
  draw_custom(GraphicsServer *graphics_server) = 0;
};

#endif
