#pragma once

#include <gameLogic.hh>
#include <X11/Xlib.h>
#include <atomic>
#include <memory>

class Cheat
{
public:
    Cheat();
    virtual void on_game_tick(ClientWorld *);
    void toggle();
    void enable();
    void disable();

    std::atomic<bool> active;
};

// Used to track the state of cheats
// TODO: Determine if this needs to be thread safe
static std::map<std::string, std::unique_ptr<Cheat>> cheats;

bool check_key(KeySym keysym);
void enable_flying();

static void init_cheats() __attribute__((constructor));
static void cleanup_cheats() __attribute__((destructor));
