#include <dlfcn.h>
#include <iostream>
#include <gameLogic.hh>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <memory>

#include "cheat.hh"
#include "flying_cheat.hh"

// Used for X Server functions
static Display *display;

// Check key events
bool check_key(KeySym keysym)
{
    char keys_return[32];

    XQueryKeymap(display, keys_return);
    KeyCode kc = XKeysymToKeycode(display, keysym);
    return !!(keys_return[kc >> 3] & (1<<(kc&7)));
}

// Handle the world tick
// Can be use to listen to input events
void World::Tick(float f)
{
    // Get the game world object
    ClientWorld *world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    if (world == NULL) {
        std::cerr << "Couldn't locate 'GameWorld' symbol" << std::endl;
        return;
    }

    // Check which cheats are enabled
    if (false) {
    }
}

// Increase the player speed
void increase_speed(ClientWorld *world)
{
    Player *player = (Player *)(world->m_activePlayer.m_object); 
    player->m_walkingSpeed = 0xf00d;
}

// Open the display structure
__attribute__((constructor)) static void init_cheats()
{
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("XOpenDisplay error");
        exit(1);
    }

    // Register cheats
    cheats.insert({"fly_hacks", std::unique_ptr<Cheat>(new FlyingCheat())});
}

// Clean up the display structure
__attribute__((destructor)) static void cleanup_cheats()
{
    XCloseDisplay(display);
    cheats.clear();
}

// Implement basic cheat controls
void Cheat::enable()
{
    this->active = true;
}

void Cheat::disable()
{
    this->active = false;
}

void Cheat::toggle()
{
    this->active = !this->active;
}
