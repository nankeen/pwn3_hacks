#pragma once

#include "cheat.hh"
class FlyingCheat: public Cheat
{
public:
    void on_game_tick(ClientWorld *);
};
