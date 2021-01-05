#ifndef STATE_H
#define STATE_H

class GameState
{
  bool should_close;
public:
  GameState();

  ~GameState();

  void
  close_game();

  bool
  game_open() const;
};

#endif
