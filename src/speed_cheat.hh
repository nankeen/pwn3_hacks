#pragma once

#include "cheat.hh"
#include <X11/Xlib.h>

class SpeedCheat: public Cheat
{
public:
    bool check_toggle(Display *) override;
    void disable() override;
    void enable() override;
private:
    float m_walkingSpeed;
};

extern bool check_key(char *, KeySym);
