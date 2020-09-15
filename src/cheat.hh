#pragma once

#include <gameLogic.hh>
#include <X11/Xlib.h>
#include <atomic>
#include <memory>

class Cheat
{
public:
    virtual void on_world_tick(ClientWorld *);
    void toggle();
    void enable();
    void disable();

    std::atomic<bool> active;
    int test;
};

extern std::unique_ptr<std::map<std::string, std::unique_ptr<Cheat>>> cheats;

bool check_key(KeySym keysym);
void enable_flying();

static void init_cheats() __attribute__((constructor));
static void cleanup_cheats() __attribute__((destructor));
