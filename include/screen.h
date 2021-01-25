#ifndef SCREEN_H
#define SCREEN_H

class Screen
{
public:
  Screen();

  virtual void
  update(float time_elapsed) = 0;

  virtual void
  draw_custom() = 0;
};

#endif
