#pragma once

#include "cheat.hh"

class FlyingCheat: public Cheat
{
public:
    void on_world_tick(ClientWorld *) override;
};
